import QtQuick 1.1
import QtDesktop 0.1

Window {
  width: 200
  height: 200

  id:mainWindow

  Text {
      id: text
      x: 0
      y: 0
      text: "Hello World!"
  }

  Button {
      id:mainButton
      text: "OK!"
  }
}
