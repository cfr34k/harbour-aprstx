/*
	This file is part of APRSTX - An APRS transmitter for SailfishOS
	Copyright (C) 2017  Thomas Kolb

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef APRSCTRL_H
#define APRSCTRL_H

#include <QObject>

#include <QAudioOutput>
#include <QBuffer>
#include <QTimer>

#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

#include <time.h>

#include "aprs.h"
#include "afskmod.h"

#define APRS_AFSK_SAMPLE_RATE 12000

class APRSCtrl : public QObject
{
	Q_OBJECT

	Q_PROPERTY(TXState txstate READ get_tx_state WRITE set_tx_state NOTIFY tx_state_changed)
	Q_PROPERTY(unsigned int secondsToAutoTx READ get_seconds_to_auto_tx NOTIFY seconds_to_auto_tx_changed)

	Q_PROPERTY(qreal latitude      READ get_latitude      NOTIFY latitudeChanged)
	Q_PROPERTY(qreal longitude     READ get_longitude     NOTIFY longitudeChanged)
	Q_PROPERTY(qreal altitude      READ get_altitude      NOTIFY altitudeChanged)
	Q_PROPERTY(qreal distance      READ get_distance      NOTIFY distanceChanged)
	Q_PROPERTY(qreal headingChange READ get_headingChange NOTIFY headingChangeChanged)

public:
	enum TXState {
		Disabled,
		Waiting,
		WaitingTriggered,
		Transmitting
	};

	Q_ENUMS(TXState)

private:
	APRS    *m_aprs;
	AFSKMod *m_afsk;

	QTimer *m_autoTxTimer;
    time_t m_nextTxMin;      //!< Timestamp when the next transmission is allowed
    time_t m_nextTxForced;   //!< Timestamp when the next transmission is forced independent of position

	QAudioOutput *m_audioOutput;

	QBuffer *m_sampleIO;

	QGeoPositionInfoSource *m_positionSource;

	TXState m_txState;

	qreal m_latitude;
	qreal m_longitude;
	qreal m_altitude;
	qreal m_distance;
	qreal m_headingChange;

    QGeoPositionInfo m_currentPositionInfo;
    QGeoPositionInfo m_lastTxPositionInfo;

	void set_tx_state(TXState state) { m_txState = state; emit tx_state_changed(state); }

public:
	explicit APRSCtrl(QObject *parent = 0);

	~APRSCtrl();

	TXState get_tx_state() { return m_txState; }
	unsigned int get_seconds_to_auto_tx();

	qreal get_latitude() { return m_latitude; }
	qreal get_longitude() { return m_longitude; }
	qreal get_altitude() { return m_altitude; }
	qreal get_distance() { return m_distance; }
	qreal get_headingChange() { return m_headingChange; }

	Q_INVOKABLE void auto_tx_start(void);
	Q_INVOKABLE void auto_tx_stop(void);

signals:
	void tx_state_changed(TXState state);
	void seconds_to_auto_tx_changed(unsigned int seconds);

	void latitudeChanged(qreal lat);
	void longitudeChanged(qreal lon);
	void altitudeChanged(qreal alt);
	void distanceChanged(qreal distance);
	void headingChangeChanged(qreal headingChange);

private slots:
	void audio_state_changed(QAudio::State state);
	void auto_tx_tick(void);
	void positionUpdated(const QGeoPositionInfo &posInfo);

public slots:
	void transmit_packet(void);

	void updateUserCall(const QString &call);
	void updateUserSSID(uint ssid);
	void updateComment(const QString &comment);
	void updateIconIndex(uint index);
};

#endif // APRSCTRL_H
