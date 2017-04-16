/*
	Copyright (C) 2013 Jolla Ltd.
	Contact: Thomas Perl <thomas.perl@jollamobile.com>
	All rights reserved.

	You may use this file under the terms of BSD license as follows:

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
	* Neither the name of the Jolla Ltd nor the
		names of its contributors may be used to endorse or promote products
		derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

import APRSTypes 1.0

Page {
	id: page

	function decodeTXState(state, secondsToTx)
	{
		switch(state) {
		case APRSCtrlTypes.Disabled:
			return qsTr("Transmitter disabled");

		case APRSCtrlTypes.Transmitting:
			return qsTr("Transmitting");

		case APRSCtrlTypes.Waiting:
			return qsTr("Waiting for next transmission (" + secondsToTx + " s)");

		default:
			return qsTr("Unknown state");
		}
	}

	function colorFromTxState(state)
	{
		switch(state) {
		case APRSCtrlTypes.Transmitting:
			return "red";
		default:
			return Theme.highlightColor;
		}
	}

	function formatCoordinate(n, isLon) {
		var coordText = "";

		var absn = Math.abs(n);
		var deg = Math.floor(absn);
		var min = ((Math.floor(absn * 6000) % 6000) / 100).toFixed(2);

		coordText  = deg + "° ";
		coordText += min + "' ";

		if(isLon) { // longitude: east/west
			if(n >= 0) {
				coordText += "E";
			} else {
				coordText += "W";
			}
		} else { // latitude: north/south
			if(n >= 0) {
				coordText += "N";
			} else {
				coordText += "S";
			}
		}

		return '<b>' + coordText + '</b>';
	}

	// The effective value will be restricted by ApplicationWindow.allowedOrientations
	allowedOrientations: Orientation.All

	// To enable PullDownMenu, place our content in a SilicaFlickable
	SilicaFlickable {
		anchors.fill: parent

		// PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
		PullDownMenu {
			MenuItem {
				text: qsTr("Settings")
				onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
			}
		}

		// Tell SilicaFlickable the height of its content.
		contentHeight: column.height

		// Place our content in a Column.  The PageHeader is always placed at the top
		// of the page, followed by our content.
		Column {
			id: column

			width: page.width
			spacing: Theme.paddingLarge

			PageHeader {
				title: qsTr("APRS Transmitter")
			}

			SectionHeader {
				text: qsTr("Transmitter Status")
			}

			Label {
				text: decodeTXState(aprsctrl.txstate, aprsctrl.secondsToAutoTx)
				color: colorFromTxState(aprsctrl.txstate)
				//color: Theme.secondaryHighlightColor
				anchors.horizontalCenter: parent.horizontalCenter
				//font.pixelSize: Theme.fontSizeLarge
			}

			Label {
				id: latlabel
				font.pixelSize: Theme.fontSizeLarge
				anchors.right: parent.right
				anchors.rightMargin: Theme.paddingLarge
				text: qsTr('Latitude: ') + page.formatCoordinate(aprsctrl.latitude, false)
			}

			Label {
				id: lonlabel
				font.pixelSize: Theme.fontSizeLarge
				anchors.right: parent.right
				anchors.rightMargin: Theme.paddingLarge
				text: qsTr('Longitude: ') + page.formatCoordinate(aprsctrl.longitude, true)
			}

			Label {
				id: altlabel
				font.pixelSize: Theme.fontSizeLarge
				anchors.right: parent.right
				anchors.rightMargin: Theme.paddingLarge
				text: qsTr('Altitude: ') + '<b>' + aprsctrl.altitude + ' m</b>'
			}

			SectionHeader {
				text: qsTr("Transmitter Control")
			}

			Button {
				text: qsTr("Transmit now!")
				onClicked: aprsctrl.transmit_packet()
				anchors.horizontalCenter: parent.horizontalCenter
				enabled: aprsctrl.txstate != APRSCtrlTypes.Transmitting
			}

			TextSwitch {
				text: qsTr("Enable automatic transmitter")
				onCheckedChanged: { if(checked) { aprsctrl.auto_tx_start() } else { aprsctrl.auto_tx_stop() } }
			}
		}
	}
}
