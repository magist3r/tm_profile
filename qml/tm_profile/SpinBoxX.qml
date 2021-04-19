import QtQuick 2.12
import QtQuick.Controls 2.12

SpinBox {
    id: spinbox
    property int decimals: 2
    property real stepSizeX: 0.01
    property real fromX: 0
    property real toX: 1

    from: fromX * 10 * decimals
    to: toX * 10 * decimals
    stepSize: stepSizeX * 10 * decimals

    property real realValue: value / (10 * decimals)


    validator: DoubleValidator {
        bottom: Math.min(spinbox.from, spinbox.to)
        top:  Math.max(spinbox.from, spinbox.to)
    }

    textFromValue: function(value, locale) {
        return Number(value / (10 * decimals)).toLocaleString(locale, 'f', spinbox.decimals)
    }

    valueFromText: function(text, locale) {
        return Number.fromLocaleString(locale, text) * 10 * decimals
    }
}
