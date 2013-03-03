import QtQuick 2.0
import QtDesktop 1.0

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
