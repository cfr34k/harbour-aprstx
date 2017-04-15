#ifndef APRSCTRL_H
#define APRSCTRL_H

#include <QObject>

#include <QAudioOutput>
#include <QBuffer>
#include <QTimer>

#include "aprs.h"
#include "afskmod.h"

class APRSCtrl : public QObject
{
	Q_OBJECT

	Q_PROPERTY(TXState txstate READ get_tx_state WRITE set_tx_state NOTIFY tx_state_changed)
	Q_PROPERTY(unsigned int secondsToAutoTx READ get_seconds_to_auto_tx WRITE set_seconds_to_auto_tx NOTIFY seconds_to_auto_tx_changed)

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

	TXState m_txState;

	void set_tx_state(TXState state) { m_txState = state; emit tx_state_changed(state); }

public:
	explicit APRSCtrl(QObject *parent = 0);

	~APRSCtrl();

	TXState get_tx_state() { return m_txState; }
	unsigned int get_seconds_to_auto_tx() { return m_secondsToAutoTx; }

	Q_INVOKABLE void auto_tx_start(void);
	Q_INVOKABLE void auto_tx_stop(void);

signals:
	void tx_state_changed(TXState state);
	void seconds_to_auto_tx_changed(unsigned int seconds);

private slots:
	void audio_state_changed(QAudio::State state);
	void auto_tx_tick(void);

public slots:
	void transmit_packet(void);

	void set_seconds_to_auto_tx(unsigned int seconds) { m_secondsToAutoTx = seconds; emit seconds_to_auto_tx_changed(seconds); }
};

#endif // APRSCTRL_H
