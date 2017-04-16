#include <QDebug>

#include <QAudioDeviceInfo>

#include "aprsctrl.h"

#include "settings.h"

APRSCtrl::APRSCtrl(QObject *parent)
	: QObject(parent),
		m_txState(APRSCtrl::Disabled),
		m_latitude(42.086), m_longitude(23.456), m_altitude(333.25)
{
	m_aprs = new APRS();
	m_afsk = new AFSKMod(APRS_AFSK_SAMPLE_RATE);

	m_sampleIO = new QBuffer();

	m_autoTxTimer = new QTimer();
	m_autoTxTimer->setInterval(1000);

	connect(m_autoTxTimer, SIGNAL(timeout()), this, SLOT(auto_tx_tick()));

	m_positionSource = QGeoPositionInfoSource::createDefaultSource(this);

	if(m_positionSource) {
		connect(m_positionSource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
		m_positionSource->startUpdates();
	}

	connect(&(Settings::instance()), SIGNAL(autoTxIntervalChanged(uint)), this, SLOT(set_seconds_to_auto_tx(uint)));

	m_aprs->set_source(Settings::instance().getUserCall().toUtf8().data(), 0);
	m_aprs->set_dest("GPS", 0);

	m_aprs->add_path("WIDE1", 1);
	m_aprs->add_path("WIDE2", 2);

	m_aprs->set_comment(Settings::instance().getComment().toUtf8().data());

	m_aprs->set_icon(APRS::AI_BIKE);

	m_aprs->update_pos_time(49.58659, 11.01217, 300, time(NULL));

	// setup audio
	QAudioFormat audioFormat;
	audioFormat.setSampleRate(APRS_AFSK_SAMPLE_RATE);
	audioFormat.setChannelCount(1);
	audioFormat.setSampleSize(16);
	audioFormat.setCodec("audio/pcm");
	audioFormat.setByteOrder(QAudioFormat::LittleEndian);
	audioFormat.setSampleType(QAudioFormat::SignedInt);

	if (!QAudioDeviceInfo::defaultOutputDevice().isFormatSupported(audioFormat)) {
			qWarning() << "Default format not supported - trying to use nearest";
			audioFormat = QAudioDeviceInfo::defaultOutputDevice().nearestFormat(audioFormat);
	}

	m_audioOutput = new QAudioOutput(audioFormat, this);

	connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audio_state_changed(QAudio::State)));
}

APRSCtrl::~APRSCtrl()
{
	delete m_autoTxTimer;

	delete m_audioOutput;
	delete m_sampleIO;

	delete m_positionSource;

	delete m_aprs;
	delete m_afsk;
}

void APRSCtrl::auto_tx_start(void)
{
	set_seconds_to_auto_tx(Settings::instance().getAutoTxInterval());
	set_tx_state(Waiting);
	m_autoTxTimer->start();
}

void APRSCtrl::auto_tx_stop(void)
{
	m_autoTxTimer->stop();
	set_tx_state(Disabled);
}

void APRSCtrl::auto_tx_tick()
{
	set_seconds_to_auto_tx(m_secondsToAutoTx-1);

	if(m_secondsToAutoTx == 0) {
		transmit_packet();
	}
}

void APRSCtrl::audio_state_changed(QAudio::State state)
{
	switch(state) {
	case QAudio::IdleState:
		m_audioOutput->stop();
		break;

	case QAudio::StoppedState:
		if(m_autoTxTimer->isActive()) {
			set_seconds_to_auto_tx(Settings::instance().getAutoTxInterval());
			set_tx_state(Waiting);
		} else {
			set_tx_state(Disabled);
		}
		m_sampleIO->close();
		break;

	default:
		break;
	}
}

void APRSCtrl::transmit_packet(void)
{
	QByteArray sampleStorage;

	int16_t tmpSamples[APRS_AFSK_SAMPLE_RATE]; // 1 second of sample storage
	uint32_t numSamples;

	uint8_t aprsFrame[APRS_MAX_FRAME_LEN];
	uint32_t aprsFrameLen;

	uint ntailFlags = Settings::instance().getTailFlags() + 1;
	uint voxToneDurationMs = Settings::instance().getVoxToneDuration();

	// add some zero samples to make the audio stable before transmission
	memset(tmpSamples, 0, sizeof(tmpSamples));
	sampleStorage.append((const char*)tmpSamples, (APRS_AFSK_SAMPLE_RATE*30/100)*sizeof(int16_t));

	// generate VOX tone in chunks of 100 ms
	while(voxToneDurationMs > 100) {
		numSamples = m_afsk->gen_vox_tone(100, tmpSamples);
		sampleStorage.append((const char*)tmpSamples, numSamples*sizeof(int16_t));

		voxToneDurationMs -= 100;

		qDebug() << "VOX: Generated " << numSamples << " samples (" << sampleStorage.size() << " total)";
	}

	numSamples = m_afsk->gen_vox_tone(voxToneDurationMs, tmpSamples);
	sampleStorage.append((const char*)tmpSamples, numSamples*sizeof(int16_t));

	qDebug() << "VOX: Generated " << numSamples << " samples (" << sampleStorage.size() << " total)";

	// generate leading Flag
	numSamples = m_afsk->mod_byte(0x7E, tmpSamples, 0);
	sampleStorage.append((const char*)tmpSamples, numSamples*sizeof(int16_t));

	qDebug() << "Flag: Generated " << numSamples << " samples (" << sampleStorage.size() << " total)";

	// generate the frame data...
	aprsFrameLen = m_aprs->build_frame(aprsFrame);

	// ... and modulate it
	for(size_t i = 0; i < aprsFrameLen; i++) {
		numSamples = m_afsk->mod_byte(aprsFrame[i], tmpSamples, 1);
		sampleStorage.append((const char*)tmpSamples, numSamples*sizeof(int16_t));

		qDebug() << "Data: Generated " << numSamples << " samples (" << sampleStorage.size() << " total)";
	}

	// generate tail flags, at least 1
	for(size_t i = 0; i < ntailFlags; i++) {
		numSamples = m_afsk->mod_byte(0x7E, tmpSamples, 0);
		sampleStorage.append((const char*)tmpSamples, numSamples*sizeof(int16_t));

		qDebug() << "Flag: Generated " << numSamples << " samples (" << sampleStorage.size() << " total)";
	}

	// add some zero samples to make the end of the transmission stable
	memset(tmpSamples, 0, sizeof(tmpSamples));
	sampleStorage.append((const char*)tmpSamples, (APRS_AFSK_SAMPLE_RATE*10/100)*sizeof(int16_t));

	// send the data to the audio output
	m_sampleIO->setData(sampleStorage);

	m_sampleIO->open(QBuffer::ReadOnly);

	m_audioOutput->start(m_sampleIO);
	set_tx_state(Transmitting);
}

void APRSCtrl::positionUpdated(const QGeoPositionInfo &posInfo)
{
	// update coordinates
	m_latitude  = posInfo.coordinate().latitude();
	m_longitude = posInfo.coordinate().longitude();
	m_altitude  = posInfo.coordinate().altitude();

	emit latitudeChanged(m_latitude);
	emit longitudeChanged(m_longitude);
	emit altitudeChanged(m_altitude);

	m_aprs->update_pos_time(m_latitude, m_longitude, m_altitude, time(NULL));
}
