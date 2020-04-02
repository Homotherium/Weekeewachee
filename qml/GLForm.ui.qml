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
    property alias label: label

    MyGLItem {
        id: myGLItem
        anchors.fill: parent

        MouseArea
        {
            id: mouseArea
            anchors.fill: parent
        }

        RowLayout {
            id: rowLayout
            x: 8
            y: 310
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
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
        Connections{
            target: myGLItem
            onTextChanged: label.text = text
        }
        Connections{
            target: myGLItem
            onTextColorChanged: label.color = color
        }
        Connections{
            target: myGLItem
            onTextBackgroundColorChanged: label.background.color = color
        }

    }
}

/*##^##
Designer {
    D{i:4;anchors_y:0}
}
##^##*/
