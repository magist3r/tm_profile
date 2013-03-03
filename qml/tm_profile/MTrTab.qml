import QtQuick 2.0
import QtDesktop 1.0

Tab {
    title: qsTr("Manual trajectory")

    CheckBox {
        id: checkbox
        checked: profile.useS_manual
        onCheckedChanged: profile.useS_manual = checked
      //  Component.onCompleted: profile[useS_manualChanged].connect(function () { item.value = profile.useS_manual})
        text: "ololo"
    }

    Repeater {
        model: 11
        anchors.top: checkbox.bottom
        Row {
            visible: checkbox.checked
            y: checkbox.height + index * (height + 10)

        Label {
            id: label
            text: index / 2 + "="
        }

        SpinBox {
            id: item
            value: my_array[index]
            property int f_index: index
            onValueChanged: {
                if (my_array[index] != item.value) {
                    my_array[index] = item.value
                    profile.s_manual = my_array
                    console.log("lilililil")
                }



               //modelData = 50.0
        //        console.log(index)
       //         console.log(my_array[index])
       //         console.log(profile.s_manual[index])
            }
            Component.onCompleted: arrayChanged.connect(function () { item.value = my_array[index]})
        }}
    }




//        ListModel {
//            id: listModel
//            Component.onCompleted: {
//                for (var i = 0; i < 9; i++) {
//                    listModel.append({"bw": i/2, "xt": 0})
//                }
//            }

//        }

//        TableView {
//            model: listModel
//          TableColumn {

//                role: "bw"
//                title: "bw"
//            }


//                        TableColumn {
//                            role: "xt"
//                            title: "xt"
//                        }

//        }
}
