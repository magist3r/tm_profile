import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


ApplicationWindow {
    id: mainWindow
    title: qsTr("tm_profile - program for calculation of cylinder-bevel transmissions")
    visible: true

    property int windowMargins: 10

    minimumWidth: rootLayout.Layout.minimumWidth
    minimumHeight: rootLayout.Layout.minimumHeight
    width: rootLayout.implicitWidth
    height: rootLayout.implicitHeight

    property bool parametersChanged: false
    property bool reloading: false
    property bool parChanged: false
    property bool modChanged: false

    property alias images: mainTab

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

    ColumnLayout {
        id: rootLayout
        anchors.fill: parent
        spacing: windowMargins

        TabBar {
            id: tabs

            Layout.fillWidth: true
            TabButton {
                text: qsTr("Inertial zone")
            }
            TabButton {
                text: qsTr("Parameters")
            }
            TabButton {
                text: qsTr("Manual trajectory")
            }
        }

        RowLayout {
            id: parameters
            spacing: 10
            Layout.leftMargin: windowMargins
            Layout.rightMargin: windowMargins

            Label {
                id: label1
                text: qsTr("Parameters:")
                font.pointSize: 10
            }

            ComboBox {
                id: parComboBox
                Layout.fillWidth: true
                Layout.maximumWidth: 200
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
                Layout.fillWidth: true
            }
        }

        RowLayout {
            id: modification
            Layout.leftMargin: windowMargins
            Layout.rightMargin: windowMargins

            // visible: !checkbox.checked

            Label {
                id: label2
                text: qsTr("Modification:")
                font.pointSize: 10
            }

            ComboBox {
                id: modComboBox
                Layout.fillWidth: true
                Layout.maximumWidth: 200
                onCurrentTextChanged: {
                    if (!reloading) {
                        profile.loadModSettings(parComboBox.currentText, currentText)
                        if (currentText == "manual")
                            checkbox.checked = true
                        else
                            checkbox.checked = false

                        images.updateImages()
                    }
                }

                Component.onCompleted: model = profile.modificationList
            }

            CheckBox {
                id: checkbox

                onCheckedChanged: {
                    parametersChanged = true
                    profile.useManualXtList = checked
                    if (checked) {
                        var index = profile.modificationList.indexOf("manual")
                        if (index !== -1)
                            modComboBox.currentIndex = index
                    } else {
                        if (modComboBox.currentText == "manual") {
                            modComboBox.currentIndex = 0
                        }
                    }
                }


                Binding {
                    target: checkbox
                    property: "checked"
                    value: profile.useManualXtList
                }

                text: qsTr("Use manual xt list")
            }
        }



        RowLayout {
            id: modFields
            Layout.leftMargin: windowMargins
            Layout.rightMargin: windowMargins
            //    visible: !checkbox.checked

            Repeater {
                model: [ "mod0", "modCenter", "modBw"]
                RowLayout {
                    id: fielditem
                    Label {
                        id: label
                        text: [modelData].valueOf(index) + "="
                        font.pointSize: 10
                    }

                    SpinBoxX {
                        id: item2
                        Layout.fillWidth: true
                        //Layout.minimumWidth:
                        //minimumValue: -0.1
                        //maximumValue: 0.1
                        decimals: 3
                        stepSize: 5
                        onValueChanged: {
                            parametersChanged = true
                            profile[modelData] = item2.value
                        }

                        Binding {
                            target: item2
                            property: "value"
                            value: profile[modelData]
                        }
                    }
                }
            }
        }

        Text {
            Layout.leftMargin: windowMargins
            Layout.rightMargin: windowMargins

            function num(number) {
                if (number > 0)
                    return '+ ' + number.toFixed(6)
                else
                    return '- ' + Math.abs(number.toFixed(6))
            }

            text: qsTr("Trajectory: %1 * w^2 %2 * w %3").arg(num(profile.trajectory[2])).arg(num(profile.trajectory[1])).arg(num(profile.trajectory[0]))
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }

        StackLayout {
            Layout.leftMargin: windowMargins
            Layout.rightMargin: windowMargins
            Layout.fillHeight: true

            currentIndex: tabs.currentIndex
            Images { id: mainTab }
            Parameters { id: parametersTab }
            XtList { id: xtListTab }
        }
    }
}
