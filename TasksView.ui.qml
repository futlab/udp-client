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
                sureDialog.visible = true
                view.taskIndex = index
            }
        }
        Connections {
            target: delegate
            onExpand: if (delegate.expanded) view.taskIndex = -1;
                      else view.taskIndex = index
        }
    }
    SignButton {
        text: "+"
        anchors.margins: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: newDialog.visible = true
    }
    SignButton {
        id: saveButton
        text: "s"
        anchors.rightMargin: 70
        anchors.margins: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
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
        onYes: tasksListModel.remove(taskIndex)
    }

    Connections {
        target: newDialog
        onAccepted: {
            var e = newDialog.editor
            backend.appendTask(e.name, e.useRos, e.command, e.launchFile)
        }
    }

    Text {
        id: text1
        x: -130
        y: 41
        text: qsTr("Text")
        font.pixelSize: 12
    }

    Connections {
        target: saveButton
        onClicked: backend.saveState()
    }
}
