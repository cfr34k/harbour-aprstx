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
                text: qsTr("Project on Github")
                onClicked: Qt.openUrlExternally("https://github.com/cfr34k/harbour-aprstx")
            }
        }
    }
}
