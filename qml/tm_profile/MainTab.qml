import QtQuick 2.0
import QtDesktop 1.0

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
            onCalculating: {Component.onCompleted: profile[modelData + "Changed"].connect(function () { item.value = profile[modelData]})
                _image1.destroy();
                _image2.destroy();
            }

        }*/

    }


}



