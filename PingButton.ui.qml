import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.2
import udp_client.backend 1.0

Button {
    id: pingButton
    property Connection connection
    text: qsTr("PING")

    Connections {
        target: pingButton
        onClicked: connection.ping();
    }

    states: [
        State {
            name: "ok"
            when: connection && connection.state === Connection.Pinged
            PropertyChanges {
                target: pingButton
                highlighted: true
            }
        },
        State {
            name: "fail"
            when: connection && connection.state === Connection.Wait
            PropertyChanges {
                target: pingButton
                Material.background: Material.Red
            }
        },
        State {
            name: "error"
            when: !connection || connection.state === Connection.Error
            PropertyChanges {
                target: pingButton
                enabled: false
                Material.background: Material.Black
            }
        }
    ]
}


