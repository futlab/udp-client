import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import udp_client.backend 1.0

Dialog {
    id: newDialog
    property alias editor: editor
    title: "Add new connection"
    visible: false
    standardButtons: Dialog.NoButton // StandardButton.Save | StandardButton.Cancel
    Connection {
        id: connection
        address: editor.address
        port: editor.port
        listenPort: editor.listenPort
        listenIf: editor.listenIf
    }

    ColumnLayout {
        anchors.fill: parent
        ConnectionEditor {
            Layout.fillWidth: true
            Layout.fillHeight: true
            //anchors.fill: parent
            id: editor
            showName: true
        }
        Row {
            spacing: 20
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Button {
                id: scanButton
                text: "AUTOSCAN"
            }
            PingButton {
                text: "PING"
                connection: connection
            }
            Button {
                id: createButton
                text: "CREATE"
            }
            Button {
                id: cancelButton
                text: "CANCEL"
            }
        }
    }

    Connections {
        target: cancelButton
        onClicked: {
            connection.unbind()
            newDialog.close()
        }
    }

    Connections {
        target: createButton
        onClicked: {
            connection.unbind()
            newDialog.accept()
        }
    }

    Connections {
        target: scanButton
        onClicked: connection.scan()
    }

    Connections {
        target: connection
        onPingOk: {
            if (connection.state === Connection.Active) {
                editor.name = host
                editor.address = remoteIf
            } else if (editor.name === "")
                editor.name = host
        }
    }
}
