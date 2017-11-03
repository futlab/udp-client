import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import udp_client.backend 1.0

GridLayout {
    id: editor
    //width: 400
    //height: 400
    property alias listenIfIdx: listenIfBox.currentIndex
    property var interfaces
    property alias listenPort: listenPortField.text
    property alias name: nameField.text
    property alias port: portField.text
    property alias address: addressField.text
    Layout.fillWidth: true
    property bool enabled: true
    property bool showName: false
    //spacing: 5
    rows: showName ? 5 : 4
    columns: 2

    HeaderLabel {
        visible: editor.showName
        text: qsTr("Name")
    }
    TextField {
        id: nameField
        visible: editor.showName
        enabled: editor.enabled
        Layout.minimumWidth: 180
        Layout.fillWidth: true
    }

    HeaderLabel { text: qsTr("Address") }
    TextField {
        id: addressField
        enabled: editor.enabled
        Layout.minimumWidth: 180
        Layout.fillWidth: true
    }

    HeaderLabel { text: qsTr("Port") }
    TextField {
        id: portField
        //to: 65535
        enabled: editor.enabled
        Layout.minimumWidth: 180
        Layout.fillWidth: true
    }
    HeaderLabel { text: qsTr("Listen IF") }
    ComboBox {
        id: listenIfBox
        //to: 65535
        model: interfaces
        enabled: editor.enabled
        Layout.minimumWidth: 180
        Layout.fillWidth: true
    }
    HeaderLabel { text: qsTr("Listen port") }
    TextField {
        id: listenPortField
        //to: 65535
        enabled: editor.enabled
        Layout.minimumWidth: 180
        Layout.fillWidth: true
    }
}
