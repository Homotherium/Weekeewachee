import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import GlComponents 1.0

Item {
    id: element
    width: 600
    height: 400
    property alias button: button
    property alias mouseArea: mouseArea
    property alias myGLItem: myGLItem
    property alias viewportX: myGLItem.viewportX
    property alias viewportY: myGLItem.viewportY

    MyGLItem {
        id: myGLItem
        anchors.fill: parent

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

        Button {
            id: button
            x: 0
            y: 0
            text: qsTr("Spiel Neustarten")
        }
    }
}
