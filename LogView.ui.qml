import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane {
    property alias logText: list.model
    property bool tracking: true
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
        SignButton {
            id: trackButton
            text: "⤓"
            highlighted: tracking
            anchors.margins: 10
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
        Connections {
            target: trackButton
            onClicked: {
                tracking = !tracking;
                if (tracking)
                    list.positionViewAtEnd()
            }
        }
        Connections {
            target: list.model
            onCountChanged: if (tracking)
                                list.positionViewAtEnd()
        }
        Connections {
            target: list
            onFlickEnded: tracking = false
        }
    }
}
