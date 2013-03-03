import QtQuick 2.0
import QtDesktop 1.0

Item {
    id: images

   property int margin: (width - _image.width ) / 2
  //  onChildrenRectChanged: margin = (width - _image2.width - _image2.width) / 3





    Image {
        id: _image

//          width: 200
//        height: 150
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: images.margin
            topMargin: 30
        }

        Connections {
            target: imageGenerator

            onImagesGenerated: {
                _image.source = image
            }

        }

        Connections {
            target: profile
            onLoadImage: _image.source = image
        }

        Connections {
            target: mTrTab.checkbox
            onCheckedChanged: _image.source = profile.getFullName()
        }
    }

   /* Image {
        id: _image2
  //      width: 200
    //    height: 150
        anchors {
            top: parent.top
            right: parent.right
            rightMargin: images.margin
            topMargin: 30
        }
        Connections {
            target: imageGenerator
            onImagesGenerated: {
                _image2.source = image2
            }
        }
    }*/

    //width: childrenRect.width

    /*Connections {
        target: imageGenerator
        onImagesGenerated: {
            _image1 = Qt.createComponent("Zone.qml").createObject(images, { "source": image1 } )
            _image2 = Qt.createComponent("Zone.qml").createObject(images, { "source": image2 } )
            _image1.anchors.top = images.top
            _image2.anchors.left = _image1.right

            console.log(image1);

        }

    }*/

  /*  Connections {
        target: calculate
        onCalculating: {Component.onCompleted: profile[modelData + "Changed"].connect(function () { item.value = profile[modelData]})
            _image1.destroy();
            _image2.destroy();
        }

    }*/

}
