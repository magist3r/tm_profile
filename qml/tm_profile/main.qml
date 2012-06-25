import QtQuick 1.1
import QtDesktop 0.1
import org.tm_profile.profile 1.0

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
                  }
              }

              Column {
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
                              model: [ "m", "z1", "z2", "bw", "d0" ]
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
              }
          }
      }



      Tab {

}

      Tab {
          title: "Par"
          Text {
              text: "blalal"
          }
      }
  }






}




