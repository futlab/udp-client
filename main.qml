import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2
import udp_client.backend 1.0

ApplicationWindow {
    title: qsTr("UDP launcher client")
    visible: true

    Material.theme: Material.Light
    Material.accent: Material.Teal

    width: 640
    height: 480

    BackEnd {
        id: backend
    }

    MainForm {
        backend: backend
        z: 10
        anchors.fill: parent
    }
    onClosing: {
        backend.saveState();
        close.accepted = true;
    }
}
