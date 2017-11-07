import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import udp_client.backend 1.0

Pane {
    id: connectionForm
    property BackEnd backend
    property var interfaces: backend.interfaces
    property Connection currentConnection: backend.connections[comboBox.currentIndex];
    property int listenPortCtr: 5555
    //width: 346
    property bool expanded: false

    ColumnLayout {
        id: column
        anchors.fill: parent
        //Layout.fillWidth: true
        spacing: 5

        RowLayout {
            id: row
            Layout.fillWidth: true
            width: column.width
            spacing: 5

            ComboBox {
                id: comboBox
                width: 200
                textRole: "name"
                model: backend.connections
                currentIndex: backend.connectionIndex
            }

            PingButton {
                id: pingButton
                connection: backend.connections[comboBox.currentIndex]
                enabled: comboBox.currentIndex >= 0
            }

            Item {
                id: item1
                Layout.fillWidth: true
                Layout.fillHeight: true
                Label {
                    text: "Unable to bind"
                    //Layout.fillHeight: true
                    color: "red"
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter//currentConnection.errorMessage
                    visible: currentConnection && currentConnection.state === Connection.Error
                }
                CheckBox {
                    text: "Autoping"
                    visible: currentConnection && currentConnection.state !== Connection.Error
                }
            }

            SignButton {
                id: newButton
                text: "+"
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom
            }
            SignButton {
                id: expandButton
                text: "▼"
                highlighted: true
                Layout.alignment: Qt.AlignRight
                onClicked: expanded = !expanded
            }
            SignButton {
                id: deleteButton
                visible: comboBox.currentIndex >= 0
                text: "–"
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom
            }
        }

        ConnectionEditor {
            id: editor
            visible: false
            enabled: comboBox.currentIndex >= 0
            interfaces: connectionForm.interfaces
            address: enabled ? backend.connections[comboBox.currentIndex].address : ''
            onAddressChanged: if (enabled)
                                  backend.connections[comboBox.currentIndex].address = address
            port: enabled ? backend.connections[comboBox.currentIndex].port : ''
            onPortChanged: if (enabled)
                               backend.connections[comboBox.currentIndex].port = port
            listenPort: enabled ? backend.connections[comboBox.currentIndex].listenPort : ''
            onListenPortChanged: if (enabled)
                                     backend.connections[comboBox.currentIndex].listenPort
                                             = listenPort
            onListenIfIdxChanged: if (enabled)
                                      backend.connections[comboBox.currentIndex].listenIf
                                              = interfaces[listenIfIdx]
        }
    }

    ConnectionDialog {
        id: newDialog
        visible: false
    }
    Connections {
        target: newButton
        onClicked: {
            var ne = newDialog.editor
            ne.listenPort = listenPortCtr++
            newDialog.visible = true
        }
    }

    Connections {
        target: deleteButton
        onClicked: backend.removeConnection(comboBox.currentIndex)
    }

    Connections {
        target: newDialog
        onAccepted: {
            var ne = newDialog.editor, i = backend.appendConnection(
                        ne.name, ne.address, ne.port,
                        interfaces[ne.listenIfIdx], ne.listenPort)
            ne.name = ''
            ne.address = ''
            ne.port = ''
            comboBox.currentIndex = i
        }
    }

    Connections {
        target: comboBox
        onCurrentIndexChanged: {
            connectionForm.currentConnection = backend.connections[comboBox.currentIndex];
            backend.connectionIndex = comboBox.currentIndex;
        }
    }

    states: [
        State {
            name: "expanded"
            when: connectionForm.expanded
            PropertyChanges {
                target: editor
                visible: true
            }
            PropertyChanges {
                target: expandButton
                text: "▲"
            }
        }
    ]
}
