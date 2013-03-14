import QtQuick 2.0
import QtDesktop 1.0

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
            text: [modelData].valueOf(index) + "="
            font.pointSize: 10
        }

        SpinBox {
            id: item
            anchors.verticalCenter: label.verticalCenter
            anchors.left: label.right
            anchors.right: parent.right
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
