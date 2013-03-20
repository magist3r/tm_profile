import QtQuick 2.0
import QtQuick.Controls 1.0

Tab {
    title: qsTr("Manual trajectory")

    Item {
        id: leftColumn
        anchors {
            top: parent.top
            bottom: parent.bottom
            topMargin: 10
        }

        width: parent.width / 2

        Repeater {
            model: profile.bw + 1

            Item {
                visible: checkbox.checked

                anchors {
                    left: parent.left
                    right: parent.right
                    margins: 10
                }

                height: item.height
                y: index * (height + 2)

                Label {
                    id: label
                    width: 40
                    text: index + " ="
                    font.pointSize: 10
                }

                SpinBox {
                    id: item
                    anchors.left: label.right
                    anchors.right: parent.right
                    anchors.verticalCenter: label.verticalCenter

                    onValueChanged: {
                        parametersChanged = true
                        profile.manualXtList[index*2] = item.value
                    }

                    Binding {
                        target: item
                        property: "value"
                        value: profile.manualXtList[index*2]
                    }
                }
            }
        }
    }

    Item {
        id: rightColumn
        anchors {
            left: leftColumn.right
            topMargin: 10
            top: parent.top
            bottom: parent.bottom
        }

        width: parent.width / 2

        Repeater {
            model: profile.bw

            Item {
                visible: checkbox.checked

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                height: item2.height
                y: index * (height + 2)

                Label {
                    id: label2
                    width: 40
                    font.pointSize: 10
                    text: (index * 2 + 1) / 2 + " ="
                }

                SpinBox {
                    id: item2
                    anchors.left: label2.right
                    anchors.right: parent.right
                    anchors.verticalCenter: label2.verticalCenter

                    onValueChanged: {
                        parametersChanged = true
                        profile.manualXtList[index*2 + 1] = item2.value
                    }

                    Binding {
                        target: item2
                        property: "value"
                        value: profile.manualXtList[index*2 + 1]
                    }
                }
            }
        }
    }
}
