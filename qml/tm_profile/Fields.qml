import QtQuick 1.1
import QtDesktop 0.1

Column {
    id: fields
    property string _title
    property variant _anchors_left
    property int _columns
    property variant _model

        anchors.left: _anchors_left
        GroupBox {
            title: _title
            Grid {
                columns: _columns
                spacing: 10
                flow: Grid.TopToBottom
                anchors.margins: 10

            Repeater {

                    model: _model
                    // "W0", "E", "x2",
                    Column {
                        spacing: 5
                        Label {
                            text: [modelData].valueOf(index) + "="
                            anchors.left: parent.left
                            font.pointSize: 10
                        }

                        SpinBox {
                            id: item
                            value: profile[modelData]
                       //     Component.onCompleted: console.log(item.value)
                            Component.onCompleted: profile[modelData + "Changed"].connect(function () { item.value = profile[modelData]})
                        }
                    }
                }

            }
        }
    }

