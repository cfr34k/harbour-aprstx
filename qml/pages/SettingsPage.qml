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


Dialog {
	id: dialog

	// The effective value will be restricted by ApplicationWindow.allowedOrientations
	allowedOrientations: Orientation.All

	onAccepted: {
		settings.userCall = userCall.text;
		settings.userSSID = userSSID.text;
		settings.comment = comment.text;
		settings.autoTxInterval = autoTxInterval.text;
		settings.voxToneDuration = voxTone.text;
		settings.tailFlags = tailFlags.text;
		settings.iconIndex = icon.currentIndex
	}

	SilicaFlickable {
		anchors.fill: parent
		contentHeight: column.height

		Column {
			id: column

			width: dialog.width
			spacing: Theme.paddingLarge

			DialogHeader {
				acceptText: qsTr("Save")
			}

			SectionHeader {
				text: qsTr("APRS Fields")
			}

			TextField {
				id: userCall
				width: parent.width
				text: settings.userCall
				label: qsTr("Your call sign")
				placeholderText: label
			}

			TextField {
				id: userSSID
				width: parent.width
				text: settings.userSSID
				label: qsTr("Source SSID (0-15, incl.)")
				inputMethodHints: Qt.ImhDigitsOnly
				placeholderText: label
				validator: IntValidator {bottom: 0; top: 15}
			}

			ComboBox {
				id: icon
				currentIndex: settings.iconIndex
				label: qsTr("Icon")
				menu: ContextMenu {
					/* Keep this list in sync with aprs.h! */
					MenuItem { text: qsTr("X") }
					MenuItem { text: qsTr("Bicycle") }
					MenuItem { text: qsTr("Car") }
					MenuItem { text: qsTr("Jeep") }
					MenuItem { text: qsTr("Van") }
					MenuItem { text: qsTr("Truck") }
					MenuItem { text: qsTr("Bus") }
					MenuItem { text: qsTr("Balloon") }
					MenuItem { text: qsTr("Recreational Vehicle") }
					MenuItem { text: qsTr("Helicopter") }
					MenuItem { text: qsTr("Yacht") }
					MenuItem { text: qsTr("Ambulance") }
					MenuItem { text: qsTr("Fire Truck") }
					MenuItem { text: qsTr("Ship") }
				}
			}

			TextField {
				id: comment
				width: parent.width
				text: settings.comment
				label: qsTr("APRS comment (max. 32 characters)")
				placeholderText: label
			}

			SectionHeader {
				text: qsTr("Operation")
			}

			TextField {
				id: autoTxInterval
				width: parent.width
				text: settings.autoTxInterval
				label: qsTr("Auto TX interval (seconds)")
				inputMethodHints: Qt.ImhDigitsOnly
				placeholderText: label
				validator: IntValidator {bottom: 1}
			}

			SectionHeader {
				text: qsTr("Modulation")
			}

			TextField {
				id: voxTone
				width: parent.width
				text: settings.voxToneDuration
				label: qsTr("VOX tone duration (milliseconds)")
				inputMethodHints: Qt.ImhDigitsOnly
				placeholderText: label
				validator: IntValidator {bottom: 0}
			}

			TextField {
				id: tailFlags
				width: parent.width
				text: settings.tailFlags
				label: qsTr("Tail flags (after end of packet)")
				inputMethodHints: Qt.ImhDigitsOnly
				placeholderText: label
				validator: IntValidator {bottom: 0}
			}
		}
	}
}
