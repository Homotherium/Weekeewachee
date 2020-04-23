#include "myglitem.h"
#include <QQuickWindow>
#include "src/gldefines.h"
#include "src/glmouseray.h"
#include <QTime>
#include <QVector3D>
#include <QtMath>
#include <qelapsedtimer.h>
#include "QThread"
#include <math.h>

//MyGLItem::MyGLItem(QQuickItem *parent) : GLItem(parent)
MyGLItem::MyGLItem() : GLItem()
{
    m_backgroundColor = GLColorRgba::clWhite;
    m_drawAxes = false;
    m_movementEnabled = true;
    m_sphere = new GLSphere(3,50,50);
    m_eye = QVector3D(0.0, 1.0, 1.0)* 17.0;
    m_rotationIncrement = 1.0f;
    m_lightingEnabled = true;
    m_colorArrayEnabled = true;
    m_mouseRay = new GLMouseRay();
    m_mouseEventProcessed = false;
    m_lastMouseEvent = nullptr;

    m_mouseRaySet = false;
    m_animationStep = 0;
    m_totalAnimationSteps = 0;
    m_animationActive = false;
    m_movementEnabled = false;
    m_timer->start();

    m_disc_white_stein = new GLDisc("stein",QVector3D(-1.5, 0.0, -7.5));
    m_disc_white_schere = new GLDisc("schere", QVector3D(1.5, 0.0, -7.5));
    m_disc_white_papier = new GLDisc("papier",QVector3D(-4.5, 0.0, -7.5));
    m_disc_white_brunnen = new GLDisc("brunnen",QVector3D(-4.5, 0.0, -7.5));

    m_disc_black_stein = new GLDisc("stein",QVector3D(-4.5, 0.0, 7.5));
    m_disc_black_schere = new GLDisc("schere",QVector3D(-1.5, 0.0, 7.5));
    m_disc_black_papier = new GLDisc("papier",QVector3D(1.5, 0.0, 7.5));
    m_disc_black_brunnen = new GLDisc("brunnen",QVector3D(4.5, 0.0, 7.5));

    m_whitedisks_list.append(m_disc_white_stein);
    m_whitedisks_list.append(m_disc_white_schere);
    m_whitedisks_list.append(m_disc_white_papier);
    m_whitedisks_list.append(m_disc_white_brunnen);

    m_blackdisks_list.append(m_disc_black_stein);
    m_blackdisks_list.append(m_disc_black_schere);
    m_blackdisks_list.append(m_disc_black_papier);
    m_blackdisks_list.append(m_disc_black_brunnen);

    m_blackPos.append(QVector3D(-1.5f, 0.0f, -7.5f));
    m_blackPos.append(QVector3D(1.5f, 0.0f, -7.5f));
    m_blackPos.append(QVector3D(4.5f, 0.0f, -7.5f));
    m_blackPos.append(QVector3D(-4.5f, 0.0f, -7.5f));

    m_whitePos.append(QVector3D(-4.5f, 0.0f, 7.5f));
    m_whitePos.append(QVector3D(-1.5f, 0.0f, 7.5f));
    m_whitePos.append(QVector3D(1.5f, 0.0f, 7.5f));
    m_whitePos.append(QVector3D(4.5f, 0.0f, 7.5f));

    m_disc = new GLDisc("My disc", QVector3D(0.0, 0.0,0.0));
    m_disc_other = new GLDisc("My disc Other", QVector3D(100.0, 0.0, 100.0));
    m_disc_temp = m_disc_other;

    m_field = new GLField();
    setPlayer(true);
    setIsMoveCorrect(true);
    m_sounds = new music(this);
    m_sounds->setEnabled(true);
    newgame = true;
    alarmtimer = new QTimer(this);
    connect(alarmtimer, SIGNAL(timeout()), this, SLOT(alarmOff()));
    rotatetimer = new QTimer(this);
    connect(rotatetimer, SIGNAL(timeout()), this, SLOT(update()));
}

void MyGLItem::paintUnderQmlScene()
{
    m_field->draw(renderer());
}

void MyGLItem::paintOnTopOfQmlScene()
{
    m_disc_black_stein->draw(renderer());
    m_disc_black_schere->draw(renderer());
    m_disc_black_papier->draw(renderer());
    m_disc_black_brunnen->draw(renderer());

    m_disc_white_stein->draw(renderer());
    m_disc_white_schere->draw(renderer());
    m_disc_white_papier->draw(renderer());
    m_disc_white_brunnen->draw(renderer());

    m_disc_other->draw(renderer());
}


