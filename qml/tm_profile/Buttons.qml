import QtQuick 1.1
import QtDesktop 0.1


Column {

    Button {
        id: loadbtn
        width: 96
        text: qsTr("Load")
        onClicked: profile.loadSettings(list.selectedText)
    }

    Button {
        id: savebtn
        width: 96
        text: qsTr("Save")
    }

    Button {
        id: newbtn
        width: 96
        text: qsTr("New")
    }

    Button {
        id: convertbtn
        width: 96
        text: qsTr("Convert")
        onClicked: profile.convertSettings()
    }
    Button {
        id: calculate
        width: 96
        text: qsTr("Calculate")
        onClicked: {
            calc.setProfile(profile);
            calc.start();
        }
    }
}
