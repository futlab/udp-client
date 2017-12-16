import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Window 2.3
import QtQuick.Controls.Material 2.2
import udp_client.backend 1.0

Button {
    id: launchButton
    property int state
    text: "LAUNCH"
    highlighted: state === Task.Active
    states:  [
        State {
            name: "active"
            when: launchButton.state === Task.Active
            PropertyChanges {
                target: launchButton
                highlighted: true
                text: "STOP"
            }
        },
        State {
            name: "wait"
            when: state === Task.Wait
            PropertyChanges {
                target: launchButton
                Material.background: Material.Amber
            }
        },
        State {
            name: "error"
            when: state === Task.Error
            PropertyChanges {
                target: launchButton
                Material.background: Material.Red
            }
        }
    ]
}
