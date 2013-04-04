import QtQuick 2.0
import QtQuick.Controls 1.0

Tab {
    title: qsTr("Parameters")
    Item {
        anchors.fill: parent

        Text {
            id: title1
            text: qsTr("Main parameters")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Item {
            id: fields1
            anchors.top: title1.bottom
            anchors.topMargin: 10
            width: parent.width / 2
            height: childrenRect.height
            Fields { model: [ "m", "z1", "z2", "bw", "ra2" ] }
        }

        Item {
            id: fields2
            anchors.top: title1.bottom
            anchors.topMargin: 10
            width: parent.width / 2
            anchors.left: fields1.right
            height: childrenRect.height
            Fields { model: [ "W0", "E", "x2", "d0", "rf2" ] }
        }

        Text {
            id: title2
            anchors.top: fields1.bottom
            anchors.topMargin: 10

            text: qsTr("Additional parameters")
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Item {
            id: fields3
            anchors.top: title2.bottom
            anchors.topMargin: 10
            width: parent.width / 2
            height: childrenRect.height
            Fields { model: [ "alpha", "c", "ha"] }

        }

        Item {
            id: fields4
            anchors.top: title2.bottom
            anchors.left: fields3.right
            anchors.topMargin: 10
            width: parent.width / 2
            height: childrenRect.height
            Fields { model: [ "z0", "x0", "da0"] }
        }
    }
}
