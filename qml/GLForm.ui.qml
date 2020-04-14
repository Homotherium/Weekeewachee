import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import GlComponents 1.0

Item {
    id: element
    width: 600
    height: 400
    property alias mouseArea: mouseArea
    property alias myGLItem: myGLItem
    property alias viewportX: myGLItem.viewportX
    property alias viewportY: myGLItem.viewportY

    MyGLItem {
        id: myGLItem
        anchors.fill: parent
        onTextChanged: label.text = text
        onTextColorChanged: label.color = color
        onTextBackgroundColorChanged: label.background.color = color
        onErrorMessage: image.visible = visibility
        onErrorText: error.text = text

        MouseArea
        {
            id: mouseArea
            anchors.fill: parent
        }

        Label {
            id: label
            x: 234
            width: 150
            height: 20
            text: "Let's fight begin!!!!"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: -200
            horizontalAlignment: Text.AlignHCenter
            background: Rectangle {
                color: "yellow"
            }

        }

        Image {
            id: image
            width: 70
            height: 70
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            fillMode: Image.PreserveAspectFit
            visible: false
            source: "qrc:/images/alarm.png"
        }

        Label {
            id: error
            width: 100
            text: qsTr("")
            lineHeight: 1.5
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.top: parent.top
            anchors.topMargin: 85
            anchors.right: parent.right
            anchors.rightMargin: 10
        }

    }
}
