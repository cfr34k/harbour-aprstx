#include <QDebug>

#include <QAudioDeviceInfo>

#include "aprsctrl.h"

#include "settings.h"

APRSCtrl::APRSCtrl(QObject *parent) : QObject(parent)
{
	m_aprs = new APRS();
	m_afsk = new AFSKMod(12000);

	m_sampleIO = new QBuffer();

	m_aprs->set_source(Settings::instance().getUserCall().toUtf8().data(), 0);
	m_aprs->set_dest("GPS", 0);

	m_aprs->add_path("WIDE1", 1);
	m_aprs->add_path("WIDE2", 2);

	m_aprs->set_comment(Settings::instance().getComment().toUtf8().data());

	m_aprs->set_icon(APRS::AI_BIKE);

	m_aprs->update_pos_time(49.58659, 11.01217, 300, time(NULL));

	// setup audio
	QAudioFormat audioFormat;
	audioFormat.setSampleRate(12000);
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
	delete m_audioOutput;

	delete m_aprs;
	delete m_afsk;
}

void APRSCtrl::audio_state_changed(QAudio::State state)
{
	switch(state) {
	case QAudio::IdleState:
		m_audioOutput->stop();
		break;

	case QAudio::StoppedState:
		set_tx_state(Disabled);
		m_sampleIO->close();
		break;

	default:
		break;
	}
}

void APRSCtrl::transmit_packet(void)
{
	QByteArray sampleStorage;

	int16_t tmpSamples[12000]; // 1 second of sample storage
	uint32_t numSamples;

	uint8_t aprsFrame[APRS_MAX_FRAME_LEN];
	uint32_t aprsFrameLen;

	numSamples = m_afsk->gen_vox_tone(300, tmpSamples);
	sampleStorage.append((const char*)tmpSamples, numSamples*sizeof(int16_t));

	numSamples = m_afsk->mod_byte(0x7E, tmpSamples, 0);
	sampleStorage.append((const char*)tmpSamples, numSamples*sizeof(int16_t));

	aprsFrameLen = m_aprs->build_frame(aprsFrame);

	for(size_t i = 0; i < aprsFrameLen; i++) {
		numSamples = m_afsk->mod_byte(aprsFrame[i], tmpSamples, 1);
		sampleStorage.append((const char*)tmpSamples, numSamples*sizeof(int16_t));
	}

	numSamples = m_afsk->mod_byte(0x7E, tmpSamples, 0);
	sampleStorage.append((const char*)tmpSamples, numSamples*sizeof(int16_t));

	// TODO: setup audio, create QBuffer from sampleStorage, play samples
	m_sampleIO->setData(sampleStorage);

	m_sampleIO->open(QBuffer::ReadOnly);

	m_audioOutput->start(m_sampleIO);
	set_tx_state(Transmitting);
}
