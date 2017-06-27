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


Dialog {
	id: dialog

	// The effective value will be restricted by ApplicationWindow.allowedOrientations
	allowedOrientations: Orientation.All

	onAccepted: {
		settings.userCall = userCall.text;
		settings.userSSID = userSSID.text;
		settings.comment = comment.text;
        settings.autoTxIntervalMin = autoTxIntervalMin.text;
        settings.autoTxIntervalForced = autoTxIntervalForced.text;
        settings.autoTxDistance = autoTxDistance.text;
        settings.autoTxHeadingChange = autoTxHeadingChange.text;
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
					MenuItem { text: qsTr("Jogger") }
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
                id: autoTxIntervalMin
				width: parent.width
                text: settings.autoTxIntervalMin
                label: qsTr("Auto TX: min. interval (seconds)")
				inputMethodHints: Qt.ImhDigitsOnly
				placeholderText: label
				validator: IntValidator {bottom: 1}
			}

            TextField {
                id: autoTxIntervalForced
                width: parent.width
                text: settings.autoTxIntervalForced
                label: qsTr("Auto TX: forced interval (seconds)")
                inputMethodHints: Qt.ImhDigitsOnly
                placeholderText: label
                validator: IntValidator {bottom: 1}
            }

            TextField {
                id: autoTxDistance
                width: parent.width
                text: settings.autoTxDistance
                label: qsTr("Auto TX: distance (meters)")
                inputMethodHints: Qt.ImhDigitsOnly
                placeholderText: label
                validator: IntValidator {bottom: 1}
            }

            TextField {
                id: autoTxHeadingChange
                width: parent.width
                text: settings.autoTxHeadingChange
                label: qsTr("Auto TX: heading change (±180 degrees)")
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
