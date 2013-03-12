import QtQuick 2.0
import QtDesktop 1.0

Tab {

   // property alias checkbox: checkbox
    title: qsTr("Manual trajectory")


    Repeater {
        model: profile.bw * 2 + 1
     //   anchors.top: checkbox.bottom
        Row {
            visible: checkbox.checked
            y: index * (height + 10)

        Label {
            id: label
            text: index / 2 + "="
        }

        SpinBox {
            id: item
            value: profile.xtList[index]
            //property int f_index: index
            onValueChanged: {
                if (profile.xtList[index] != item.value) {
                  //  my_array[index] = item.value
                    parametersChanged = true
                    profile.xtList[index] = item.value
                    console.log(profile.xtList[index])
                }



               //modelData = 50.0
        //        console.log(index)
       //         console.log(my_array[index])
       //         console.log(profile.s_manual[index])
            }
            Component.onCompleted: profile["xtListChanged"].connect(function () {
                if (item.value != profile.xtList[index])
                    item.value = profile.xtList[index]
            })

        }
        }
    }
}
