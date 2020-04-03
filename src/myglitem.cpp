#include "myglitem.h"
#include <QQuickWindow>
#include "src/gldefines.h"
#include "src/glmouseray.h"
#include <QVector3D>
#include <QtMath>
#include "QThread"

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

    m_disc_white_stein = new GLDisc("stein",QVector3D(2,0.0,-5));
    m_disc_white_schere = new GLDisc("schere", QVector3D(4.5,0.0,-5));
    m_disc_white_papier = new GLDisc("papier",QVector3D(-2,0.0,-5));
    m_disc_white_brunnen = new GLDisc("brunnen",QVector3D(-4.5,0.0,-5));

    m_disc_black_stein = new GLDisc("stein",QVector3D(-2.0,0.0,5));
    m_disc_black_schere = new GLDisc("schere",QVector3D(-4.5, 0.0, 5));
    m_disc_black_papier = new GLDisc("papier",QVector3D(2.0,0.0,5));
    m_disc_black_brunnen = new GLDisc("brunnen",QVector3D(4.5,0.0,5));

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

    m_disc = new GLDisc("My disc",QVector3D(0.0, 0.0,0.0));
    m_disc_other = new GLDisc("My disc Other",QVector3D(0, 0.0, 0));

    m_field = new GLField();
    setPlayer(true);
    setIsMoveCorrect(true);
    m_sounds = new music(this);
    m_sounds->setEnabled(true);
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
}

void MyGLItem::createTriangles()
{
    m_firstTrianglePoint = m_points.size();

    m_points.append(GLPoint(QVector3D(0,0,0),v_Z,v_Zero,GLColorRgba::clRed));
    m_points.append(GLPoint(QVector3D(2,0,0),v_Z,v_Zero,GLColorRgba::clGreen));
    m_points.append(GLPoint(QVector3D(0,2,0),v_Z,v_Zero,GLColorRgba::clBlue));

    m_lastTrianglePoint	= m_points.size() - 1;
}

void MyGLItem::drawTriangles()
{
    if(m_lastAxesPoint - m_firstAxesPoint == 0) //We have no axes
    {
        return;
    }

    m_renderer->setColorArrayEnabled(true);
    m_renderer->setLightingEnabled(false);

    m_vertexBuffer->bind();
    //setup renderer
    m_renderer->activateAttributeBuffer(GLESRenderer::VERTEX_LOCATION);
    m_renderer->activateAttributeBuffer(GLESRenderer::COLOR_LOCATION);
    //draw
    glDrawArrays(GL_TRIANGLES, m_firstTrianglePoint, m_lastTrianglePoint -
                 m_firstTrianglePoint + 1);
    //cleanup
    m_renderer->disableAttributeArrays();
    m_vertexBuffer->release();

    //restore old settings
    m_renderer->setColorArrayEnabled(m_colorArrayEnabled);
    m_renderer->setLightingEnabled(m_lightingEnabled);
}

