import QtQuick 2.0
import QtDesktop 1.0

ComboBox {
    id: item
    model: profile.listOfParameters
   // anchors.left: label1.right
  //  anchors.right: buttons.left
    anchors.margins: 10
    onSelectedTextChanged: {
        if (loadSettings) {
            profile.loadSettings(item.selectedText)
            images.setImageSource()


        }
    }
 //   Component.onCompleted: profile.loadSettings(parComboBox.selectedText)
}
