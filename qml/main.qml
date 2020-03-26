import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Tabs")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

      /*  GLForm {
            id: glForm
                myGLItem.viewportY: tabBar.height
        }*/

        GLPage {
            mouseArea.onPressed: myGLItem.mousePressed(mouse.x, mouse.y, mouse.button)
            mouseArea.onReleased:myGLItem.mouseReleased(mouse.x, mouse.y, mouse.button)
            mouseArea.onPositionChanged:myGLItem.mouseMoved(mouse.x, mouse.y, mouse.button)
            viewportY: tabBar.height
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Page 1")
        }
        TabButton {
            text: qsTr("Page 2")
        }
    }
}
