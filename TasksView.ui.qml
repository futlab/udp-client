import QtQuick 2.7
import QtQuick.Controls 2.1
import udp_client.backend 1.0

ListView {
    id: view
    property Connection connection
    property BackEnd backend
    //signal onDeleteTask(int index)
    boundsBehavior: Flickable.StopAtBounds
    property int taskIndex: -1
    clip: true

    model: backend.tasks

    delegate: TaskDelegate {
        id: delegate
        z: 1
        width: view.width
        connection: view.connection
        expanded: index === taskIndex

        Connections {
            target: delegate
            onRemoveTask: {
                sureDialog.data = index
                sureDialog.visible = true
            }
            onNewTask: newDialog.visible = true
            onExpand: if (delegate.expanded)
                          view.taskIndex = -1
                      else
                          view.taskIndex = index
        }
    }

    TaskDialog {
        id: newDialog
        visible: false
        connection: view.connection
        property string name: ''
        property string command: ''
    }
    SureDialog {
        id: sureDialog
        visible: false
    }
    Connections {
        target: sureDialog
        onYes: backend.removeTask(sureDialog.data)
    }

    Connections {
        target: newDialog
        onAccepted: {
            var e = newDialog.editor
            backend.appendTask(e.name, e.useRos, e.command, e.launchFile)
        }
    }
    Connections {
        target: view
        onTaskIndexChanged: backend.saveSettings()
    }
}
