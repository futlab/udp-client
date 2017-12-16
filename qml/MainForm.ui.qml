import QtQuick 2.8
//import QtQml 2.2
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
        TabBar {
            id: bar
            TabButton {
                text: qsTr("TASKS")
                width: 80
            }
            TabButton {
                text: qsTr("SCANS")
                width: 80
            }
        }

        StackLayout {
            id: tasksBox
            //padding: 5
            Layout.fillHeight: true
            Layout.fillWidth: true
            currentIndex: bar.currentIndex

            //title: qsTr("Tasks")
            TasksView {
                anchors.fill: parent
                id: tasksView
                connection: currentConnection
                backend: form.backend
            }
            Loader {
                id: loader
                sourceComponent: empty
            }

            /*ScanView {
                //anchors.fill: parent

            }*/
        }
    }

    Connections {
        target: tasksBox
        onCurrentIndexChanged: {
            if (this.target.currentIndex === 1)
                loader.sourceComponent = scanView
        }
    }
    Component {
        id: empty
        Item {

        }
    }
    Component {
        id: scanView
        ScanView {

        }
    }
}
