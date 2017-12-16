import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import udp_client.backend 1.0

GridLayout {
    id: editor
    property alias name: nameField.text
    property alias useRos: useRosSwitch.checked
    property alias command: commandField.text
    property alias launchFile: launchFileBox.text
    //property string launchFile
    property var launchFiles: connection ? connection.launchFiles : undefined
    property Connection connection
    rows: 4
    columns: 2
    Layout.fillWidth: true
    HeaderLabel {
        text: 'Name'
    }
    TextField {
        id: nameField
        Layout.minimumWidth: 180
        Layout.fillWidth: true
    }

    HeaderLabel {
        text: 'Use roslaunch'
    }
    Switch {
        id: useRosSwitch
        checked: false
        position: 0
    }

    HeaderLabel {
        text: 'ROS launch file'
        enabled: useRos
    }
    EditableComboBox {
        id: launchFileBox
        enabled: useRos
        Layout.minimumWidth: 180
        Layout.fillWidth: true
        model: connection ? connection.launchFiles : []
    }

    HeaderLabel {
        text: 'Command'
        enabled: !useRos
    }
    TextField {
        id: commandField
        enabled: !useRos
        Layout.minimumWidth: 180
        Layout.fillWidth: true
    }

    Connections {
        target: editor
        onUseRosChanged: if (useRos) {
                             if (connection)
                                 connection.queryList()
                         }
    }

    Connections {
        target: launchFileBox
        onDownChanged: if (useRos)
                           connection.queryList()
    }
}
