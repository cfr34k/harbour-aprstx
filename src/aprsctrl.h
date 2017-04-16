#ifndef APRSCTRL_H
#define APRSCTRL_H

#include <QObject>

#include <QAudioOutput>
#include <QBuffer>
#include <QTimer>

#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

#include "aprs.h"
#include "afskmod.h"

#define APRS_AFSK_SAMPLE_RATE 12000

class APRSCtrl : public QObject
{
	Q_OBJECT

	Q_PROPERTY(TXState txstate READ get_tx_state WRITE set_tx_state NOTIFY tx_state_changed)
	Q_PROPERTY(unsigned int secondsToAutoTx READ get_seconds_to_auto_tx WRITE set_seconds_to_auto_tx NOTIFY seconds_to_auto_tx_changed)


	Q_PROPERTY(qreal latitude  READ get_latitude  NOTIFY latitudeChanged)
	Q_PROPERTY(qreal longitude READ get_longitude NOTIFY longitudeChanged)
	Q_PROPERTY(qreal altitude  READ get_altitude  NOTIFY altitudeChanged)

public:
	enum TXState {
		Disabled,
		Waiting,
		Transmitting
	};

	Q_ENUMS(TXState)

private:
	APRS    *m_aprs;
	AFSKMod *m_afsk;

	QTimer *m_autoTxTimer;
	unsigned int m_secondsToAutoTx;

	QAudioOutput *m_audioOutput;

	QBuffer *m_sampleIO;

	QGeoPositionInfoSource *m_positionSource;

	TXState m_txState;

	qreal m_latitude;
	qreal m_longitude;
	qreal m_altitude;

	void set_tx_state(TXState state) { m_txState = state; emit tx_state_changed(state); }

public:
	explicit APRSCtrl(QObject *parent = 0);

	~APRSCtrl();

	TXState get_tx_state() { return m_txState; }
	unsigned int get_seconds_to_auto_tx() { return m_secondsToAutoTx; }

	qreal get_latitude() { return m_latitude; }
	qreal get_longitude() { return m_longitude; }
	qreal get_altitude() { return m_altitude; }

	Q_INVOKABLE void auto_tx_start(void);
	Q_INVOKABLE void auto_tx_stop(void);

signals:
	void tx_state_changed(TXState state);
	void seconds_to_auto_tx_changed(unsigned int seconds);

	void latitudeChanged(qreal lat);
	void longitudeChanged(qreal lon);
	void altitudeChanged(qreal alt);

private slots:
	void audio_state_changed(QAudio::State state);
	void auto_tx_tick(void);
	void positionUpdated(const QGeoPositionInfo &posInfo);

public slots:
	void transmit_packet(void);

	void set_seconds_to_auto_tx(unsigned int seconds) { m_secondsToAutoTx = seconds; emit seconds_to_auto_tx_changed(seconds); }

	void updateUserCall(const QString &call);
	void updateUserSSID(uint ssid);
	void updateComment(const QString &comment);
	void updateIconIndex(uint index);
};

#endif // APRSCTRL_H
