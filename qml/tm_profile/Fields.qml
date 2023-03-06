import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.0

Repeater {
    id: repeater

    RowLayout {
        id: fielditem
        y: index * (height + 10)
        height: label.height
        anchors {
            left: parent.left
            right: parent.right
            margins: 10
        }

        property var labels: {
            "m": "Модуль зацепления",
            "z1": "Число зубьев сателлита",
            "z2": "Число зубьев центрального колеса",
            "x2": "Коэффициент смещения колеса",
            "E": "Межосевой угол",
            "alpha": "Угол профиля",
            "c": "Коэффициент радиального зазора",
            // Параметры зуборезного долбяка
            "z0": "Число зубьев",
            "x0": "Коэффициент смещения",
            "da0": "Диаметр вершин зубьев",

            "bw": "Ширина венца сателлита",
            "W0": "Аппликата большего торцового сечения венца шестерни",
            "ha": "Коэффициент высоты зуба",

            "d0": "Делительный диаметр фрезы",
            "ra2": "Радиус вершин зубьев колеса",
            "rf2": "Радиус впадин колеса"
        }


        Label {
            id: label
            text: labels[[modelData].valueOf(index)] + " " + [modelData].valueOf(index) + " ="
            font.pointSize: 10
        }

        SpinBox {
            id: item
            property var ints: ["m", "z1", "z2", "E", "bw", "d0", "alpha", "z0"]
            Layout.fillWidth: true
            decimals: {
                if (ints.indexOf(modelData) != -1)
                    return 0
                else {
                    item.stepSize = 0.1
                    return 3
                }
            }
            onValueChanged: {
                parametersChanged = true
                profile[modelData] = item.value
            }

            Binding {
                target: item
                property: "value"
                value: profile[modelData]
            }
        }
    }
}
