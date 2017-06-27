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

Page {
    id: page

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge

            PageHeader {
				title: qsTr("About APRSTX")
            }
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
				text: qsTr("Author: Thomas Kolb")
                color: Theme.highlightColor
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
				text: qsTr("Visit Project on Github")
				onClicked: Qt.openUrlExternally("https://github.com/cfr34k/harbour-aprstx")
            }
			Label {
				anchors.horizontalCenter: parent.horizontalCenter
				width: parent.width - Theme.paddingMedium
				text:"
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>."
				color: Theme.highlightColor
				font.pixelSize: Theme.fontSizeTiny
				wrapMode: Text.WordWrap
			}
		}
    }
}