void MyGLItem::setupGeometry()
{
    GLItem::setupGeometry();
    //setup vertexbuffer
    //    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    //    if(!m_vertexBuffer->create())
    //    {
    //        qDebug() << "MIST!!!!!!!!";
    //        exit(1);
    //    }
    if(newgame){
        m_field->setTextureFile(":/textures/sbrett.png");

        m_disc_white_schere->setTextureFile(":/textures/Stein_weiss_schere.png");
        m_disc_white_stein->setTextureFile(":/textures/Stein_weiss_stein.png");
        m_disc_white_papier->setTextureFile(":/textures/Stein_weiss_papier.png");
        m_disc_white_brunnen->setTextureFile(":/textures/Stein_weiss_brunnen.png");

        m_disc_black_stein->setTextureFile(":/textures/Stein_Schwarz_stein.png");
        m_disc_black_schere->setTextureFile(":/textures/Stein_Schwarz_schere.png");
        m_disc_black_papier->setTextureFile(":/textures/Stein_Schwarz_papier.png");
        m_disc_black_brunnen->setTextureFile(":/textures/Stein_Schwarz_brunnen.png");
    }
    newgame = false;

    m_disc_white_schere->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_white_stein->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_white_papier->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_white_brunnen->readBinaryModelFile(":/models/Stein_weiss1.dat");

    m_disc_black_stein->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_black_schere->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_black_papier->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_black_brunnen->readBinaryModelFile(":/models/Stein_weiss1.dat");

    //    m_disc_black_schere->move(QVector3D(-4.5f, 0.0f, -7.5f));
    //    m_disc_black_stein->move(QVector3D(-1.5f, 0.0f, -7.5f));
    //    m_disc_black_papier->move(QVector3D(1.5f, 0.0f, -7.5f));
    //    m_disc_black_brunnen->move(QVector3D(4.5f, 0.0f, -7.5f));

    //    m_disc_white_brunnen->move(QVector3D(-4.5f, 0.0f, 7.5f));
    //    m_disc_white_papier->move(QVector3D(-1.5f, 0.0f, 7.5f));
    //    m_disc_white_stein->move(QVector3D(1.5f, 0.0f, 7.5f));
    //    m_disc_white_schere->move(QVector3D(4.5f, 0.0f, 7.5f));

    m_disc_black_schere->setDisc_Color("black");
    m_disc_black_stein->setDisc_Color("black");
    m_disc_black_papier->setDisc_Color("black");
    m_disc_black_brunnen->setDisc_Color("black");

    m_disc_white_brunnen->setDisc_Color("white");
    m_disc_white_papier->setDisc_Color("white");
    m_disc_white_stein->setDisc_Color("white");
    m_disc_white_schere->setDisc_Color("white");

    //    m_disc_black_schere->setDisc_Coordinates(QVector3D(-4.5f, 0.0f, -7.5f));
    //    m_disc_black_stein->setDisc_Coordinates(QVector3D(-1.5f, 0.0f, -7.5f));
    //    m_disc_black_papier->setDisc_Coordinates(QVector3D(1.5f, 0.0f, -7.5f));
    //    m_disc_black_brunnen->setDisc_Coordinates(QVector3D(4.5f, 0.0f, -7.5f));

    //    m_disc_white_brunnen->setDisc_Coordinates(QVector3D(-4.5f, 0.0f, 7.5f));
    //    m_disc_white_papier->setDisc_Coordinates(QVector3D(-1.5f, 0.0f, 7.5f));
    //    m_disc_white_stein->setDisc_Coordinates(QVector3D(1.5f, 0.0f, 7.5f));
    //    m_disc_white_schere->setDisc_Coordinates(QVector3D(4.5f, 0.0f, 7.5f));

    // Set Disks
    setDisks();

    // Fake disk
    m_disc_other->setTextureFile(":/textures/Stein_schwarz.png");
    m_disc_other->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_other->move(QVector3D(100.0f, 0.0f, 100.0f));
    m_disc_other->setHoldCoordinates(QVector3D(100.0f, 0.0f, 100.0f));
}

