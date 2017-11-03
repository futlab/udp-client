import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import udp_client.backend 1.0

Pane {
    id: form
    property BackEnd backend
    property alias currentConnection: connectionForm.currentConnection

    ColumnLayout {
        id: column
        anchors.fill: parent
        spacing: 10

        GroupBox {
            id: connectGroupBox
            padding: 5
            Layout.fillWidth: true
            title: qsTr("Connection")

            ConnectionForm {
                id: connectionForm
                anchors.fill: parent
                backend: form.backend
            }
        }

        GroupBox {
            id: tasksBox
            padding: 5
            Layout.fillHeight: true
            Layout.fillWidth: true
            title: qsTr("Tasks")

            TasksView {
                anchors.fill: parent
                id: tasksView
                connection: currentConnection
                backend: form.backend
            }
        }
    }
}
