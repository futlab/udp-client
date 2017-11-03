import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.2
import udp_client.backend 1.0

Button {
    id: pingButton
    property Connection connection
    onConnectionChanged: pingButton.state = ""
    text: qsTr("PING")

    Connections {
        target: pingButton
        onClicked: {
            pingButton.state = "fail"
            connection.ping();
        }
    }
    Connections {
        target: connection
        onPingOk: pingButton.state = "ok"
    }

    states: [
        State {
            name: "ok"
            PropertyChanges {
                target: pingButton
                //Material.background: Material.Green
                highlighted: true
            }

        },
        State {
            name: "fail"
            PropertyChanges {
                target: pingButton
                Material.background: Material.Red
            }
        }
    ]
}