bool MyGLItem::kampf(GLDisc *disk)
{
    if (disk->getDXZ() == disk->getDXZ_temp()){
        return false;
    }
    QList<GLDisc*> f_disks_list;
    QList<GLDisc*> e_disks_list;
    bool figth = false;
    // Listen
    if (disk->getDisc_Color() == "black"){
        f_disks_list = m_blackdisks_list;
        e_disks_list = m_whitedisks_list;
    } else {
        f_disks_list = m_whitedisks_list;
        e_disks_list = m_blackdisks_list;
    }
    // Kampf
    qDebug() << "Kampf";
    for (int i = 0; i < e_disks_list.size(); i++) {
        if(e_disks_list[i]->getHoldCoordinates() == disk->getMoveCoordinates() && disk->isFigth(e_disks_list[i]->getDXZ()))
        {
            if(disk->getDisc_Name() == "stein" && e_disks_list[i]->getDisc_Name() == "schere"){
                qDebug() << "stein gegen schere";
                move_away(e_disks_list[i]);
                e_disks_list = deleteDiskFromList(e_disks_list, e_disks_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "stein" && e_disks_list[i]->getDisc_Name() == "papier"){
                qDebug() << "stein gegen papier";
                move_away(disk);
                f_disks_list = deleteDiskFromList(f_disks_list, disk->getDisc_Name());
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "stein" && e_disks_list[i]->getDisc_Name() == "brunnen"){
                qDebug() << "stein gegen brunnen";
                move_away(disk);
                f_disks_list = deleteDiskFromList(f_disks_list, disk->getDisc_Name());
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "schere" && e_disks_list[i]->getDisc_Name() == "stein"){
                qDebug() << "schere gegen stein";
                move_away(disk);
                f_disks_list = deleteDiskFromList(f_disks_list, disk->getDisc_Name());
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "schere" && e_disks_list[i]->getDisc_Name() == "papier"){
                qDebug() << "schere gegen papier";
                move_away(e_disks_list[i]);
                e_disks_list = deleteDiskFromList(e_disks_list, e_disks_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "schere" && e_disks_list[i]->getDisc_Name() == "brunnen"){
                qDebug() << "schere gegen brunnen";
                move_away(disk);
                f_disks_list = deleteDiskFromList(f_disks_list, disk->getDisc_Name());
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "papier" && e_disks_list[i]->getDisc_Name() == "stein"){
                qDebug() << "papier gegen stein";
                move_away(e_disks_list[i]);
                e_disks_list = deleteDiskFromList(e_disks_list, e_disks_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "papier" && e_disks_list[i]->getDisc_Name() == "schere"){
                qDebug() << "papier gegen schere";
                move_away(disk);
                f_disks_list = deleteDiskFromList(f_disks_list, disk->getDisc_Name());
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "papier" && e_disks_list[i]->getDisc_Name() == "brunnen"){
                qDebug() << "papier gegen brunnen";
                move_away(e_disks_list[i]);
                e_disks_list = deleteDiskFromList(e_disks_list, e_disks_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "brunnen" && e_disks_list[i]->getDisc_Name() == "stein"){
                qDebug() << "brunnen gegen stein";
                move_away(e_disks_list[i]);
                e_disks_list = deleteDiskFromList(e_disks_list, e_disks_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "brunnen" && e_disks_list[i]->getDisc_Name() == "schere"){
                qDebug() << "brunnen gegen schere";
                move_away(e_disks_list[i]);
                e_disks_list = deleteDiskFromList(e_disks_list, e_disks_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "brunnen" && e_disks_list[i]->getDisc_Name() == "papier"){
                qDebug() << "brunnen gegen papier";
                move_away(disk);
                f_disks_list = deleteDiskFromList(f_disks_list, disk->getDisc_Name());
                figth = true;
                break;
            }
        }
    }
    // Liste aktualisieren
    if (figth) {
        if (disk->getDisc_Color() == "black"){
            qDebug() << "Liste aktualisieren if";
            m_whitedisks_list = e_disks_list;
            m_blackdisks_list = f_disks_list;
        } else {
            qDebug() << "Liste aktualisieren else";
            m_blackdisks_list = e_disks_list;
            m_whitedisks_list = f_disks_list;
        }
    }
    // set Moved
    disk->setIsMoved(false);
    // Koordinaten setzen
    qDebug() << "Disc HoldCoordinates: " << m_disc->getHoldCoordinates();
    qDebug() << "Disc MoveCoordinates: " << m_disc->getMoveCoordinates();
    disk->setHoldCoordinates(m_disc->getMoveCoordinates());
    // Update XZ
    disk->setStepVector(QVector3D(0.0f, 0.0f, 0.0f));
    disk->updateXZ();
    turnEnd();
    return true;
}

void MyGLItem::moving(GLDisc * disk, QVector3D MousePos)
{
    if (isMoveCorrect){
        alarmOff();
        QList<GLDisc*> frends_list;
        QList<GLDisc*> enemy_list;
        if (disk->getDisc_Color() == "black"){
            frends_list = m_blackdisks_list;
            enemy_list = m_whitedisks_list;
        } else {
            frends_list = m_whitedisks_list;
            enemy_list = m_blackdisks_list;
        }

        float mouse_x = MousePos.x();
        float mouse_z = MousePos.z();
        qDebug() << "mouse_x: " << mouse_x;
        qDebug() << "mouse_z: " << mouse_z;
        QString buch = QString(disk->getDXZ_temp()[0]);
        QString zahl = QString(disk->getDXZ_temp()[1]);
        // Out of Range
        if (mouse_x > 6.0f || mouse_x < -6.0f || mouse_z > 9.0f || mouse_z < -9.0f){
            //        qDebug() << "Out of Range, new disc_Coordinates: " << m_disc->getDisc_Coordinates();
            showErrorMesage("Out of Range!");
            m_sounds->playSound(":/music/when.wav");
        }
        // X-Werte
        if (mouse_x > -5.9f && mouse_x < -3.1f){
            buch = "A";
        }
        if (mouse_x > -2.9f && mouse_x < -0.1f){
            buch = "B";
        }
        if (mouse_x > 0.1f && mouse_x < 2.9f){
            buch = "C";
        }
        if (mouse_x > 3.1f && mouse_x < 5.9f){
            buch = "D";
        }
        // Z-Werte
        if (mouse_z > 6.1f && mouse_z < 8.9f) {
            zahl = "1";
        }
        if (mouse_z > 3.1f && mouse_z < 5.9f) {
            zahl = "2";
        }
        if (mouse_z > 0.1f && mouse_z < 2.9f) {
            zahl = "3";
        }
        if (mouse_z > -2.9f && mouse_z < -0.1f) {
            zahl = "4";
        }
        if (mouse_z > -5.9f && mouse_z < -3.1f) {
            zahl = "5";
        }
        if (mouse_z > -8.9f && mouse_z < -6.1f) {
            zahl = "6";
        }
        if ((disk->getDXZ_temp() != buch+zahl) && !besetzt(disk->getDXZ(), buch+zahl, disk->getDisc_Name(), frends_list, enemy_list)){
            qDebug() << "StartCoord: " << disk->getDXZ();
            qDebug() << "Coord: " << buch+zahl;
            if (disk->isMovementOk(buch+zahl)){
                if(disk->isMoved()){
                   disk->backStep();
                }
                qDebug() << disk->getList();
                QVector3D moveDisk = disk->getVector(disk->getList(), buch+zahl);
                qDebug() << moveDisk;
                qDebug() << "Move Coor: " << disk->getMoveCoordinates();
                disk->setMoveCoordinates(disk->getHoldCoordinates() + moveDisk);
                qDebug() << "Moved to: " << disk->getMoveCoordinates();
                disk->move(moveDisk);
                disk->setDXZ_temp(buch+zahl);
                m_sounds->playSound(":/music/clearly.wav");
                disk->setIsMoved(true);
            }
        }
    }
}

void MyGLItem::move_away(GLDisc *disk)
{
    disk->move(QVector3D(+100.0f, 0.0f, +100.0f));
    disk->setHoldCoordinates(disk->getHoldCoordinates() + QVector3D(+100.0f, 0.0f, +100.0f));
    m_sounds->playSound(":/music/Blop.wav");
    qDebug() << "Disk " << disk->getDisc_Color() << " " << disk->getDisc_Name() << " ist gelöscht";
}

QList<GLDisc *> MyGLItem::deleteDiskFromList(QList<GLDisc *> m_disks_list, QString disk_name)
{
    for (int i = 0; i < m_disks_list.size(); i++) {
        if (m_disks_list[i]->getDisc_Name() == disk_name){
            m_disks_list.removeAt(i);
        }
    }
    return m_disks_list;
}

void MyGLItem::changePlayer(bool player)
{
    setPlayer(!player);
}

void MyGLItem::mouseReleased(int x, int y, int button)
{
    if(m_lastMouseEvent)
        delete m_lastMouseEvent;
    m_lastMouseEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease, QPointF(x, y), QPointF(x, y),
                                       static_cast<Qt::MouseButton>(button),static_cast<Qt::MouseButton>(button), Qt::NoModifier);
    m_lastMouseEvent->setAccepted(false);

    if(window())
        window()->update();

    m_disc->setSelected(false);
    //    // Out of Range Test
    //    QVector3D end;
    //    renderer()->mouseIntersection(&end, v_Y, 0.0f, m_lastMouseEvent->pos());
    //    float end_x = end.x();
    //    float end_z = end.z();
    //    if (end_x > 6.0f || end_x < -6.0f || end_z > 9.0f || end_z < -9.0f){
    //        qDebug() << "Out of Range, new disc_Coordinates: " << m_disc->getDisc_Coordinates();
    //        showErrorMesage("Out of Range!");
    //        m_disc->draw(renderer());
    //        update();
    //        m_sounds->playSound(":/music/when.wav");
    //        setIsMoveCorrect(false);
    //    }
    //    // Weit geclickt
    //    else if (!isFar(m_disc->getDisc_Coordinates(), end)){
    //        qDebug() << "Out of Range, weit geclickt!";
    //        showErrorMesage("Zu weit geklickt!");
    //        m_sounds->playSound(":/music/when.wav");
    //        setIsMoveCorrect(false);
    //    }
    //    // Nah geclickt
    //    else if (!isNear(m_disc->getDisc_Coordinates(), end)){
    //        qDebug() << "Out of Range, nah geclickt!";
    //        showErrorMesage("Zu nah geklickt!");
    //        m_sounds->playSound(":/music/when.wav");
    //        setIsMoveCorrect(false);
    //    }
    QVector3D movePos;
    renderer()->mouseIntersection(&movePos, v_Y, 0.0f, m_lastMouseEvent->pos());
    QVector3D moveDistance = movePos + m_pressPosToDiscPos;

    m_disc->setFieldCoord(moveDistance);
}

void MyGLItem::mouseMoved(int x, int y, int button)
{

    if(m_lastMouseEvent)
        delete m_lastMouseEvent;
    m_lastMouseEvent = new QMouseEvent(QMouseEvent::MouseMove, QPointF(x, y), QPointF(x, y),
                                       static_cast<Qt::MouseButton>(button),static_cast<Qt::MouseButton>(button), Qt::NoModifier);
    m_lastMouseEvent->setAccepted(false);

    if(window())
        window()->update();

    //    QVector3D movePos;
    //    renderer()->mouseIntersection(&movePos, v_Y, 0.0f, m_lastMouseEvent->pos());
    //    QVector3D moveDistance = movePos + m_pressPosToDiscPos;
    //    QMatrix4x4 m;

    //        if(m_disc->getFieldCoord().x() > movePos.x()){
    //            //m.translate(moveDistance);
    //            //m_disc->setFieldCoord(moveDistance);
    //            m_disc->setTransformation(m);
    //            //qDebug() << "Mouse move event Begrenzung erreicht";
    //        }


    //qDebug() << " mouse moved" << "x wert: " << x << "y Wert: " << y;
}

void MyGLItem::mousePressed(int x, int y, int button)
{
    alarmOff();
    if(m_lastMouseEvent)
        delete m_lastMouseEvent;
    m_lastMouseEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, QPointF(x, y), QPointF(x, y),
                                       static_cast<Qt::MouseButton>(button),static_cast<Qt::MouseButton>(button), Qt::NoModifier);
    m_lastMouseEvent->setAccepted(false);

    if(window())
        window()->update();

    QList<GLDisc*> disks_list;
    if (player){
        disks_list = m_blackdisks_list;
    } else {
        disks_list = m_whitedisks_list;
    }

    setIsMoveCorrect(true);
    // Ob Spiler richtige Farbe trifft
    for (int d = 0; d < disks_list.size(); d++) {
        if(disks_list[d]->isHit(QPoint(x,y),renderer())){
            qDebug() << "Spieler darf diese Fabre nicht bewegen!!!!!!!!!!!";
            showErrorMesage("Falsche Farbe");
            setIsMoveCorrect(false);
        }
    }
    for (int w = 0; w < m_whitedisks_list.size(); w++) {
        if(m_whitedisks_list[w]->isHit(QPoint(x,y),renderer())){
            m_disc = m_whitedisks_list[w];
        }
    }
    for (int b = 0; b < m_blackdisks_list.size(); b++) {
        if(m_blackdisks_list[b]->isHit(QPoint(x,y),renderer())){
            m_disc = m_blackdisks_list[b];
        }
    }
}

void MyGLItem::doSynchronizeThreads()
{
    GLItem::doSynchronizeThreads();
    setupRenderer();

    //Mouse pressed
    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseButtonPress) && !m_lastMouseEvent->isAccepted()){
        m_lastMouseEvent->setAccepted(true);
        QVector3D nearPoint;
        QVector3D farPoint;
        if( m_lastMouseEvent){
            renderer()->calculateMousePoints(&nearPoint, &farPoint, m_lastMouseEvent->pos());
        }
        if(m_disc->isHit(nearPoint, farPoint)){
            m_disc->setSelected(true);
        }
        m_disc->setIsMoved(false);
        m_disc->setMoveCoordinates(m_disc->getHoldCoordinates());
        m_disc_temp = m_disc;
        m_disc_temp->jumpUp();
        //m_disc->setTempCoordinates(QVector3D(0.0f, 0.0f,0.0f));
        m_totalAnimationSteps = 10;
        m_animationActive = true;
        m_lastMouseEvent->setAccepted(true);
    }

    //mouse move
    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseMove) && !m_lastMouseEvent->isAccepted()){
        m_lastMouseEvent->setAccepted(true);
        if (m_disc->getFinalLiftVector().y() == 1.0f){
            renderer()->mouseIntersection(&diskPosition, v_Y, 0.0f, m_lastMouseEvent->pos());
            //qDebug() << "MouseVector" << diskPosition;
            moving(m_disc, diskPosition);
            update();
        }
    }

    //mouse release
    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseButtonRelease) && !m_lastMouseEvent->isAccepted()){
        alarmOff();
        m_disc->setSelected(false);
        //        QVector3D end;
        //        renderer()->mouseIntersection(&end, v_Y, 0.0f, m_lastMouseEvent->pos());
        qDebug() << "Kampf Überprüfung";
        //        kampf(m_disc, end, m_lastMouseEvent->pos());
        kampf(m_disc);
        qDebug() << "";
        m_disc_temp->jumpDown();
        m_totalAnimationSteps = 10;
        m_animationActive = true;
        m_lastMouseEvent->setAccepted(true);
    }

    //Animation
    if(m_animationActive){
        if(m_animationStep < m_totalAnimationSteps){
            m_animationStep++;
            //m_disc->updateAnimatedProperties(static_cast<float>(m_animationStep) / static_cast<float>(m_totalAnimationSteps) );
            m_disc_temp->updateAnimatedProperties(static_cast<float>(m_animationStep) / static_cast<float>(m_totalAnimationSteps) );
        }else{
            //m_disc->finishAnimation();
            m_disc_temp->finishAnimation();
            m_animationStep = 0;
            m_animationActive = false;
        }
    }
}

