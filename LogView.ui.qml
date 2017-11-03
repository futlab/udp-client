import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane {
    property alias logText: logText.text
    padding: 5
    Layout.fillWidth: true
    Layout.fillHeight: true
    Flickable {
        id: flick
        clip: true
        anchors.fill: parent
        contentWidth: logText.paintedWidth
        contentHeight: logText.paintedHeight
        Text {
            id: logText
            z: 5
            clip: false
            font.family: "Courier"
            textFormat: Text.PlainText
            elide: Text.ElideLeft
        }
    }
}
