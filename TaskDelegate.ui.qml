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

    contentItem: ColumnLayout {
        spacing: 10
        RowLayout {
            spacing: 10
            Label {
                id: label
                text: name
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
            TabBar {
                id: bar
                currentIndex: 1
                visible: delegate.expanded
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
                state: modelData.state
            }
            SignButton {
                id: expandButton
                text: "▼"
                highlighted: false
                Layout.alignment: Qt.AlignRight
                onClicked: delegate.expanded = !delegate.expanded
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
                    name: model.name
                    useRos: model.useRos
                    onUseRosChanged: modelData.useRos = useRos
                    command: useRos ? "roslaunch " + model.command : model.command
                    onCommandChanged: if (!useRos) modelData.command = command
                    launchFile: useRos ? model.command : ""
                    onLaunchFileChanged: if (useRos) modelData.command = launchFile
                }
            }
        }
    }

    Connections {
        target: removeButton
        onClicked: removeTask()
    }

    Connections {
        target: launchButton
        onClicked: connection.launch(modelData)
    }
    states: [
        State {
            name: "expanded"
            when: delegate.expanded
            PropertyChanges {
                target: content
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
        }
    ]
}