void MyGLItem::setupBuffers()
{
    //setup vertexbuffer
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if(!m_vertexBuffer->create()){
        exit(1);
    }
    m_vertexBuffer->bind();
    //  m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw); // StaticDraw is default. However we set it for clearity.
    m_vertexBuffer->allocate(m_points.data(), m_points.size() * static_cast<int>(sizeof(GLPoint)) );
    m_vertexBuffer->release();

    //setup indexbuffer
    m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    if(!m_indexBuffer->create()){
        exit(1);
    }
    m_indexBuffer->bind();
    m_indexBuffer->allocate(m_indices.data(), m_indices.size() * static_cast<int>(sizeof(IndexType)));
    m_indexBuffer->release();

}

bool MyGLItem::getIsMoveCorrect() const
{
    return isMoveCorrect;
}

void MyGLItem::setIsMoveCorrect(bool value)
{
    isMoveCorrect = value;
}

void MyGLItem::collisionKampf(GLDisc * disc1, GLDisc * disc2)
{
    QString disk1Color = disc1->getDisc_Color();
    QString disk2Color = disc2->getDisc_Color();


    if(disc1->getDisc_Name() == "stein" && disc2->getDisc_Name() == "schere"){
        qDebug() << "stein gegen schere";
        move_away(disc2);
        if (disk2Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "schere");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "schere");
        }
    }
    if(disc1->getDisc_Name() == "stein" && disc2->getDisc_Name() == "papier"){
        qDebug() << "stein gegen papier";
        move_away(disc1);
        if (disk1Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "stein");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "stein");
        }
    }
    if(disc1->getDisc_Name() == "stein" && disc2->getDisc_Name() == "brunnen"){
        qDebug() << "stein gegen brunnen";
        move_away(disc1);
        if (disk1Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "stein");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "stein");
        }
    }
    if(disc1->getDisc_Name() == "schere" && disc2->getDisc_Name() == "stein"){
        qDebug() << "schere gegen stein";
        move_away(disc1);
        if (disk1Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "schere");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "schere");
        }
    }
    if(disc1->getDisc_Name() == "schere" && disc2->getDisc_Name() == "papier"){
        qDebug() << "schere gegen papier";
        move_away(disc2);
        if (disk2Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "papier");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "papier");
        }
    }
    if(disc1->getDisc_Name() == "schere" && disc2->getDisc_Name() == "brunnen"){
        qDebug() << "schere gegen brunnen";
        move_away(disc1);
        if (disk1Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "schere");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "schere");
        }
    }
    if(disc1->getDisc_Name() == "papier" && disc2->getDisc_Name() == "stein"){
        qDebug() << "papier gegen stein";
        move_away(disc2);
        if (disk2Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "stein");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "stein");
        }
    }
    if(disc1->getDisc_Name() == "papier" && disc2->getDisc_Name() == "schere"){
        qDebug() << "papier gegen schere";
        move_away(disc2);
        if (disk2Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "schere");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "schere");
        }
    }
    if(disc1->getDisc_Name() == "papier" && disc2->getDisc_Name() == "brunnen"){
        qDebug() << "papier gegen brunnen";
        move_away(disc2);
        if (disk2Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "brunnen");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "brunnen");
        }
    }
    if(disc1->getDisc_Name() == "brunnen" && disc2->getDisc_Name() == "stein"){
        qDebug() << "brunnen gegen stein";
        move_away(disc2);
        if (disk2Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "stein");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "stein");
        }
    }
    if(disc1->getDisc_Name() == "brunnen" && disc2->getDisc_Name() == "schere"){
        qDebug() << "brunnen gegen schere";
        move_away(disc2);
        if (disk2Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "schere");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "schere");
        }
    }
    if(disc1->getDisc_Name() == "brunnen" && disc2->getDisc_Name() == "papier"){
        qDebug() << "brunnen gegen papier";
        move_away(disc1);
        if (disk1Color == "black"){
            m_blackdisks_list = deleteDiskFromList(m_blackdisks_list, "brunnen");
        } else {
            m_whitedisks_list = deleteDiskFromList(m_whitedisks_list, "brunnen");
        }
    }
}

