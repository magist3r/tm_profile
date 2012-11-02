import QtQuick 1.1
import QtDesktop 0.1

Column {
    id: fields
    property string _title
    property variant _anchors_left
    property variant _anchors_right
    property int _columns
    property variant _model
    property int _width

        anchors.left: _anchors_left
        anchors.right: _anchors_right
        GroupBox {
            title: _title
            width: _width
            Grid {
                columns: _columns
                spacing: 10
                flow: Grid.TopToBottom
                anchors.margins: 10
                width: _width

            Repeater {

                    model: _model
                    // "W0", "E", "x2",

                    RowLayout {

                        Label {
                            id: label
                            //width: 33
                            text: [modelData].valueOf(index) + "="
                      //      anchors.left: item.left
                            font.pointSize: 10
                        }

                        SpinBox {
                            id: item
                            //width: _width / _columns - label.width
                            anchors.verticalCenter: label.verticalCenter
                            value: profile[modelData]
                       //     Component.onCompleted: console.log(item.value)
                            Component.onCompleted: profile[modelData + "Changed"].connect(function () { item.value = profile[modelData]})
                        }


                    }
                }

            }
        }
    }

