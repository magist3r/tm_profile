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

    minimumWidth: 640
    minimumHeight: 480

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

    TabFrame {
        id: tabs
        property int margins : styleitem.style == "mac" ? 16 : 0
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        height: 200

        anchors.margins: margins
      //  onChildrenRectChanged: height = childrenRect.height

        MainTab { id: mainTab }
        AddTab {}
        MTrTab { id: mTrTab }
    }

    Images {
        id: images

        anchors {
            top: tabs.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
}