bool MyGLItem::gameOverTest()
{
    for (int b = 0; b < m_blackdisks_list.size(); b++) {
        for (int i = 0; i < 4; i++) {
            if (m_blackdisks_list[b]->getMoveCoordinates() == m_whitePos[i]){
                qDebug() << "Black Player wins!";
                m_sounds->playSound(":/music/applauses.wav");
                return true;
            }
        }
    }
    for (int w = 0; w < m_whitedisks_list.size(); w++) {
        for (int j = 0; j < 4; j++) {
            if (m_whitedisks_list[w]->getMoveCoordinates() == m_blackPos[j]){
                qDebug() << "White Player wins!";
                m_sounds->playSound(":/music/applauses.wav");
                return true;
            }
        }
    }
    if (m_whitedisks_list.isEmpty()){
        qDebug() << "Black Player wins!";
        m_sounds->playSound(":/music/applauses.wav");
        return true;
    }
    if (m_blackdisks_list.isEmpty()){
        qDebug() << "White Player wins!";
        m_sounds->playSound(":/music/applauses.wav");
        return true;
    }
    return false;
}

bool MyGLItem::diskCollision(GLDisc *disk)
{
    qDebug() << "diskCollision test";
    if (disk->getDisc_Color() == "black"){
        for (int i = 0; i < m_whitedisks_list.size(); i++) {
            if(disk->getHoldCoordinates() == m_whitedisks_list[i]->getHoldCoordinates())
            {
                if (disk->getDisc_Name() == m_whitedisks_list[i]->getDisc_Name()){
                    qDebug() << "Schwarz-Weiss gleiche Disks";
                    return true;
                }
                collisionKampf(disk, m_whitedisks_list[i]);
                qDebug() << "Schwarz-Weiss collisionKampf";
                return false;
            }
        }
        for (int i = 0; i < m_blackdisks_list.size(); i++) {
            if(disk->getHoldCoordinates() == m_blackdisks_list[i]->getHoldCoordinates() && disk->getDisc_Name() !=  m_blackdisks_list[i]->getDisc_Name())
            {
                qDebug() << "Schwarz-Schwarz diskCollision";
                return true;
            }
        }
    } else {
        for (int i = 0; i < m_whitedisks_list.size(); i++) {
            if(disk->getHoldCoordinates() == m_whitedisks_list[i]->getHoldCoordinates() && disk->getDisc_Name() !=  m_whitedisks_list[i]->getDisc_Name())
            {
                qDebug() << "Weiss-Weiss diskCollision";
                return true;
            }
        }
        for (int i = 0; i < m_blackdisks_list.size(); i++) {
            if(disk->getHoldCoordinates() == m_blackdisks_list[i]->getHoldCoordinates())
            {
                if (disk->getDisc_Name() == m_blackdisks_list[i]->getDisc_Name()){
                    qDebug() << "Weiss-Schwarz gleiche Disks";
                    return true;
                }
                collisionKampf(disk, m_blackdisks_list[i]);
                qDebug() << "Weiss-Schwarz collisionKampf";
                return false;
            }
        }
    }
    qDebug() << "Shit!!!";
    return false;
}

