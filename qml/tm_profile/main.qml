import QtQuick 2.0
import QtDesktop 1.0

ApplicationWindow {
    id:mainWindow
    title: qsTr("tm_profile - program for calculation of cylinder-bevel transmissions")
    visible: true

    minimumWidth: 480
    minimumHeight: 640

    property bool parametersChanged: false
    property bool firstRun: true
    property bool reloading: false

    property var parComboBox: createComboBox("ParComboBox.qml", parameters)
    property var modComboBox: createComboBox("ModComboBox.qml", modification)

    property alias image1: images.image1
    property alias image2: images.image2

    function createComboBox(component, parent) {
        var comp = Qt.createComponent(component)
        if (comp.status == Component.Ready)
            var comboBox = comp.createObject(parent);

        return comboBox
    }

    function updateSettingsAndImages(imagesOnly, name) {
        var parName = parComboBox.selectedText
        var modName

        if (name === undefined)
            modName = modComboBox.selectedText
        else
            modName = name

        if (!imagesOnly) {
            profile.loadSettings(parName)
            profile.loadModSettings(parName, modName)
        }

        image1.source = ""
        image2.source = ""

        var baseName = profile.dataLocation + '/' + parName
        if (checkbox.checked) {
            image1.source = baseName + '_manual_1.png'
            image2.source = baseName + '_manual_2.png'
        } else {
            image1.source = baseName + '_' + modName + '_1.png'
            image2.source = baseName + '_' + modName + '_2.png'
        }
    }

    Connections {
        target: profile
        onListOfParametersChanged: {
            reloading = true
            parComboBox.destroy()
            parComboBox = createComboBox("ParComboBox.qml", parameters, "parComboBox")
            parComboBox.selectedText = profile.getBaseName()
            console.log('don')
            reloading = false
        }

        onModificationListChanged: {
            reloading = true
            modComboBox.destroy()
            modComboBox = createComboBox("ModComboBox.qml", modification, "modComboBox")
         //   modComboBox.selectedText = profile.getModName()
            //parComboBox.hoveredIndex = 5
           // parComboBox.hoveredText = 'm' + profile.m + '_' + profile.z1 + '_' + profile.z2 + '_' + profile.bw
            reloading = false

        }

      /*  onCalculateFinished: {
            loadSettings = false

            parComboBox.selectedText = 'm' + profile.m + '_' + profile.z1 + '_' + profile.z2 + '_' + profile.bw
            modComboBox.selectedText = profile.getModName()
            loadSettings = true
         }*/
    }

    Connections {
        target: parComboBox
        onChangeSelectedText:{
            console.log('ololo')
            parComboBox.selectedText = profile.getBaseName()
        }
    }

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
            updateSettingsAndImages(true)
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

        visible: !checkbox.checked


        Label {
            id: label2
            text: qsTr("Modification:")
            font.pointSize: 10
            anchors.verticalCenter: modComboBox.verticalCenter
        }

      /*  ComboBox {
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
        //}


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

            visible: !checkbox.checked


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
