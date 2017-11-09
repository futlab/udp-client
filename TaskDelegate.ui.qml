import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Window 2.3
import udp_client.backend 1.0

ItemDelegate {
    id: delegate
    checkable: true
    property bool expanded: false
    property Connection connection
    signal removeTask
    signal newTask
    signal expand
    ListModel {
        id: log
    }

    contentItem: ColumnLayout {
        spacing: 10
        RowLayout {
            spacing: 10
            Label {
                id: label
                text: modelData ? modelData.name : ''
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
            TabBar {
                id: bar
                visible: false
                currentIndex: 1
                TabButton {
                    id: tabButton
                    width: 100
                    text: "LOG"
                    onClicked: delegate.checked = true
                }
                TabButton {
                    width: 100
                    onClicked: delegate.checked = true
                    text: "SETUP"
                }
            }
            LaunchButton {
                id: launchButton
                state: modelData ? modelData.state : Task.Stop
            }
            SignButton {
                id: expandButton
                text: "▼"
                highlighted: false
                Layout.alignment: Qt.AlignRight
            }
            SignButton {
                id: removeButton
                text: '–'
                highlighted: false
            }
        }
        StackLayout {
            id: content
            visible: false
            Layout.fillWidth: true
            currentIndex: bar.currentIndex
            Item {
                id: logTab
                Layout.fillWidth: true
                implicitHeight: paramTab.implicitHeight
                LogView {
                    logText: log
                    width: parent.width
                    height: parent.height
                }
            }
            ColumnLayout {
                id: paramTab
                Layout.fillWidth: true
                TaskEditor {
                    id: taskEditor
                    connection: delegate.connection
                    name: modelData ? modelData.name : ''
                    onNameChanged: modelData.name = name
                    useRos: modelData ? modelData.useRos : false
                    onUseRosChanged: modelData.useRos = useRos
                    command: modelData ? modelData.command : ''
                    onCommandChanged: modelData.command = command
                    launchFile: modelData ? modelData.launchFile : ''
                    onLaunchFileChanged: modelData.launchFile = launchFile
                }
            }
        }
    }

    Connections {
        target: removeButton
        onClicked: if (modelData)
                       removeTask()
                   else
                       newTask()
    }

    Connections {
        target: launchButton
        onClicked: if (modelData.state === Task.Active)
                       connection.stop(modelData)
                   else {
                       log.clear()
                       connection.launch(modelData)
                   }
    }

    Connections {
        target: expandButton
        onClicked: expand()
    }

    Connections {
        target: modelData
        onLog: log.append({
                              line: line
                          })
    }

    Connections {
        target: modelData
        onStateChanged: if (state === Task.Active)
                            log.clear()
    }

    states: [
        State {
            name: "expanded"
            when: expanded
            PropertyChanges {
                target: content
                visible: true
            }
            PropertyChanges {
                target: bar
                visible: true
            }
            PropertyChanges {
                target: label
                font.pointSize: 10
            }
            PropertyChanges {
                target: expandButton
                text: "▲"
                highlighted: true
            }
        },
        State {
            name: "new"
            when: !modelData
            PropertyChanges {
                target: expandButton
                visible: false
            }
            PropertyChanges {
                target: launchButton
                visible: false
            }
            PropertyChanges {
                target: removeButton
                text: "+"
            }
        }
    ]
}
