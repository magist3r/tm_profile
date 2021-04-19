import QtQuick 2.12
import QtQuick.Controls 2.12

//Row {
//    spacing: 10

    Button {
        id: calculate
        width: 96
        text: qsTr("Calculate")
        ToolTip.text: "Not all fields are not empty"
        ToolTip.visible: down
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

  /*  Button {
        id: calculateR
        width: 96
        text: qsTr("CalculateR")
        onClicked: profile.getRadius()
    }*/
//}
