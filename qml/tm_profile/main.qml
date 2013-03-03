import QtQuick 2.0
import QtDesktop 1.0
//import org.tm_profile.profile 1.0
//import "qml/tm_profile/Fields.qml"

ApplicationWindow {
    id:mainWindow
    title: qsTr("tm_profile - program for calculation of cylinder-bevel transmissions")
    visible: true

    signal arrayChanged

  //  color: syspal.window

    minimumWidth: 640
    minimumHeight: 480

    property bool parametersChanged: false
    property var my_array: new Array(11)

    Connections {
        target: profile
        onS_manualChanged: {
            console.log("ololololololoolololololo")
            my_array = profile.s_manual
            arrayChanged()
        }
    }

    function isArrayEmpty() {
        if (checkbox.checked) {
            for (var i=0; i<=10; i++) {
                if (my_array[i] === 0 || my_array[i] === undefined)
                    return true
            }
        }
        return false
    }

    // Profile { id: profile }

   /* Connections {
        target: profile
        onCalculateFinished: imageGenerator.onCalculated(_result, _delta_s_max)
    }*/

    TabFrame {
        id: tabs
        property int margins : styleitem.style == "mac" ? 16 : 0
        anchors.fill: parent
        anchors.margins: margins

        MainTab {}
        AddTab {}
        MTrTab {}
    }
}