void MyGLItem::rotateBoard()
{
    QVector3D factor = QVector3D(1.0f, 1.0f, -1.0f);
    qDebug() << "Seiten wechseln";
    if (player){
        emit textChanged("White Player turn");
        emit textColorChanged("white");
        emit textBackgroundColorChanged("black");
        qDebug() << "White Player turn";
    } else {
        emit textChanged("Black Player turn");
        emit textColorChanged("black");
        emit textBackgroundColorChanged("white");
        qDebug() << "Black Player turn";
    }
    //    QVector3D temp = m_eye * factor;
    //    rotatetimer->moveToThread(this->thread());
    //    while(m_eye != temp){
    //        if(temp.z() < 0){
    //            m_eye.setZ(m_eye.z() - 1.0f);
    //        } else {
    //            m_eye.setZ(m_eye.z() + 1.0f);
    //        }
    //        rotatetimer->start(1000);
    //    }
    m_eye = m_eye * factor;
    update();
}

void MyGLItem::turnEnd()
{
    if (!gameOverTest()){
        // Disk Collision
        if(diskCollision(m_disc)){
            qDebug() << "Disk Collision!!!!!!!!!!!!!";
            m_disc->backStep();
            m_sounds->playSound(":/music/when.wav");
        }
        // Actual disk to Fake disk
        m_disc = m_disc_other;
        // Spieler wechsel
        changePlayer(player);
        // Board umdrehen
        rotateBoard();
    }  else {
        spielNeustarten();
    }
}

