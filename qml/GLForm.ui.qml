import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import GlComponents 1.0

Item {
    width: 600
    height: 400
    property alias mouseArea: mouseArea
    property alias buttonRight: buttonRight
    property alias buttonLeft: buttonLeft
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

        RowLayout {
            id: rowLayout
            x: 8
            y: 310
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3

            Button {
                id: buttonLeft
                text: qsTr("Rotate left")
            }

            Button {
                id: buttonRight
                text: qsTr("Rotate right")
            }
        }

    }
}
