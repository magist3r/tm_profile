import QtQuick 2.0
import QtDesktop 1.0

Item {
    id: images

    property alias image1: _image1
    property alias image2: _image2

    function setImageSource(basename, modname) {
        _image1.source = ""
        _image2.source = ""
        var name = profile.dataLocation + '/' + basename
        if (checkbox.checked) {
            _image1.source = name + '_manual_1.png'
            _image2.source = name + '_manual_2.png'
        } else {
            _image1.source = name + '_' + modname + '_1.png'
            _image2.source = name + '_' + modname + '_2.png'
        }
    }

    Connections {
        target: imageGenerator
        onImagesGenerated: updateSettingsAndImages(true)
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
