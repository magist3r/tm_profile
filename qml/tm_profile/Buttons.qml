import QtQuick 2.0
import QtDesktop 1.0


Column {

    Button {
        id: calculate
        signal calculating
        width: 96
        text: qsTr("Calculate")
        tooltip: "Not all fields are not empty"
        onClicked: {

            if (profile.areEmpty() || isArrayEmpty()) {
                Qt.createComponent("Alert.qml").createObject(mainWindow)
                console.log("haha")

            } else {
                if (parametersChanged) {
                    profile.saveMainSettings()
                    parametersChanged = false
                }
                calculating()
                profile.calculate()
            }
        }
    }
    Button {
        id: calculateR
        width: 96
        text: qsTr("CalculateR")
        onClicked: {
            profile.getRadius();
        }
    }

    Loader { id: loader }





}
