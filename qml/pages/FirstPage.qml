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
            return qsTr("Next transmission forced in ") + secondsToTx + qsTr(" s");

        case APRSCtrlTypes.WaitingTriggered:
            return qsTr("Transmission delayed for ") + secondsToTx + qsTr(" s");

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
				text: qsTr("About")
				onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
			}
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

            Label {
                id: distanceLabel
                font.pixelSize: Theme.fontSizeLarge
                anchors.right: parent.right
                anchors.rightMargin: Theme.paddingLarge
                text: qsTr('Distance: ') + '<b>' + aprsctrl.distance.toFixed(1) + ' m</b>'
            }

            Label {
                id: headingChangeLabel
				font.pixelSize: Theme.fontSizeLarge
				anchors.right: parent.right
				anchors.rightMargin: Theme.paddingLarge
                text: qsTr('Heading change: ') + '<b>' + aprsctrl.headingChange.toFixed(1) + ' °</b>'
			}

			SectionHeader {
				text: qsTr("Transmitter Control")
			}

			Button {
				text: qsTr("Transmit now!")
				onClicked: aprsctrl.transmit_packet()
				anchors.horizontalCenter: parent.horizontalCenter
				enabled: settings.userCall.length > 0 && aprsctrl.txstate != APRSCtrlTypes.Transmitting
			}

			TextSwitch {
				text: qsTr("Enable automatic transmitter")
				enabled: settings.userCall.length > 0
				onCheckedChanged: { if(checked) { aprsctrl.auto_tx_start() } else { aprsctrl.auto_tx_stop() } }
			}
		}
	}
}
