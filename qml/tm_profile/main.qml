import QtQuick 2.0
import QtDesktop 1.0

ApplicationWindow {
    id:mainWindow
    title: qsTr("tm_profile - program for calculation of cylinder-bevel transmissions")
    visible: true

    minimumWidth: 450
    minimumHeight: 550

    property bool parametersChanged: false
    property bool loadSettings: true
    property var parComboBox: createComboBox()

    function createComboBox() {
        var comboBox = Qt.createComponent("MyComboBox.qml")
        if (comboBox.status == Component.Ready)
            comboBox.createObject(parameters, {"anchors.left": label1.right, "anchors.right": buttons.left});

        return comboBox
    }

    Connections {
        target: profile
        onListOfParametersChanged: {
            console.log('olok')
            loadSettings = false
            parComboBox.destroy()
            parComboBox = createComboBox()
            parComboBox.hoveredIndex = 5
           // parComboBox.hoveredText = 'm' + profile.m + '_' + profile.z1 + '_' + profile.z2 + '_' + profile.bw
            loadSettings = true
        }
    }

  //  property var parList: profile.listOfParameters
  //  property var modList: profile.modificationList

    Item {
        id: parameters
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 20
        anchors.margins: 10

        Label {
            id: label1
            text: qsTr("Parameters:")
            font.pointSize: 10
            anchors.verticalCenter: parComboBox.verticalCenter

        }

/*        ComboBox {
            id: parComboBox
            model: profile.listOfParameters
            anchors.left: label1.right
            anchors.right: buttons.left
            anchors.margins: 10
            onSelectedTextChanged: {
                if (loadSettings) {
                    profile.loadSettings(parComboBox.selectedText)
                    images.setImageSource()
                }


            }
            Component.onCompleted: profile.loadSettings(parComboBox.selectedText)
        }*/

        Buttons {
            id: buttons
            anchors.right: parent.right
        }
    }

    CheckBox {
        id: checkbox

        anchors {
            top: parameters.bottom
            right: parent.right
            left: parent.left
            margins: 10
        }

        onCheckedChanged: {
            parametersChanged = true
            profile.useManualXtList = checked
            images.setImageSource()
        }

        Binding {
            target: checkbox
            property: "checked"
            value: profile.useManualXtList
        }

        text: qsTr("Use manual xt list")
    }

    Item {
        id: modification
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: checkbox.bottom
        height: modComboBox.height
        anchors.margins: 10

        Label {
            id: label2
            text: qsTr("Modification:")
            font.pointSize: 10
            anchors.verticalCenter: modList.verticalCenter
        }

        ComboBox {
            id: modComboBox
            model: profile.modificationList
            anchors.left: label2.right
          //  anchors.right: modFields.left
            anchors.margins: 10
            onSelectedTextChanged: {
                profile.loadModSettings(parComboBox.selectedText, modComboBox.selectedText)
                images.setImageSource()
            }
            Component.onCompleted: profile.loadModSettings(parComboBox.selectedText, modComboBox.selectedText)

      /*      Binding {
                target: modList
                property: "model"
                value: profile.modificationList
            }*/
        }


    }

        Item {
            id: modFields
            anchors {
                top: modification.bottom
                left: parent.left
                right: parent.right
                margins: 10
            }
            height: childrenRect.height

            Repeater {
                model: [ "mod0", "modCenter", "modBw"]
                Item {
                    id: fielditem
                    x: index * (width + 10)
                    width: parent.width / 3 - 10
                    height: label.height


                    Label {
                        id: label
                        anchors.left: parent.left

                    //    width: 80
                        text: [modelData].valueOf(index) + "="
                        font.pointSize: 10
                    }

                    SpinBox {
                        id: item
                        anchors.verticalCenter: label.verticalCenter
                        anchors.left: label.right
                        anchors.leftMargin: 10
                        anchors.right: parent.right
                        minimumValue: -0.1
                        maximumValue: 0.1
                        singleStep: 0.005
                        onValueChanged: {
                            parametersChanged = true
                            profile[modelData] = item.value
                        }

                        Binding {
                            target: item
                            property: "value"
                            value: profile[modelData]
                        }
                    }
                }
            }


       }





    TabFrame {
        id: tabs

        anchors {
            top: modFields.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 2
            topMargin: 10
        }

        Tab {
            id: mainTab
            title: qsTr("Inertial zone")
            Images {
                id: images
                anchors.fill: parent
            }
        }

        Parameters { id: parametersTab }

        XtList { id: xtListTab }

    }
}
