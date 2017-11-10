import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import udp_client.backend 1.0

ComboBox {
    id: box
    editable: true
    property alias text: textField.text
    onActivated: launchFile = box.currentText
    contentItem: TextField {
        id: textField
        //text: "any text"
    }
}
