import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.3

Dialog {
    id: dialog
    width: 200
    height: 100
    visible: true
    title: qsTr("Error!")
    Text {
        anchors.fill: parent
        text: qsTr("There are empty fields! Can not calculate.")
        wrapMode: Text.WordWrap
    }
}
