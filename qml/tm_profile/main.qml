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
    property bool reloading: false
    property bool parChanged: false
    property bool modChanged: false

    property alias images: mainTab.item

    Connections {
        target: profile
        onListOfParametersChanged: {
            reloading = true
            parComboBox.model = arg
            parComboBox.currentIndex = profile.listOfParameters.indexOf(profile.getBaseName())
            reloading = false
        }


        onModificationListChanged: {
            if (parChanged) {
                modComboBox.model = arg
                if (modComboBox.currentIndex !== 0) {
                    modChanged = true
                    modComboBox.currentIndex  = 0
                }
                parChanged = false
            } else {
                reloading = true
                modComboBox.model = arg
                modComboBox.currentIndex = profile.modificationList.indexOf(profile.getModName())
                reloading = false
            }
        }
    }

    Item {
        id: rootItem
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 10
        }
        height: childrenRect.height

        Item {
            id: parameters
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 20

            Label {
                id: label1
                text: qsTr("Parameters:")
                font.pointSize: 10
                anchors.verticalCenter: parComboBox.verticalCenter
            }

            ComboBox {
                id: parComboBox
                anchors.left: label1.right
                anchors.right: buttons.left
                anchors.margins: 10
                onCurrentTextChanged: {
                    if (!reloading) {
                        parChanged = true
                        profile.loadSettings(currentText)
                        if (!modChanged) {
                            profile.loadModSettings(currentText, modComboBox.currentText)
                            images.updateImages()
                        } else
                            modChanged = false
                    }
                }

                Component.onCompleted: model = profile.listOfParameters
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
                topMargin: 10
            }

            onCheckedChanged: {
                parametersChanged = true
                profile.useManualXtList = checked
                images.updateImages()
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
            anchors.topMargin: 10

            visible: !checkbox.checked

            Label {
                id: label2
                text: qsTr("Modification:")
                font.pointSize: 10
                anchors.verticalCenter: modComboBox.verticalCenter
            }

            ComboBox {
                id: modComboBox
                anchors.left: label2.right
                anchors.margins: 10
                onCurrentTextChanged: {
                    if (!reloading) {
                        profile.loadModSettings(parComboBox.currentText, currentText)
                        images.updateImages()
                    }
                }

                Component.onCompleted: model = profile.modificationList
            }
        }

        Item {
            id: modFields
            anchors {
                top: modification.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
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
           anchors {
               top: modFields.bottom
               topMargin: 10
               left: parent.left
               right: parent.right
           }
           text: profile.trajectory[2] + '   ' + profile.trajectory[1] + '   ' + profile.trajectory[0]
           horizontalAlignment: Text.AlignHCenter
           wrapMode: Text.WordWrap
       }
    }

    TabView {
        id: tabs

        anchors {
            top: rootItem.bottom
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
