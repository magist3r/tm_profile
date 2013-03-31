import QtQuick 2.1
import QtQuick.Controls 1.0

Item {
    id: images

    property alias image1: _image1
    property alias image2: _image2

    function updateImages() {
        var parName = parComboBox.currentText
        var modName = modComboBox.currentText

        _image1.source = ""
        _image2.source = ""

        var baseName = profile.dataLocation + '/' + parName
        _image1.source = baseName + '_' + modName + '_1.png'
        _image2.source = baseName + '_' + modName + '_2.png'
    }

    Connections {
        target: imageGenerator
        onImagesGenerated: updateImages()
    }

    Image {
        id: _image1
        cache: false

        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
    }

    Text {
        id: text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: _image1.bottom
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        text: qsTr("This is image #2")
    }

    Image {
        id: _image2
        cache: false

        anchors {
            top: text.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }
}
