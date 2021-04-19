import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    id: dialog
    width: 200
    height: 100
    standardButtons: Dialog.Close
    visible: true
    title: qsTr("Error!")
    Text {
        anchors.fill: parent
        text: qsTr("There are empty fields! Can not calculate.")
        wrapMode: Text.WordWrap
    }
}
