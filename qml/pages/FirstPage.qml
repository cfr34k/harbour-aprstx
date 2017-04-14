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

	function decodeTXState(state)
	{
		switch(state) {
		case APRSCtrlTypes.Disabled:
			return qsTr("Transmitter disabled");

		case APRSCtrlTypes.Transmitting:
			return qsTr("Transmitting");

		case APRSCtrlTypes.Waiting:
			return qsTr("Waiting for next transmission");

		default:
			return qsTr("Unknown state");
		}
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
				text: decodeTXState(aprsctrl.txstate)
				//color: Theme.secondaryHighlightColor
				anchors.horizontalCenter: parent.horizontalCenter
				//font.pixelSize: Theme.fontSizeLarge
			}

			Label {
				id: latlabel
				font.pixelSize: Theme.fontSizeLarge
				anchors.right: parent.right
				anchors.rightMargin: Theme.paddingLarge
				text: qsTr('Latitude: ') //+ page.formatCoordinate(positionsensor.latitude, false)
			}

			Label {
				id: lonlabel
				font.pixelSize: Theme.fontSizeLarge
				anchors.right: parent.right
				anchors.rightMargin: Theme.paddingLarge
				text: qsTr('Longitude: ') //+ page.formatCoordinate(positionsensor.latitude, false)
			}

			Label {
				id: altlabel
				font.pixelSize: Theme.fontSizeLarge
				anchors.right: parent.right
				anchors.rightMargin: Theme.paddingLarge
				text: qsTr('Altitude: ') //+ page.formatCoordinate(positionsensor.latitude, false)
			}

			SectionHeader {
				text: qsTr("Transmitter Control")
			}

			Button {
				text: qsTr("Transmit now!")
				onClicked: aprsctrl.transmit_packet()
			}

			TextSwitch {
				text: qsTr("Transmitter On")
			}
		}
	}
}