void MyGLItem::printDiskLists()
{
    // Print Black List
    for (int b = 0; b < m_blackdisks_list.size(); b++) {
        qDebug() << m_blackdisks_list[b]->getDisc_Name() << m_blackdisks_list[b]->getDisc_Color() << m_blackdisks_list[b]->getHoldCoordinates();
    }
    // Print White List
    for (int w = 0; w < m_whitedisks_list.size(); w++) {
        qDebug() << m_whitedisks_list[w]->getDisc_Name() << m_whitedisks_list[w]->getDisc_Color() << m_whitedisks_list[w]->getHoldCoordinates();
    }
}

void MyGLItem::showErrorMesage(QString text)
{
    int alarmTime = 5000;
    qDebug() << "Alarm";
    emit errorIcon(true);
    emit errorMessage(text);
    alarmtimer->start(alarmTime);
}

void MyGLItem::spielNeustarten()
{
    qDebug() << "Spiel neustarten";
    // Liste löschen
    m_whitedisks_list.clear();
    m_blackdisks_list.clear();
    // Spiel neustarten
    setPlayer(true);
    setIsMoveCorrect(true);
    m_eye = QVector3D(0.0, 1.0, 1.0)* 17.0;
    emit textChanged("Let's fight begin!!!!");
    emit textColorChanged("black");
    emit textBackgroundColorChanged("yellow");
    qDebug() << "Liste";
    m_whitedisks_list.append(m_disc_white_stein);
    m_whitedisks_list.append(m_disc_white_schere);
    m_whitedisks_list.append(m_disc_white_papier);
    m_whitedisks_list.append(m_disc_white_brunnen);

    m_blackdisks_list.append(m_disc_black_stein);
    m_blackdisks_list.append(m_disc_black_schere);
    m_blackdisks_list.append(m_disc_black_papier);
    m_blackdisks_list.append(m_disc_black_brunnen);
    qDebug() << "setupGeometry";
    setupGeometry();
    qDebug() << "Update";
    update();
    qDebug() << "End";
}

