import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane {
    property alias logText: list.model
    padding: 5
    Layout.fillWidth: true
    Layout.fillHeight: true

    ListView {
        id: list
        clip: true
        anchors.fill: parent
        //model: ['tex1', 'text2']
        //contentWidth: logText.paintedWidth
        //contentHeight: logText.paintedHeight
        delegate: Text {
            id: delegate
            text: line
            z: 5
            clip: false
            font.family: "Courier"
            textFormat: Text.PlainText
            elide: Text.ElideLeft
        }
    }
}
