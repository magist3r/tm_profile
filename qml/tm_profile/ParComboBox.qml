import QtQuick 2.0
import QtDesktop 1.0

ComboBox {
    id: item
    model: profile.listOfParameters
    signal changeSelectedText
    anchors.left: label1.right
    anchors.right: buttons.left
    anchors.margins: 10
    onSelectedTextChanged: {
        console.log('lollllll')
        if (firstRun) {
            profile.loadSettings(item.selectedText)
        } else {
            if (!reloading)
                updateSettingsAndImages()
            else {

                if (item.selectedText !== profile.getBaseName()) {
                    console.log("hahahah")
                  //  changeSelectedText()
            //    item.selectedText = profile.getBaseName()

                }}
        }

    }


}
