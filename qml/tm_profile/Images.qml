import QtQuick 2.0
import QtDesktop 1.0

Item {
    id: images

    function setImageSource() {
        _image1.source = ""
        _image2.source = ""
        var basename = profile.dataLocation + '/' + parComboBox.selectedText
        if (checkbox.checked) {
            _image1.source = basename + '_manual_1.png'
            _image2.source = basename + '_manual_2.png'
        } else {
            _image1.source = basename + '_' + modComboBox.selectedText + '_1.png'
            _image2.source = basename + '_' + modComboBox.selectedText + '_2.png'
        }
    }

    Connections {
        target: imageGenerator
        onImagesGenerated: setImageSource()
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
