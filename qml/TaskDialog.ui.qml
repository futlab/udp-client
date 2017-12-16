import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import udp_client.backend 1.0

Dialog {
    id: dialog
    property Connection connection
    property alias editor: taskEditor
    visible: false
    title: 'Define new task'
    standardButtons: StandardButton.Save | StandardButton.Cancel
    TaskEditor {
        //Layout.fillWidth: true
        anchors.fill: parent
        id: taskEditor
        connection: dialog.connection
    }
}
