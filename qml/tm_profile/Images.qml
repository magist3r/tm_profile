import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout {
    id: images
    spacing: 10

    property alias image1: _image1
    property alias image2: _image2

    function updateImages() {
        var parName = parComboBox.currentText
        var modName = modComboBox.currentText

        var baseName = "file:///" + profile.dataLocation + '/' + parName

        _image1.source = ""
        _image2.source = ""

        if (!profile.imageExists(baseName + '_' + modName))
            return

            _image1.source = 'file:///' + baseName + '_' + modName + '_1.png'
            _image2.source = 'file:///' + baseName + '_' + modName + '_2.png'
        }

    Connections {
        target: imageGenerator
        onImagesGenerated: updateImages()
    }

    Text {
        id: text1
        visible: _image1.status == Image.Ready
        /*anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: 10
            }*/
        Layout.fillWidth: true
        Layout.alignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        text: qsTr("Deviation relative to the theoretical profile:")
    }

    Image {
        id: _image1
        cache: false
        //  Layout.fillWidth: true
        Layout.alignment: Qt.AlignHCenter

        /*anchors {
                top: text1.bottom
                margins: 10
                horizontalCenter: parent.horizontalCenter
            }*/
    }

    Text {
        id: text2
        visible: _image2.status == Image.Ready

        Layout.alignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        text: qsTr("Deviation with respect to the maximum point:")
    }

    Image {
        id: _image2
        cache: false
        // Layout.fillWidth: true
        Layout.alignment: Qt.AlignHCenter
    }

    Text {
        id: comment
        visible: _image2.status == Image.Ready
        Layout.fillWidth: true
        Layout.alignment: Text.AlignBottom
        font.pointSize: 7
        text: qsTr("*all deviations in the images are in microns.")
    }
}