void MyGLItem::setupGeometry()
{
    GLItem::setupGeometry();
    //setup vertexbuffer
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if(!m_vertexBuffer->create())
    {
        exit(1);
    }

    m_field->setTextureFile(":/textures/sbrett.png");

    m_disc_white_schere->setTextureFile(":/textures/Stein_weiss_schere.png");
    m_disc_white_stein->setTextureFile(":/textures/Stein_weiss_stein.png");
    m_disc_white_papier->setTextureFile(":/textures/Stein_weiss_papier.png");
    m_disc_white_brunnen->setTextureFile(":/textures/Stein_weiss_brunnen.png");

    m_disc_black_stein->setTextureFile(":/textures/Stein_Schwarz_stein.png");
    m_disc_black_schere->setTextureFile(":/textures/Stein_Schwarz_schere.png");
    m_disc_black_papier->setTextureFile(":/textures/Stein_Schwarz_papier.png");
    m_disc_black_brunnen->setTextureFile(":/textures/Stein_Schwarz_brunnen.png");

    m_disc_white_schere->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_white_stein->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_white_papier->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_white_brunnen->readBinaryModelFile(":/models/Stein_weiss1.dat");

    m_disc_black_stein->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_black_schere->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_black_papier->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_black_brunnen->readBinaryModelFile(":/models/Stein_weiss1.dat");

    m_disc_black_schere->move(QVector3D(-4.5f, 0.0f, -7.5f));
    m_disc_black_stein->move(QVector3D(-1.5f, 0.0f, -7.5f));
    m_disc_black_papier->move(QVector3D(1.5f, 0.0f, -7.5f));
    m_disc_black_brunnen->move(QVector3D(4.5f, 0.0f, -7.5f));

    m_disc_white_brunnen->move(QVector3D(-4.5f, 0.0f, 7.5f));
    m_disc_white_papier->move(QVector3D(-1.5f, 0.0f, 7.5f));
    m_disc_white_stein->move(QVector3D(1.5f, 0.0f, 7.5f));
    m_disc_white_schere->move(QVector3D(4.5f, 0.0f, 7.5f));

    m_disc_black_schere->setDisc_Color("black");
    m_disc_black_stein->setDisc_Color("black");
    m_disc_black_papier->setDisc_Color("black");
    m_disc_black_brunnen->setDisc_Color("black");

    m_disc_white_brunnen->setDisc_Color("white");
    m_disc_white_papier->setDisc_Color("white");
    m_disc_white_stein->setDisc_Color("white");
    m_disc_white_schere->setDisc_Color("white");

    m_disc_black_schere->setDisc_Coordinates(QVector3D(-4.5f, 0.0f, -7.5f));
    m_disc_black_stein->setDisc_Coordinates(QVector3D(-1.5f, 0.0f, -7.5f));
    m_disc_black_papier->setDisc_Coordinates(QVector3D(1.5f, 0.0f, -7.5f));
    m_disc_black_brunnen->setDisc_Coordinates(QVector3D(4.5f, 0.0f, -7.5f));

    m_disc_white_brunnen->setDisc_Coordinates(QVector3D(-4.5f, 0.0f, 7.5f));
    m_disc_white_papier->setDisc_Coordinates(QVector3D(-1.5f, 0.0f, 7.5f));
    m_disc_white_stein->setDisc_Coordinates(QVector3D(1.5f, 0.0f, 7.5f));
    m_disc_white_schere->setDisc_Coordinates(QVector3D(4.5f, 0.0f, 7.5f));
}

