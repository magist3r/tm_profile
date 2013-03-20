/*import QtQuick 2.0
import QtQuick.Controls 1.0

ComboBox {
    id: item
    model: profile.modificationList
    anchors.left: label2.right
  //  anchors.right: buttons.left
    anchors.margins: 10
    onCurrentTextChanged: {
        if (!reloading) {
            if (firstRun) {
                profile.loadModSettings(parComboBox.currentText, item.currentText)
                updateSettingsAndImages(true, item.currentText)
                firstRun = false
            }
            updateSettingsAndImages()
           // profile.loadModSettings(parComboBox.selectedText, item.selectedText)
      //      images.setImageSource(parComboBox.selectedText, item.selectedText)


        } else {
            this.selectedText = profile.getModName()
        }

    }
 //   Component.onCompleted: profile.loadSettings(parComboBox.selectedText)
}*/
