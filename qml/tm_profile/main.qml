import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Private 1.0

ApplicationWindow {
    id:mainWindow
    title: qsTr("tm_profile - program for calculation of cylinder-bevel transmissions")
    visible: true

    minimumWidth: 480
    minimumHeight: 640

    property bool parametersChanged: false
    property bool firstRun: true
 //   property bool reloading: false
    property bool parChanged: false
    property bool modChanged: false

  //  property var parComboBox: createComboBox("ParComboBox.qml", parameters)
 //   property var modComboBox: createComboBox("ModComboBox.qml", modification)

   // property alias image1: mainTab.component.image1
  //  property alias image2: images.image2

    function createComboBox(component, parent) {
        var comp = Qt.createComponent(component)
        if (comp.status == Component.Ready)
            var comboBox = comp.createObject(parent);

        return comboBox
    }



  /*  Connections {
        target: profile
        onListOfParametersChanged: {


        onModificationListChanged: {




       //     console.log(profile.getModName() + ' slot')
       //     modComboBox.currentIndex = profile.modificationList.indexOf(profile.getModName())
        //    modComboBox.destroy()
       //     modComboBox = createComboBox("ModComboBox.qml", modification, "modComboBox")
         //   modComboBox.selectedText = profile.getModName()
            //parComboBox.hoveredIndex = 5
           // parComboBox.hoveredText = 'm' + profile.m + '_' + profile.z1 + '_' + profile.z2 + '_' + profile.bw


        }*/

      /*  onCalculateFinished: {
            loadSettings = false

            parComboBox.selectedText = 'm' + profile.m + '_' + profile.z1 + '_' + profile.z2 + '_' + profile.bw
            modComboBox.selectedText = profile.getModName()
            loadSettings = true
         }*/

 /*   Connections {
        target: parComboBox
        onChangeSelectedText:{
            console.log('ololo')
            parComboBox.selectedText = profile.getBaseName()
        }
    }*/

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

        ComboBox {
            id: parComboBox
            property bool reloading: false
            model: profile.listOfParameters
            anchors.left: label1.right
            anchors.right: buttons.left
            anchors.margins: 10
            onCurrentTextChanged: {
                if (!reloading) {
                    parChanged = true
                    profile.loadSettings(currentText)
                    console.log(parChanged, modChanged)
                    if (!modChanged) {
                        profile.loadModSettings(currentText, modComboBox.currentText)
                             console.log('load mod')
                    } else {
                        modChanged = false
                    }


                   // updateImages()
                    //updateSettingsAndImages()
                   // console.log(modComboBox.currentText + 'jk')
                }
            }

            onModelChanged: {
                console.log('1u' + profile.getBaseName())
                reloading = true
                currentIndex = profile.listOfParameters.indexOf(profile.getBaseName())
                reloading = false
            }

           // Component.onCompleted: profile.loadSettings(parComboBox.currentText)
        }

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

        ComboBox {
            id: modComboBox
            property bool reloading: false
            model: profile.modificationList
            anchors.left: label2.right
          //  anchors.right: modFields.left
            anchors.margins: 10
            onCurrentTextChanged: {
                if (!reloading) {
                    console.log('ololololololololololo')
                    profile.loadModSettings(parComboBox.currentText, currentText)
                  //  updateImages()
                } else {
                    console.log('aaaaaaaa')
                }

             /*   if (!reloading) {
                    console.log('gdhvbfhvgh!!!!!!!')
                    updateSettingsAndImages()
                }*/
            }

            onModelChanged: {
                console.log('2u')
                if (parChanged) {
                    if (currentIndex !== 0) {
                        modChanged = true
                        currentIndex = 0
                    }
                    parChanged = false
                } else {
                    reloading = true
                    currentIndex = profile.modificationList.indexOf(profile.getModName())
                    reloading = false
                }
            }

          //  Component.onCompleted: profile.loadModSettings(parComboBox.currentText, modComboBox.currentText)
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
                        decimals: 3
                        stepSize: 0.005
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

       Text {
           anchors.top: modFields.bottom
           text: profile.trajectory[2] + '   ' + profile.trajectory[1] + '   ' + profile.trajectory[0]
       }





    TabView {
        id: tabs


        anchors {
            top: modFields.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 2
            topMargin: 10
        }

       //  property alias images: images

        Tab {
            id: mainTab
            title: qsTr("Inertial zone")
          //  property alias images: images
            Images {
                id: images
                anchors.fill: parent
            }


        }

        Parameters { id: parametersTab }

        //XtList { id: xtListTab }

    }
}
