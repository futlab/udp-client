import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import udp_client.backend 1.0

Pane {
    id: form
    property BackEnd backend
    property alias currentConnection: connectionForm.currentConnection
    padding: 0
    ColumnLayout {
        id: column
        anchors.fill: parent
        spacing: 10

        /*GroupBox {
            id: connectGroupBox
            padding: 5
            Layout.fillWidth: true
            title: qsTr("Connection")*/
        ConnectionForm {
            id: connectionForm
            Layout.fillWidth: true
            //anchors.fill: parent
            backend: form.backend
        }

        RowLayout {
            id: barRow
            width: column.width
            Layout.fillWidth: true
            //Layout.fillWidth: false
            //Layout.fillWidth: true
            spacing: 0
            TabBar {
                id: bar
                visible: true
                currentIndex: 0
                TabButton {
                    id: tabButton
                    width: 200
                    text: "TASKS"
                }
                TabButton {
                    id: tabButton1
                    width: 200
                    text: "BAGS " + backend.bags.count + "/" + backend.bags.totalSize
                    SignButton {
                        id: removeBagsButton
                        text: "×"
                        anchors.right: parent.right
                        Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                    }
                }
            }
            Label {
                text: "build: " + backend.buildInfo
                padding: 10
                horizontalAlignment: Text.AlignRight
                transformOrigin: Item.Center
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                anchors.right: parent.right
                Layout.fillWidth: true
            }
        }
        StackLayout {
            id: content
            visible: true
            Layout.fillWidth: true
            currentIndex: bar.currentIndex
            TasksView {
                anchors.fill: parent
                id: tasksView
                connection: currentConnection
                backend: form.backend
            }
            BagsView {
                anchors.fill: parent
                id: bagsView
                backend: form.backend
            }
        }
    }

    Connections {
        target: removeBagsButton
        onClicked: backend.bags.removeAll()
    }
}
