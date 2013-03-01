import QtQuick 2.0
import QtDesktop 1.0
//import org.tm_profile.profile 1.0
//import "qml/tm_profile/Fields.qml"

ApplicationWindow {
    id:mainWindow
    title: qsTr("tm_profile - program for calculation of cylinder-bevel transmissions")
    visible: true

  //  color: syspal.window

    minimumWidth: 640
    minimumHeight: 480

    property bool parametersChanged: false
    property var my_array: new Array(11)

    function isArrayEmpty() {
        if (checkbox.checked) {
            for (var i=0; i<=10; i++) {
                if (my_array[i] === 0 || my_array[i] === undefined)
                    return true
            }
        }
        return false
    }

    // Profile { id: profile }

   /* Connections {
        target: profile
        onCalculateFinished: imageGenerator.onCalculated(_result, _delta_s_max)
    }*/

    TabFrame {
        id: tabs
        property int margins : styleitem.style == "mac" ? 16 : 0
        anchors.fill: parent
        anchors.margins: margins

        Tab {
            title: qsTr("Inertial zone")


            Item {
                id: leftcol
                anchors {
                    left: parent.left
                    top: parent.top
                    right: buttons.left
                    margins: 7
                }

                height: childrenRect.height

                Text {
                    id: title
                    text: qsTr("Main parameters")
                    anchors.horizontalCenter: parent.horizontalCenter
                }



                Item {
                    id: parameters
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: title.bottom
                    height: list.height

                    Label {
                        id: label1
                        text: qsTr("Parameters:")
                        font.pointSize: 10
                        anchors.verticalCenter: list.verticalCenter

                    }

                    ComboBox {
                        id: list
                        model: profile.listOfParameters
                        anchors.left: label1.right
                        anchors.right: parent.right
                        onSelectedTextChanged: profile.loadSettings(list.selectedText)
                        Component.onCompleted: profile.loadSettings(list.selectedText)
                    }
                }



                Item {
                    id: fields1
                    anchors.top: parameters.bottom
                    width: parent.width / 2
                    height: childrenRect.height
                    Fields { model: [ "m", "z1", "z2", "bw", "ra2" ] }
                }

                Item {
                    id: fields2
                    anchors.top: parameters.bottom
                    width: parent.width / 2
                    anchors.left: fields1.right
                    height: childrenRect.height
                    Fields { model: [ "W0", "E", "x2", "d0", "rf2" ] }
                }



               /* Item {
                       id: fields2
                       anchors.top: parameters.bottom

                       width: parent.width / 2
                Fields {


                }
                }*/

//                Text {
//                    id: traectory
//                    text: {
//                        if (profile.xt_w.isEmpty())
//                            return "ssss"
//                    }




//                    anchors.top: fields2.bottom
//                }

              /*  Fields {
                    start_y: parameters.y + parameters.height
                    anchors.right: fields1.left
                    model: [ "m", "z1", "z2", "bw", "W0", "E", "x2", "d0"  ]
                }*/


            }




            Buttons {
                id: buttons
                anchors.right: parent.right
            }

            Item {
                id: images

                property int margin: (width - _image2.width - _image2.width) / 3
                onChildrenRectChanged: margin = (width - _image2.width - _image2.width) / 3



                anchors {
                    top: leftcol.bottom
                    left: parent.left
                    right: parent.right
                }

                Image {
                    id: _image1

          //          width: 200
            //        height: 150
                    anchors {
                        top: parent.top
                        left: parent.left
                        leftMargin: images.margin
                        topMargin: 30
                    }

                    Connections {
                        target: imageGenerator
                        onImagesGenerated: {
                            _image1.source = image1
                        }
                    }
                }

                Image {
                    id: _image2
              //      width: 200
                //    height: 150
                    anchors {
                        top: parent.top
                        right: parent.right
                        rightMargin: images.margin
                        topMargin: 30
                    }
                    Connections {
                        target: imageGenerator
                        onImagesGenerated: {
                            _image2.source = image2
                        }
                    }
                }

                //width: childrenRect.width

                /*Connections {
                    target: imageGenerator
                    onImagesGenerated: {
                        _image1 = Qt.createComponent("Zone.qml").createObject(images, { "source": image1 } )
                        _image2 = Qt.createComponent("Zone.qml").createObject(images, { "source": image2 } )
                        _image1.anchors.top = images.top
                        _image2.anchors.left = _image1.right

                        console.log(image1);

                    }

                }*/

              /*  Connections {
                    target: calculate
                    onCalculating: {
                        _image1.destroy();
                        _image2.destroy();
                    }

                }*/

            }


        }



    Tab {
          title: qsTr("Add. parameters")
          GroupBox {
          Fields {

              model: [ "alpha", "c", "ha"]
          }
              title: qsTr("blabla")
id: field1
              width: parent.width / parent.children.length
          }

          GroupBox {
          Fields {

              model: [ "z0", "x0", "da0"]
          }
id: field2
               anchors.left: field1.right
               title: qsTr("blablabla")
              width: parent.width / parent.children.length
          }

}
    Tab {
        title: qsTr("Manual trajectory")

        CheckBox {
            id: checkbox
            onCheckedChanged: profile.useS_manual = checked
            text: "ololo"
        }

        Repeater {
            model: 11
            anchors.top: checkbox.bottom
            Row {
                visible: checkbox.checked
                y: checkbox.height + index * (height + 10)

            Label {
                id: label
                text: index / 2 + "="
            }

            SpinBox {
                id: item
                value: my_array[index]
                property int f_index: index
                onValueChanged: {
                    my_array[index] = item.value
                    profile.s_manual = my_array
                   //modelData = 50.0
            //        console.log(index)
           //         console.log(my_array[index])
           //         console.log(profile.s_manual[index])
                }
            //    Component.onCompleted: profile.s_manual[f_index] = 0.0
            }}
        }




//        ListModel {
//            id: listModel
//            Component.onCompleted: {
//                for (var i = 0; i < 9; i++) {
//                    listModel.append({"bw": i/2, "xt": 0})
//                }
//            }

//        }

//        TableView {
//            model: listModel
//          TableColumn {

//                role: "bw"
//                title: "bw"
//            }


//                        TableColumn {
//                            role: "xt"
//                            title: "xt"
//                        }

//        }
    }




  }

 }






