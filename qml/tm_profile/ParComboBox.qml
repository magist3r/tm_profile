/*import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Private 1.0

ComboBox {
    id: item
    model: profile.listOfParameters
   // signal changeSelectedText
    anchors.left: label1.right
    anchors.right: buttons.left
    anchors.margins: 10
    onCurrentTextChanged: {
        console.log('lollllll' + profile.listOfParameters.indexOf(currentText))
        if (firstRun) {
            profile.loadSettings(item.currentText)
        } else {

            else {
             //   item.currentIndex =
                if (item.currentText !== profile.getBaseName()) {
                    console.log("hahahah")

                  //  changeSelectedText()
            //    item.selectedText = profile.getBaseName()

                }}
        }

    }

*/
*/
