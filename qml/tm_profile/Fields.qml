import QtQuick 1.1
import QtDesktop 0.1

Repeater {
    id: fields
    property variant _model






                    model: _model
                    // "W0", "E", "x2",

                    Row {

                        y: parent.y + index * (height + 3)
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
                      //      anchors.left: item.left
                     /*       Layout.minimumWidth: 50
                            Layout.maximumWidth: 100
                            Layout.horizontalSizePolicy: Layout.Expanding*/
                            font.pointSize: 10
                        }

                        SpinBox {
                            id: item
                            width: 20
                            anchors.verticalCenter: label.verticalCenter
                            anchors.left: label.right
                            anchors.right: parent.right
                            value: profile[modelData]
                            minimumWidth: 40
               /*             Layout.minimumWidth: 50
                            Layout.maximumWidth: 100
                            Layout.horizontalSizePolicy: Layout.Expanding*/
                       //     Component.onCompleted: console.log(item.value)
                            Component.onCompleted: profile[modelData + "Changed"].connect(function () { item.value = profile[modelData]})
                        }


                    }
                }

           // }
     //   }
  //  }

