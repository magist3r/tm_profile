import QtQuick 1.1
import QtDesktop 0.1
import org.tm_profile.profile 1.0
//import "qml/tm_profile/Fields.qml"

Rectangle {
    id:mainWindow

    SystemPalette {id: syspal}
    StyleItem{ id: styleitem}
    color: syspal.window
    width: 538 + tabs.margins * 2
    height: 360 + tabs.margins * 2

    Profile {
      id: profile

    /*  onMChanged: repeater1.itemAt(0).children[1].value = profile.m
      onZ1Changed: repeater1.itemAt(1).children[1].value = profile.z1
      onZ2Changed: repeater1.itemAt(2).children[1].value = profile.z2
      onBwChanged: repeater1.itemAt(3).children[1].value = profile.bw
      //onW0Changed: repeater1.itemAt(4).children[1].value = profile.W0
      //onEChanged: repeater1.itemAt(5).children[1].value = profile.E
      //onX2Changed: repeater1.itemAt(6).children[1].value = profile.x2
      onD0Changed: repeater1.itemAt(7).children[1].value = profile.d0*/

    }




  TabFrame {
      id: tabs
      property int margins : styleitem.style == "mac" ? 16 : 0
      anchors.fill: parent
      anchors.margins: margins
      Tab {
          title: qsTr("Inertial zone")

          Row {
              id: contentRow
              anchors.fill: parent
              anchors.margins: 8
              spacing: 16
              Column {
                  id: leftcol
                  Row {
                      Label {
                          id: label1
                          text: qsTr("Parameters:")
                          font.pointSize: 10
                          anchors.verticalCenter: parent.verticalCenter
                      }

                      ComboBox {
                          id: list
                          model: profile.listOfParameters
                          width: 200
                          onPressedChanged: console.log(list.selectedText)
                      }
                  }

                  ButtonRow {
                  //    anchors.top: row1.bottom

                      Button {
                          id: loadbtn
                          width: 96
                          text: qsTr("Load")
                          onClicked: profile.loadSettings(list.selectedText)
                      }

                      Button {
                          id: savebtn
                          width: 96
                          text: qsTr("Save")
                      }
                  }

                  ButtonRow {
                      Button {
                          id: newbtn
                          width: 96
                          text: qsTr("New")
                      }

                      Button {
                          id: convertbtn
                          width: 96
                          text: qsTr("Convert")
                          onClicked: profile.convertSettings()
                      }
                      Button {
                          id: calculate
                          width: 96
                          text: qsTr("Calculate")
                          onClicked: profile.onCalculate();
                      }
                  }
              }



          /*    Fields {
                  _title: qsTr("Main parameters")
                  _columns: 2
                  _model: [ "m", "z1", "z2", "bw", "W0", "E", "x2", "d0"  ]
                  _anchors_right: parent.right
                  _width: parent.width - leftcol.width

              }*/

            /*Column {
                  anchors.right: parent.right
                  GroupBox {
                      title: qsTr("Main parameters")

                      Grid {
                          id: grid
                          columns: 2
                          spacing: 10
                          flow: Grid.TopToBottom
                          anchors.margins: 10

                          Repeater {
                              id: repeater1
                              model: [ "m", "z1", "z2", "bw", "W0", "E", "x2", "d0"  ]
                              // "W0", "E", "x2",
                              Row {
                                  spacing: 5
                                  Label {
                                      text: [modelData].valueOf(index) + "="
                                      anchors.verticalCenter: parent.verticalCenter
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
              }*/


          }
      }



      Tab {
          title: qsTr("Add. parameters")
          GroupBox {
          Fields {

              _model: [ "alpha", "c", "ha"]
          }
              title: qsTr("blabla")
id: field1
              width: parent.width / parent.children.length
          }

          GroupBox {
          Fields {

              _model: [ "z0", "x0", "da0"]
          }
id: field2
               anchors.left: field1.right
               title: qsTr("blablabla")
              width: parent.width / parent.children.length
          }

          GroupBox {
          Fields {

              _model: [ "ra2"]
          }
          id:field3

             title: qsTr("blablabla")
              anchors.left: field2.right
              width: parent.width / parent.children.length
          }


}



      Tab {
          title: "Par"
      //   Grid {
       //       id: grid
       //       columns:1
     //     Column {
       //       anchors.fill: parent
        //      spacing: 10


          Repeater {
              model: 3


        // {
          /*  anchors {
                     //  top: parent.top
                        left: parent.left
                        right: parent.right
                       margins: 10
                    }*/
//Row {
    //anchors.fill: parent
          RowLayout {
              y: parent.y + index * (height + 3)
              anchors {

                                      left: parent.left
                                      right: parent.right
                                     margins: 10
                                  }
              height: 30
     //         anchors.left: grid.left
     //         anchors.right: grid.right

              Rectangle {
                //  width: 100
                  height: parent.height
                  color: "red"
                  Layout.minimumHeight: 200
                  Layout.maximumHeight: 400
                  Layout.verticalSizePolicy: Layout.Expanding

              }
              Rectangle {
                //  width: 100
                  height: parent.height
                  color: "blue"
                  Layout.minimumHeight: 200
                  Layout.maximumHeight: 400
                  Layout.verticalSizePolicy: Layout.Expanding

              }
          }
//}
//}
      }
  }
 }





}




