import QtQuick 1.1
import QtDesktop 0.1
import org.tm_profile.profile 1.0

ApplicationWindow {

  Profile {
      id: profile
      m: 2
      z1: 32
      z2: 40

  }
  width: 600
  height: 600

  id:mainWindow

  TabFrame {
      id: tabs
      height: parent.height
      anchors.right: parent.right
      anchors.left: parent.left

      Tab {
          title: qsTr("Main parameters")

          Grid {
              id: grid
              columns: 2
              spacing: 10
              flow: Grid.TopToBottom
              anchors.margins: 10

              Repeater {
                  model: [ "m", "z1", "z2", "bw", "W0", "E", "x2", "d0" ]
                  Row {
                      spacing: 5
                      Label {
                          text: [modelData].valueOf(index) + "="
                          anchors.verticalCenter: parent.verticalCenter
                          font.pointSize: 10
                      }

                      SpinBox {
                          value: profile[modelData]
                      }
                  }
              }
          }
      }

      Tab {
          title: "Par"
          Text {
              text: "blalal"
          }
      }
  }






}




