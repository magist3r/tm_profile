import QtQuick 1.1
import QtDesktop 0.1


Column {

    Button {
        id: calculate
        signal calculating
        width: 96
        text: qsTr("Calculate")
        tooltip: "Not all fields are not empty"
        onClicked: {

            if (profile.areEmpty()) {
                Qt.createComponent("Alert.qml").createObject(mainWindow)
                console.log("haha")

            } else {
                if (parametersChanged) {
                    profile.saveMainSettings()
                    parametersChanged = false
                }
                calculating()
                profile.calculate();
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
