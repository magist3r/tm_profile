import QtQuick 1.1
import QtDesktop 0.1

Dialog {
    id: dialog
    width: 200
    height: 100
    buttons: close
    visible: true
    title: qsTr("Error!")
    Text {
        anchors.fill: parent
        text: qsTr("There are empty fields! Can not calculate.")
        wrapMode: Text.WordWrap
    }
}
