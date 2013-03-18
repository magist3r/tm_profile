import QtQuick 2.0
import QtDesktop 1.0

ComboBox {
    id: item
    model: profile.modificationList
    anchors.left: label2.right
  //  anchors.right: buttons.left
    anchors.margins: 10
    onSelectedTextChanged: {
        if (!reloading) {
            if (firstRun) {
                profile.loadModSettings(parComboBox.selectedText,item.selectedText)
                updateSettingsAndImages(true, item.selectedText)
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
}
