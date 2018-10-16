import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

// custom qml import
import SGNode 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5

        Button {
            id: buttonId
            text: qsTr("!! Sketch wave !!")
            height: 50
            Layout.fillWidth: true
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            onClicked: sgnodeId.buttonClicked()
        }

        Rectangle {
            color: "light grey"
            Layout.fillWidth: true
            Layout.fillHeight: true

            SGNode {
                id: sgnodeId
                anchors.fill: parent
            }
        }
    }
}
