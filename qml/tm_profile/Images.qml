import QtQuick 2.0
import QtDesktop 1.0

Item {
    id: images
   // property alias image: _image

  //  property int margin: (width - _image.width ) / 2

    function setImageSource() {
        _image1.source = ""
        _image2.source = ""
        var basename = profile.dataLocation + '/' + list.selectedText
        if (checkbox.checked) {
            _image1.source = basename + '_manual_1.png'
            _image2.source = basename + '_manual_2.png'
        } else {
            _image1.source = basename + '_1.png'
            _image2.source = basename + '_2.png'
        }
    }

    Connections {
        target: imageGenerator

        onImagesGenerated: {
            console.log('ololololo')
            setImageSource()
           // _image.update()
        }

    }





    Image {
        id: _image1
        cache: false
      //  antialiasing: true

        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        //    leftMargin: images.margin
       //     topMargin: 30
        }
    }
    Text {
        id: text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: _image1.bottom
      //  anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        text: "dpvmfhjv;lkfhjv;fjh;lvgfmjhl vkdjhmvljmh;lddbjvflg hjv,lkghbmjvlkfbhjkfghvkjdnh gvlkdfhigjvhdfjgt hdkfjghvkjdghvjg hvjnfhgkvjgnkghvkjdh"
    }

    Image {
        id: _image2
        cache: false

        anchors {
            top: text.bottom
            horizontalCenter: parent.horizontalCenter
          //  leftMargin: images.margin
          //  topMargin: 30
        }
    }
}

     /*   states: [
            State {
                name: "clicked"
                PropertyChanges {
                    target: _image
                    source: profile.dataLocation + '/' + mainTab.list.selectedText + '_manual.png'
                }
            }
        ]*/




/*
        Connections {
            target: profile
            onLoadImage: _image.source = image
        }

        Connections {
            target: mTrTab.checkbox
            onCheckedChanged: _image.source = profile.getFullName()
        }
    }*/

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



