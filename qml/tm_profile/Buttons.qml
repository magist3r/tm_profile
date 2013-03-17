import QtQuick 2.0
import QtDesktop 1.0


Row {
    spacing: 10

    Button {
        id: calculate
        width: 96
        text: qsTr("Calculate")
        tooltip: "Not all fields are not empty"
        onClicked: {

            if (profile.areEmpty()) {
                Qt.createComponent("Alert.qml").createObject(mainWindow)
            } else {
                if (parametersChanged) {
                    profile.saveMainSettings()
                    parametersChanged = false
                }

                profile.calculate()
            }
        }
    }

    Button {
        id: calculateR
        width: 96
        text: qsTr("CalculateR")
        onClicked: profile.getRadius()
    }

  /*  Button {
        id: ool
        width: 96
        text: "gkfjgdo"
        onClicked: {

          //  parameters.children.
           // parComboBox.menuItems = ["aa", "mmm"]
           //var modelka = ["aa", "mmm"]
            //parComboBox.model = modelka
          //  parComboBox.update()

        }
    }*/
}
