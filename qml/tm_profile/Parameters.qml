import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout {
    Layout.preferredWidth: parent.width

    Text {
        text: qsTr("Main parameters")
        Layout.alignment: Text.AlignHCenter
    }

    GridLayout
    {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.topMargin: 10
        Layout.bottomMargin: 20
        columnSpacing: 50

        rows: 5
        flow: GridLayout.TopToBottom
        Fields { model: [ "m", "z1", "z2", "bw", "ra2", "W0", "E", "x2", "d0", "rf2" ] }
    }

    Text {
        Layout.alignment: Text.AlignHCenter
        text: qsTr("Additional parameters")
    }

    GridLayout
    {
        rows: 3
        flow: GridLayout.TopToBottom
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        columnSpacing: 50
        Layout.topMargin: 10
        Layout.bottomMargin: 20
        Fields { model: [ "alpha", "c", "ha", "z0", "x0", "da0"] }
    }
}
