import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Weekeewachee")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        focus: true
        Keys.onPressed: {
            if (event.key === Qt.Key_R && !event.isAutoRepeat)
            {
                GLForm.myGLItem.spielNeustarten();
                event.accepted = true;
            }
        }

        GLPage {
            mouseArea.onPressed: myGLItem.mousePressed(mouse.x, mouse.y, mouse.button)
            mouseArea.onReleased:myGLItem.mouseReleased(mouse.x, mouse.y, mouse.button)
            mouseArea.onPositionChanged:myGLItem.mouseMoved(mouse.x, mouse.y, mouse.button)
            button.onClicked: myGLItem.spielNeustarten()
        }
    }
}
