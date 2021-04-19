import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Repeater {
    id: repeater

    RowLayout {
        id: fielditem

        Label {
            id: label
            Layout.minimumWidth: 50
            text: [modelData].valueOf(index) + " ="
            font.pointSize: 10
        }

        SpinBoxX {
            id: item2
            property var ints: ["m", "z1", "z2", "E", "bw", "d0", "alpha", "z0"]
            decimals: {
                if (ints.indexOf(modelData) != -1)
                    return 0
                else {
                    item2.stepSizeX = 0.1
                    return 3
                }
            }
            onValueChanged: {
                parametersChanged = true
                profile[modelData] = item2.value
            }

            /*Binding {
                target: item
                property: "value"
                value: profile[modelData]
            }*/
        }
    }
}
