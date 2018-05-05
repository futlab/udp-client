import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.2
import udp_client.backend 1.0

Pane {
    id: form
    property BackEnd backend
    property var interfaces: backend.interfaces
    property Connection currentConnection: backend.connections[comboBox.currentIndex]
    property int listenPortCtr: 5555
    property bool expanded: false
    leftPadding: 5
    padding: 0
    Material.theme: Material.Dark

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
                width: 250
                textRole: "name"
                model: backend.connections
                currentIndex: backend.connectionIndex
                Material.theme: Material.Dark
            }

            PingButton {
                id: pingButton
                connection: enabled ? backend.connections[comboBox.currentIndex] : null
                enabled: currentConnection
                         && currentConnection.state !== Connection.Error
            }

            Item {
                id: item1
                Layout.fillWidth: true
                Layout.fillHeight: true
                Label {
                    text: "Unable to bind"
                    color: "red"
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    visible: currentConnection
                             && currentConnection.state === Connection.Error
                }
                CheckBox {
                    text: "Autoping"
                    visible: currentConnection
                             && currentConnection.state !== Connection.Error
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
                Layout.alignment: Qt.AlignRight
            }
            SignButton {
                id: deleteButton
                visible: comboBox.currentIndex >= 0
                text: "–"
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom
            }
        }
        /*Frame {
            id: editorFrame
            visible: false
            Layout.fillWidth: true*/
        ConnectionEditor {
            id: editor
            visible: false
            enabled: comboBox.currentIndex >= 0
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
            listenIf: enabled ? backend.connections[comboBox.currentIndex].listenIf : ''
            onListenIfChanged: if (enabled)
                                   backend.connections[comboBox.currentIndex].listenIf = listenIf
        }
        //}
    }

    ConnectionDialog {
        id: newDialog
        visible: false
        backend: form.backend
    }
    Connections {
        target: newButton
        onClicked: {
            var ne = newDialog.editor
            ne.listenPort = listenPortCtr++
            ne.name = ''
            ne.address = ''
            if (ne.listenIf === '')
                ne.listenIf = editor.listenIf
            newDialog.visible = true
        }
    }
    Connections {
        target: expandButton
        onClicked: {
            expanded = !expanded
            if (!expanded)
                backend.saveSettings()
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
                        ne.name, ne.address, ne.port, ne.listenIf,
                        ne.listenPort)
            ne.name = ''
            ne.address = ''
            comboBox.currentIndex = i
        }
    }

    Connections {
        target: comboBox
        onCurrentIndexChanged: {
            form.currentConnection = backend.connections[comboBox.currentIndex]
            backend.connectionIndex = comboBox.currentIndex
        }
    }

    states: [
        State {
            name: "expanded"
            when: form.expanded
            PropertyChanges {
                target: editor
                visible: true
            }
            /*PropertyChanges {
                target: editorFrame
                visible: true
            }*/
            PropertyChanges {
                target: expandButton
                text: "▲"
                highlighted: true
            }
        }
    ]
}
