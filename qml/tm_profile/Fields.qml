import QtQuick 2.0
import QtQuick.Controls 1.0

Repeater {
    id: repeater

    Item {
        id: fielditem
        y: index * (height + 10)
        height: label.height
        anchors {
            left: parent.left
            right: parent.right
            margins: 10
        }

        Label {
            id: label
            width: 40
            text: [modelData].valueOf(index) + "= "
            font.pointSize: 10
        }

        SpinBox {
            id: item
            property var ints: ["m", "z1", "z2", "E", "bw", "d0", "alpha", "z0"]
            anchors.verticalCenter: label.verticalCenter
            anchors.left: label.right
            anchors.right: parent.right
            decimals: {
                if (ints.indexOf(modelData) != -1)
                    return 0
                else {
                    item.stepSize = 0.1
                    return 3
                }
            }
            onValueChanged: {
                parametersChanged = true
                profile[modelData] = item.value
            }

            Binding {
                target: item
                property: "value"
                value: profile[modelData]
            }
        }
    }
}