bool MyGLItem::kampf(GLDisc * disk, QVector3D kampf_punkt, QPoint hit_coordinaten)
{
    if (!isMoveCorrect){
        return false;
    }
    QList<GLDisc*> f_disks_list;
    QList<GLDisc*> e_disks_list;
    bool figth = false;
    // Gleiche Farbe
    if (disk->getDisc_Color() == "black"){
        for (int i = 0; i < m_blackdisks_list.size(); i++) {
            if(m_blackdisks_list[i]->isHit(hit_coordinaten, renderer()))
            {
                qDebug() << "Gleiche Farbe: " << disk->getDisc_Name() << " " << disk->getDisc_Color() << " trifft " <<
                            m_blackdisks_list[i]->getDisc_Name() << " " << m_blackdisks_list[i]->getDisc_Color();
                return false;
            }
        }
        f_disks_list = m_blackdisks_list;
        e_disks_list = m_whitedisks_list;
    } else {
        for (int i = 0; i < m_whitedisks_list.size(); i++) {
            if(m_whitedisks_list[i]->isHit(hit_coordinaten, renderer()))
            {
                qDebug() << "Gleiche Farbe: " << disk->getDisc_Name() << " " << disk->getDisc_Color() << " trifft " <<
                            m_whitedisks_list[i]->getDisc_Name() << " " << m_whitedisks_list[i]->getDisc_Color();
                return false;
            }
        }
        f_disks_list = m_whitedisks_list;
        e_disks_list = m_blackdisks_list;
    }
    // Gleiche Stein
    for (int i = 0; i < e_disks_list.size(); i++) {
        if(e_disks_list[i]->isHit(hit_coordinaten, renderer()))
        {
            qDebug() << "Gleiche Stein: " << disk->getDisc_Name() << " trifft " <<
                        e_disks_list[i]->getDisc_Name();
            if(disk->getDisc_Name() == e_disks_list[i]->getDisc_Name()){
                qDebug() << "Steine sind gleich";
                return false;
            }
        }
    }
    // Kampf
    qDebug() << "Kampf";
    for (int i = 0; i < e_disks_list.size(); i++) {
        if(e_disks_list[i]->isHit(hit_coordinaten, renderer()))
        {
            if(disk->getDisc_Name() == "stein" && e_disks_list[i]->getDisc_Name() == "schere"){
                qDebug() << "stein gegen schere";
                moveDisk(disk, disk->getDisc_Coordinates(), kampf_punkt);
                move_away(e_disks_list[i]);
                e_disks_list.removeAt(i);
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "stein" && e_disks_list[i]->getDisc_Name() == "papier"){
                qDebug() << "stein gegen papier";
                move_away(disk);
                f_disks_list.removeAt(0);
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "stein" && e_disks_list[i]->getDisc_Name() == "brunnen"){
                qDebug() << "stein gegen brunnen";
                move_away(disk);
                f_disks_list.removeAt(0);
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "schere" && e_disks_list[i]->getDisc_Name() == "stein"){
                qDebug() << "schere gegen stein";
                move_away(disk);
                f_disks_list.removeAt(1);
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "schere" && e_disks_list[i]->getDisc_Name() == "papier"){
                qDebug() << "schere gegen papier";
                moveDisk(disk, disk->getDisc_Coordinates(), kampf_punkt);
                move_away(e_disks_list[i]);
                e_disks_list.removeAt(i);
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "schere" && e_disks_list[i]->getDisc_Name() == "brunnen"){
                qDebug() << "schere gegen brunnen";
                move_away(disk);
                f_disks_list.removeAt(1);
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "papier" && e_disks_list[i]->getDisc_Name() == "stein"){
                qDebug() << "papier gegen stein";
                moveDisk(disk, disk->getDisc_Coordinates(), kampf_punkt);
                move_away(e_disks_list[i]);
                e_disks_list.removeAt(i);
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "papier" && e_disks_list[i]->getDisc_Name() == "schere"){
                qDebug() << "papier gegen schere";
                move_away(disk);
                f_disks_list.removeAt(2);
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "papier" && e_disks_list[i]->getDisc_Name() == "brunnen"){
                qDebug() << "papier gegen brunnen";
                moveDisk(disk, disk->getDisc_Coordinates(), kampf_punkt);
                move_away(e_disks_list[i]);
                e_disks_list.removeAt(i);
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "brunnen" && e_disks_list[i]->getDisc_Name() == "stein"){
                qDebug() << "brunnen gegen stein";
                moveDisk(disk, disk->getDisc_Coordinates(), kampf_punkt);
                move_away(e_disks_list[i]);
                e_disks_list.removeAt(i);
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "brunnen" && e_disks_list[i]->getDisc_Name() == "schere"){
                qDebug() << "brunnen gegen schere";
                moveDisk(disk, disk->getDisc_Coordinates(), kampf_punkt);
                move_away(e_disks_list[i]);
                e_disks_list.removeAt(i);
                figth = true;
                break;
            }
            if(disk->getDisc_Name() == "brunnen" && e_disks_list[i]->getDisc_Name() == "papier"){
                qDebug() << "brunnen gegen papier";
                move_away(disk);
                f_disks_list.removeAt(3);
                figth = true;
                break;
            }
        }
    }
    // Bewegen
    if (!figth){
        qDebug() << "Move";
        moveDisk(disk, disk->getDisc_Coordinates(), kampf_punkt);
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
    turnEnd();
    // Print Black List
    //    for (int b = 0; b < m_blackdisks_list.size(); b++) {
    //        qDebug() << m_blackdisks_list[b]->getDisc_Name() << m_blackdisks_list[b]->getDisc_Color() << m_blackdisks_list[b]->getDisc_Coordinates();
    //    }
    // Print White List
    //    for (int w = 0; w < m_whitedisks_list.size(); w++) {
    //        qDebug() << m_whitedisks_list[w]->getDisc_Name() << m_whitedisks_list[w]->getDisc_Color() << m_whitedisks_list[w]->getDisc_Coordinates();
    //    }
    return true;
}

void MyGLItem::moveDisk(GLDisc * disk, QVector3D start, QVector3D end)
{
    qDebug() << "start: " << start << "end: " << end;
    float x_diff = start.x() - end.x();
    float z_diff = start.z() - end.z();
    // oben-rechts
    if (x_diff < -1.5f && z_diff > 1.5f) {
        disk->move(QVector3D(+3.0f, 0.0f, -3.0f));
        disk->setDisc_Coordinates(start + QVector3D(+3.0f, 0.0f, -3.0f));
        qDebug() << "new Disc_Coordinates: " << disk->getDisc_Coordinates();
    }
    // oben-links
    if (x_diff > 1.5f && z_diff > 1.5f) {
        disk->move(QVector3D(-3.0f, 0.0f, -3.0f));
        disk->setDisc_Coordinates(start + QVector3D(-3.0f, 0.0f, -3.0f));
        qDebug() << "new Disc_Coordinates: " << disk->getDisc_Coordinates();
    }
    // unten-links
    if (x_diff > 1.5f && z_diff < -1.5f) {
        disk->move(QVector3D(-3.0f, 0.0f, +3.0f));
        disk->setDisc_Coordinates(start + QVector3D(-3.0f, 0.0f, +3.0f));
        qDebug() << "new Disc_Coordinates: " << disk->getDisc_Coordinates();
    }
    // unten-rechts
    if (x_diff < -1.5f && z_diff < -1.5f) {
        disk->move(QVector3D(+3.0f, 0.0f, +3.0f));
        disk->setDisc_Coordinates(start + QVector3D(+3.0f, 0.0f, +3.0f));
        qDebug() << "new Disc_Coordinates: " << disk->getDisc_Coordinates();
    }
    // oben
    if (x_diff < 1.5f && x_diff > -1.5f && z_diff > 1.5f) {
        disk->move(QVector3D(0.0f, 0.0f, -3.0f));
        disk->setDisc_Coordinates(start + QVector3D(0.0f, 0.0f, -3.0f));
        qDebug() << "new Disc_Coordinates: " << disk->getDisc_Coordinates();
    }
    // unten
    if (x_diff < 1.5f && x_diff > -1.5f && z_diff < -1.5f) {
        disk->move(QVector3D(0.0f, 0.0f, +3.0f));
        disk->setDisc_Coordinates(start + QVector3D(0.0f, 0.0f, +3.0f));
        qDebug() << "new Disc_Coordinates: " << disk->getDisc_Coordinates();
    }
    // links
    if (x_diff > 1.5f && z_diff > -1.5f && z_diff < 1.5f) {
        disk->move(QVector3D(-3.0f, 0.0f, 0.0f));
        disk->setDisc_Coordinates(start + QVector3D(-3.0f, 0.0f, 0.0f));
        qDebug() << "new Disc_Coordinates: " << disk->getDisc_Coordinates();
    }
    // rechts
    if (x_diff < -1.5f && z_diff > -1.5f && z_diff < 1.5f) {
        disk->move(QVector3D(+3.0f, 0.0f, 0.0f));
        disk->setDisc_Coordinates(start + QVector3D(+3.0f, 0.0f, 0.0f));
        qDebug() << "new Disc_Coordinates: " << disk->getDisc_Coordinates();
    }
    m_sounds->playSound(":/music/Link.wav");
}

void MyGLItem::move_away(GLDisc *disk)
{
    disk->move(QVector3D(+100.0f, 0.0f, +100.0f));
    disk->setDisc_Coordinates(disk->getDisc_Coordinates() + QVector3D(+100.0f, 0.0f, +100.0f));
    qDebug() << "Disk " << disk->getDisc_Color() << " " << disk->getDisc_Name() << " ist gelöscht";
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
    // Out of Range Test
    QVector3D end;
    renderer()->mouseIntersection(&end, v_Y, 0.0f, m_lastMouseEvent->pos());
    float end_x = end.x();
    float end_z = end.z();
    if (end_x > 6.0f || end_x < -6.0f || end_z > 9.0f || end_z < -9.0f){
        qDebug() << "Out of Range, new disc_Coordinates: " << m_disc->getDisc_Coordinates();
        setIsMoveCorrect(false);
    }
    // Distance Test
    if (!discsDistance(m_disc->getDisc_Coordinates(), end)){
        qDebug() << "Out of Range, new disc_Coordinates: " << m_disc->getDisc_Coordinates();
        setIsMoveCorrect(false);
    }
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

    QVector3D movePos;
    renderer()->mouseIntersection(&movePos, v_Y, 0.0f, m_lastMouseEvent->pos());
    QVector3D moveDistance = movePos + m_pressPosToDiscPos;
    QMatrix4x4 m;

    //    if(m_disc->getFieldCoord().x() > movePos.x()){
    //        m.translate(moveDistance);
    //        //m_disc->setFieldCoord(moveDistance);
    //        m_disc->setTransformation(m);
    //        //qDebug() << "Mouse move event Begrenzung erreicht";
    //    }


    //qDebug() << " mouse moved" << "x wert: " << x << "y Wert: " << y;
}


void MyGLItem::mousePressed(int x, int y, int button)
{
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
        m_disc->jumpUp();
        m_totalAnimationSteps = 50;
        m_animationActive = true;
        m_lastMouseEvent->setAccepted(true);
    }

    //mouse release
    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseButtonRelease) && !m_lastMouseEvent->isAccepted()){
        m_disc->jumpDown();
        m_disc->setSelected(false);
        QVector3D end;
        renderer()->mouseIntersection(&end, v_Y, 0.0f, m_lastMouseEvent->pos());
        qDebug() << "Kampf Überprüfung";
        kampf(m_disc, end, m_lastMouseEvent->pos());
        qDebug() << "";
        m_totalAnimationSteps = 50;
        m_animationActive = true;
        m_lastMouseEvent->setAccepted(true);
    }

    //    //mouse move
    //    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseMove) && !m_lastMouseEvent->isAccepted()){


    //        m_lastMouseEvent->setAccepted(true);
    //        QVector3D movePos;
    //        renderer()->mouseIntersection(&movePos, v_Y, 0.0f, m_lastMouseEvent->pos());
    //        QVector3D moveDistance = movePos + m_pressPosToDiscPos;
    //        QMatrix4x4 m;

    //        qDebug() << "is MovementOK(): " << m_disc->isMovementOk(m_disc, moveDistance, m_field);
    //        if(m_disc->isMovementOk(m_disc, moveDistance, m_field)){
    //            m.translate(moveDistance);
    //            m_disc->setTransformation(m);
    //        }

    //        /*
    //        if(m_disc->isMovementOk(m_disc, moveDistance,m_field)){
    //            m.translate(moveDistance);
    //            m_disc->setFieldCoord(moveDistance);
    //            m_disc->setTransformation(m);
    //        }
    //        */

    //        qDebug() << "Test disc x-Koordinaten: " << m_disc->getFieldCoord().x() <<" z-Koordinaten: " << m_disc->getFieldCoord().z();

    //    }

    //Animation
    if(m_animationActive){
        if(m_animationStep < m_totalAnimationSteps){
            m_animationStep++;
            m_disc->updateAnimatedProperties(static_cast<float>(m_animationStep) / static_cast<float>(m_totalAnimationSteps) );
        }else{
            m_disc->finishAnimation();
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

void MyGLItem::wheelEvent(QWheelEvent * e)
{
    if(e->delta() < 0)
    {
        //m_eye.setX(deltaX * 0.1);
        QVector3D temp = m_eye - m_center;
        temp *= 1.1f;
        m_eye = m_center + temp;
    }
    else
    {
        QVector3D temp = m_eye - m_center;
        temp /= 1.1f;
        m_eye = m_center + temp;
    }
    update();
}

void MyGLItem::createCube(const QVector3D &lbb, const QVector3D &rtf)
{
    m_cube = new GLCube("myCube",1.0,GLColorRgba::clWhite);
}

void MyGLItem::createNormals()
{
    m_firstNormalPoint = m_points.size();
    for(int i = m_firstCubePoint; i <= m_lastCubePoint; i++)
    {
        m_points.append(GLPoint(m_points[i].vertex(),m_points[i].vertex(),m_points[i].normal(),GLColorRgba::clWhite));
        m_points.append(GLPoint(m_points[i].vertex() + m_points[i].normal(),
                                m_points[i].normal(),
                                m_points[i].texCoord(),
                                GLColorRgba::clWhite));
    }
    m_lastNormalPoint = m_points.size() - 1;
}

void MyGLItem::createNormalsF()
{
    m_firstNormalPoint = m_points.size();
    for(int i = m_firstFPoint; i <= m_lastFPoint; i++)
    {
        m_points.append(GLPoint(m_points[i].vertex(),m_points[i].vertex(),m_points[i].normal(),GLColorRgba::clWhite));
        m_points.append(GLPoint(m_points[i].vertex() + m_points[i].normal(),
                                m_points[i].normal(),
                                m_points[i].texCoord(),
                                GLColorRgba::clWhite));
    }
    m_lastNormalPoint = m_points.size() - 1;
}

void MyGLItem::drawCube()
{
    if(m_lastAxesPoint - m_firstAxesPoint == 0) //We have no axes
    {
        return;
    }

    m_renderer->setColorArrayEnabled(true);
    m_renderer->setLightingEnabled(false);

    m_vertexBuffer->bind();
    //setup renderer
    m_renderer->activateAttributeBuffer(GLESRenderer::VERTEX_LOCATION);
    m_renderer->activateAttributeBuffer(GLESRenderer::COLOR_LOCATION);
    //draw
    //    for(int i  = m_firstCubePoint; i < m_lastCubePoint; i += 3)
    glDrawArrays(GL_TRIANGLES,m_firstCubePoint, m_lastCubePoint - m_firstCubePoint +1);
    glDrawArrays(GL_LINES,m_firstNormalPoint, m_lastNormalPoint - m_firstNormalPoint +1);
    //cleanup
    m_renderer->disableAttributeArrays();
    m_vertexBuffer->release();

    m_renderer->setColorArrayEnabled(m_colorArrayEnabled);
    m_renderer->setLightingEnabled(m_lightingEnabled);
}

bool MyGLItem::getIsMoveCorrect() const
{
    return isMoveCorrect;
}

void MyGLItem::setIsMoveCorrect(bool value)
{
    isMoveCorrect = value;
}

bool MyGLItem::discsDistance(QVector3D disc1, QVector3D disc2)
{
    QVector3D distanceVector = QVector3D(disc1.x()-disc2.x(), 0.0f, disc1.z()-disc2.z());
    float distance = sqrt(distanceVector.x()*distanceVector.x() + distanceVector.z()*distanceVector.z());
    if (distance > 6.05f){
        return false;
    }
    return true;
}

bool MyGLItem::gameOverTest()
{
    for (int b = 0; b < m_blackdisks_list.size(); b++) {
        for (int i = 0; i < 4; i++) {
            if (m_blackdisks_list[b]->getDisc_Coordinates() == m_whitePos[i]){
                qDebug() << "Black Player wins!";
                return true;
            }
        }
    }
    for (int w = 0; w < m_whitedisks_list.size(); w++) {
        for (int j = 0; j < 4; j++) {
            if (m_whitedisks_list[w]->getDisc_Coordinates() == m_blackPos[j]){
                qDebug() << "White Player wins!";
                return true;
            }
        }
    }
    if (m_whitedisks_list.isEmpty()){
        qDebug() << "Black Player wins!";
        return true;
    }
    if (m_blackdisks_list.isEmpty()){
        qDebug() << "White Player wins!";
        return true;
    }
    if (m_whitedisks_list.size() == 1 && m_blackdisks_list.size() == 1){
        if(m_whitedisks_list[0]->getDisc_Name() == m_blackdisks_list[0]->getDisc_Name()){
            qDebug() << "Spiel ist unentschieden beendet!";
            return true;
        }
    }
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
    m_eye = m_eye * factor;
    update();
}

void MyGLItem::turnEnd()
{
    // Spieler wechsel
    changePlayer(player);
    if (!gameOverTest()){
        // Board umdrehen
        rotateBoard();
    }  else {
        // Spiel neustarten
        setPlayer(true);
        setIsMoveCorrect(true);
        m_eye = QVector3D(0.0, 1.0, 1.0)* 17.0;
        emit textChanged("Let's fight begin!!!!");
        emit textColorChanged("black");
        emit textBackgroundColorChanged("yellow");
        setupGeometry();
        m_whitedisks_list.append(m_disc_white_stein);
        m_whitedisks_list.append(m_disc_white_schere);
        m_whitedisks_list.append(m_disc_white_papier);
        m_whitedisks_list.append(m_disc_white_brunnen);

        m_blackdisks_list.append(m_disc_black_stein);
        m_blackdisks_list.append(m_disc_black_schere);
        m_blackdisks_list.append(m_disc_black_papier);
        m_blackdisks_list.append(m_disc_black_brunnen);
        update();
    }
}

bool MyGLItem::getPlayer() const
{
    return player;
}

void MyGLItem::setPlayer(bool value)
{
    player = value;
}
