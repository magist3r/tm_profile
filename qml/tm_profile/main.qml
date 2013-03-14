import QtQuick 2.0
import QtDesktop 1.0

ApplicationWindow {
    id:mainWindow
    title: qsTr("tm_profile - program for calculation of cylinder-bevel transmissions")
    visible: true

    minimumWidth: 450
    minimumHeight: 550

    property bool parametersChanged: false

    Item {
        id: parameters
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: list.height
        anchors.margins: 10

        Label {
            id: label1
            text: qsTr("Parameters:")
            font.pointSize: 10
            anchors.verticalCenter: list.verticalCenter

        }

        ComboBox {
            id: list
            model: profile.listOfParameters
            anchors.left: label1.right
            anchors.right: buttons.left
            anchors.margins: 10
            onSelectedTextChanged: {
                profile.loadSettings(list.selectedText)
                images.setImageSource()
            }
            Component.onCompleted: profile.loadSettings(list.selectedText)
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
            profile.useXtList = checked
            images.setImageSource()
        }

        Binding {
            target: checkbox
            property: "checked"
            value: profile.useXtList
        }

        text: qsTr("Use manual xt list")
    }

    TabFrame {
        id: tabs

        anchors {
            top: checkbox.bottom
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
