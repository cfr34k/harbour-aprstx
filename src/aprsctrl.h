#ifndef APRSCTRL_H
#define APRSCTRL_H

#include <QObject>

#include <QAudioOutput>
#include <QBuffer>

#include "aprs.h"
#include "afskmod.h"

class APRSCtrl : public QObject
{
	Q_OBJECT

	Q_PROPERTY(TXState txstate READ get_tx_state WRITE set_tx_state NOTIFY tx_state_changed)

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

	QAudioOutput *m_audioOutput;

	QBuffer *m_sampleIO;

	TXState m_txState;

	void set_tx_state(TXState state) { m_txState = state; emit tx_state_changed(state); }

public:
	explicit APRSCtrl(QObject *parent = 0);

	~APRSCtrl();

	TXState get_tx_state() { return m_txState; }

signals:
	void tx_state_changed(TXState state);

private slots:
	void audio_state_changed(QAudio::State state);

public slots:
	void transmit_packet(void);
};

#endif // APRSCTRL_H
