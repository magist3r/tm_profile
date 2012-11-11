import QtQuick 1.1
import QtDesktop 0.1
import org.tm_profile.profile 1.0
import org.tm_profile.calc 1.0
//import "qml/tm_profile/Fields.qml"

Rectangle {
    id:mainWindow

    SystemPalette { id: syspal }
    StyleItem{ id: styleitem }
    color: syspal.window
    width: 538 + tabs.margins * 2
    height: 360 + tabs.margins * 2

    Profile { id: profile }
    Calc {
        id: calc
        profile: profile
    }

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

            //    width: parent.width - buttons.width

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
                        onPressedChanged: console.log(list.selectedText)
                    }
                }



                Item {
                    id: fields1
                    anchors.top: parameters.bottom
                    width: parent.width / 2
                    height: childrenRect.height
                    Fields { model: [ "m", "z1", "z2", "bw" ] }
                }

                Item {
                    id: fields2
                    anchors.top: parameters.bottom
                    width: parent.width / 2
                    anchors.left: fields1.right
                    height: childrenRect.height
                    Fields { model: [ "W0", "E", "x2", "d0" ] }
                }


               /* Item {
                       id: fields2
                       anchors.top: parameters.bottom

                       width: parent.width / 2
                Fields {


                }
                }*/

                Text {
                    text: "sggvcdghvfjvfg"
                    anchors.top: fields2.bottom
                }

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

          GroupBox {
          Fields {

              model: [ "ra2"]
          }
          id:field3

             title: qsTr("blablabla")
              anchors.left: field2.right
              width: parent.width / parent.children.length
          }


}




  }
 }






