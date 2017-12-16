import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import udp_client.backend 1.0

ComboBox {
    id: box
    property alias text: textField.text
    onActivated: textField.text = box.currentText
    editable: false
    function exists(model, val) {
        var re = /^([\d.]+):/;
        var a = re.exec(val);
        if (!a) return false;
        a = a[1];
        for (var x in model) {
            var aa = re.exec(model[x]);
            if (aa && aa[1] === a) return true;
        }
        return false;
    }

    contentItem: Text {
        id: textField
        anchors.fill: parent
        font: box.font
        color: exists(model, text) ? "black" : "red"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    model: ifs.list
    Interfaces {
        id: ifs
    }
}
