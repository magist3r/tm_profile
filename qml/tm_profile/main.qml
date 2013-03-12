import QtQuick 2.0
import QtDesktop 1.0
//import org.tm_profile.profile 1.0
//import "qml/tm_profile/Fields.qml"

ApplicationWindow {
    id:mainWindow
    title: qsTr("tm_profile - program for calculation of cylinder-bevel transmissions")
    visible: true

   // property alias xtList: profile.xtList

    signal arrayChanged

  //  color: syspal.window

    minimumWidth: 450
    minimumHeight: 500

    //property alias checkbox: mTrTab.checkbox
    property bool parametersChanged: false
   // property var my_array: new Array(11)

    /*Connections {
        target: profile
        onS_manualChanged: {
            console.log("ololololololoolololololo")
            my_array = profile.s_manual
            arrayChanged()
        }
    }*/

  /*  function isArrayEmpty() {
        if (mTrTab.checkbox.checked) {
            for (var i=0; i<=10; i++) {
                if (my_array[i] === 0 || my_array[i] === undefined)
                    return true
            }
        }
        return false
    }*/

    // Profile { id: profile }

   /* Connections {
        target: profile
        onCalculateFinished: imageGenerator.onCalculated(_result, _delta_s_max)
    }*/

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
        anchors {
            top: parameters.bottom
            right: parent.right
            left: parent.left
            margins: 10
        }


     //   anchors.margins: 10
        id: checkbox
        checked: profile.useXtList
        onCheckedChanged: {
            profile.useXtList = checked
            images.setImageSource()
        }
        Component.onCompleted: profile["useXtListChanged"].connect(function () { checkbox.checked = profile.useXtList})
        text: qsTr("Use manual xt list")
    }



    TabFrame {
        id: tabs
      //  property int margins : styleitem.style == "mac" ? 16 : 0
        anchors {
            top: checkbox.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 2
            topMargin: 10
        }

      //  height: 200

     //   anchors.margins: margins
      //  onChildrenRectChanged: height = childrenRect.height

        Tab {
            id: mainTab
            title: qsTr("Inertial zone")
            Images {
                id: images
                anchors.fill: parent
            }
        }
        AddTab {}
        MTrTab {
            id: mTrTab
            visible: false
        }
        Tab {
            Repeater {
                model: ["W0", "bw"]
                SpinBox {
                    id: item
                    y: index*height + 10
                 //   value: profile[modelData]
                    onValueChanged: profile[modelData] = value
                    Binding { target: item; property: value; value: profile[modelData] }
                }

            }


        }
    }


}