bool MyGLItem::besetzt(QString start, QString zelle, QString disk_name, QList<GLDisc*> frends_list, QList<GLDisc*> enemy_list)
{
    qDebug() << "BesetzBlock";
    qDebug() << "Start XZ: " << start;
    qDebug() << "Mouse XZ: " << zelle;
    qDebug() << "Disk name: " << disk_name;
    QString stein = "";
    for (int i = 0; i < frends_list.size(); i++) {
        stein = frends_list[i]->getDXZ();
        if (stein == zelle && stein != start){
            showErrorMesage("Gleiche Farbe!");
            m_sounds->playSound(":/music/when.wav");
            return true;
        }
    }
    for (int i = 0; i < enemy_list.size(); i++) {
        stein = enemy_list[i]->getDXZ();
        if (stein == zelle && enemy_list[i]->getDisc_Name() == disk_name){
            showErrorMesage("Gleiche Stein!");
            m_sounds->playSound(":/music/when.wav");
            return true;
        }
    }
    return false;
}

bool MyGLItem::getPlayer() const
{
    return player;
}

void MyGLItem::setPlayer(bool value)
{
    player = value;
}

void MyGLItem::setDisks()
{
    QList<GLDisc*> blackDisks = m_blackdisks_list;
    QList<QVector3D> blackPositions = m_blackPos;
    QList<GLDisc*> whiteDisks = m_whitedisks_list;
    QList<QVector3D> whitePositions = m_whitePos;
    // Shuffle
    qsrand(uint(time(nullptr)));
    for (int i = 0; i < 3; i++) {
        std::random_shuffle(whitePositions.begin()+i, whitePositions.begin()+4);
        std::random_shuffle(blackPositions.begin() + i, blackPositions.begin() + 4);
    }
    // Schwarze Steine
    for (int b = 0; b < blackDisks.size(); b++) {
        blackDisks[b]->move(blackPositions[b]);
        blackDisks[b]->setHoldCoordinates(blackPositions[b]);
        blackDisks[b]->setMoveCoordinates(blackPositions[b]);
        blackDisks[b]->setXZ();
    }
    // Weise Steine
    for (int w = 0; w < whiteDisks.size(); w++) {
        whiteDisks[w]->move(whitePositions[w]);
        whiteDisks[w]->setHoldCoordinates(whitePositions[w]);
        whiteDisks[w]->setMoveCoordinates(whitePositions[w]);
        whiteDisks[w]->setXZ();
    }
}
