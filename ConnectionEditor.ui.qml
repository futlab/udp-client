import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import udp_client.backend 1.0

GridLayout {
    id: editor
    property alias listenIf: listenIfBox.text
    property alias listenPort: listenPortField.value
    property alias name: nameField.text
    property alias port: portField.value
    property alias address: addressField.text
    Layout.fillWidth: true
    property bool enabled: true
    property bool showName: false
    rows: showName ? 5 : 4
    columns: 2

    HeaderLabel {
        visible: editor.showName
        text: qsTr("Name")
    }
    TextField {
        id: nameField
        horizontalAlignment: Text.AlignHCenter
        visible: editor.showName
        enabled: editor.enabled
        Layout.minimumWidth: 180
        Layout.fillWidth: true
    }

    HeaderLabel {
        text: qsTr("Address")
    }
    TextField {
        id: addressField
        horizontalAlignment: Text.AlignHCenter
        enabled: editor.enabled
        Layout.minimumWidth: 180
        Layout.fillWidth: true
    }

    HeaderLabel {
        text: qsTr("Port")
    }
    SpinBox {
        id: portField
        value: 5007
        editable: true
        enabled: editor.enabled
        Layout.minimumWidth: 180
        Layout.fillWidth: true
        to: 65535
    }
    HeaderLabel {
        text: qsTr("Listen IF")
    }
    IfComboBox {
        id: listenIfBox
        enabled: editor.enabled
        Layout.minimumWidth: 180
        Layout.fillWidth: true
    }
    HeaderLabel {
        text: qsTr("Listen port")
    }
    SpinBox {
        id: listenPortField
        to: 65535
        enabled: editor.enabled
        Layout.minimumWidth: 180
        Layout.fillWidth: true
    }
}
