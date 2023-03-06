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
            width: parent.width
            height: childrenRect.height
            Fields { model: [ "m", "z1", "z2", "bw", "E", "x2", "d0", "W0", "ra2", "rf2" ] }
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
            width: parent.width
            height: childrenRect.height
            Fields { model: [ "alpha", "c", "ha", "z0", "x0", "da0"] }

        }
    }
}
