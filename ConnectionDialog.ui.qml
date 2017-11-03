import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import udp_client.backend 1.0

Dialog {
    id: newDialog
    property alias editor: editor
    title: "Add new connection"
    visible: false
    standardButtons: StandardButton.Save | StandardButton.Cancel
    ConnectionEditor {
        anchors.fill: parent
        id: editor
        showName: true
        interfaces: backend.interfaces
    }
}
