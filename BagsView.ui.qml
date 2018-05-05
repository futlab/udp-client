import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import udp_client.backend 1.0

ListView {
    id: view
    property BackEnd backend
    boundsBehavior: Flickable.StopAtBounds
    //property int taskIndex: -1
    clip: true

    model: backend.bags
    delegate: ItemDelegate {
        width: view.width
        contentItem: RowLayout {
            Layout.fillWidth: true
            spacing: 10
            Label {
                text: model ? model.name : ''
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
            Label {
                text: model ? model.sizeString : ''
                font.bold: true
                elide: Text.ElideRight
                width: 80
            }
            SignButton {
                id: removeButton
                text: "×"
                anchors.right: parent.right
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom
            }
        }
        Connections {
            target: removeButton
            onClicked: view.backend.bags.remove(model.name)
        }
    }
}
