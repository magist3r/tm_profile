import QtQuick 1.1
import QtDesktop 0.1

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
            value: profile[modelData]
            onValueChanged: {
                parametersChanged = true
                profile[modelData] = item.value
            }
            Component.onCompleted: profile[modelData + "Changed"].connect(function () { item.value = profile[modelData]})
        }
    }
}
