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

/**
 * Methodenname : MyGLItem
 * Funktion     : Konstruktor der Klasse MyGLItem
 * Parameter    : keine
 */
MyGLItem::MyGLItem() : GLItem()
{
    m_backgroundColor = GLColorRgba::clWhite;
    m_movementEnabled = true;
    // m_eye Abstand Kamerasicht zu Spielfeld
    m_eye = QVector3D(0.0f, 17.0f, 17.0f);
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

    //Weiße Spielsteine
    m_disc_white_stein = new GLDisc("stein",QVector3D(-1.5, 0.0, -7.5));
    m_disc_white_schere = new GLDisc("schere", QVector3D(1.5, 0.0, -7.5));
    m_disc_white_papier = new GLDisc("papier",QVector3D(-4.5, 0.0, -7.5));
    m_disc_white_brunnen = new GLDisc("brunnen",QVector3D(-4.5, 0.0, -7.5));

    //Schwarze Spielsteine
    m_disc_black_stein = new GLDisc("stein",QVector3D(-4.5, 0.0, 7.5));
    m_disc_black_schere = new GLDisc("schere",QVector3D(-1.5, 0.0, 7.5));
    m_disc_black_papier = new GLDisc("papier",QVector3D(1.5, 0.0, 7.5));
    m_disc_black_brunnen = new GLDisc("brunnen",QVector3D(4.5, 0.0, 7.5));

    //Weiße Spielsteine werden in die Liste eingefügt
    m_whitediscs_list.append(m_disc_white_stein);
    m_whitediscs_list.append(m_disc_white_schere);
    m_whitediscs_list.append(m_disc_white_papier);
    m_whitediscs_list.append(m_disc_white_brunnen);

    //Schwarze Spielsteine werden in die Liste eingefügt
    m_blackdiscs_list.append(m_disc_black_stein);
    m_blackdiscs_list.append(m_disc_black_schere);
    m_blackdiscs_list.append(m_disc_black_papier);
    m_blackdiscs_list.append(m_disc_black_brunnen);

    //Startposition von Schwarzen Spielsteinen
    m_blackPos.append(QVector3D(-1.5f, 0.0f, -7.5f));
    m_blackPos.append(QVector3D(1.5f, 0.0f, -7.5f));
    m_blackPos.append(QVector3D(4.5f, 0.0f, -7.5f));
    m_blackPos.append(QVector3D(-4.5f, 0.0f, -7.5f));

    //Startposition von Weißen Spielsteinen
    m_whitePos.append(QVector3D(-4.5f, 0.0f, 7.5f));
    m_whitePos.append(QVector3D(-1.5f, 0.0f, 7.5f));
    m_whitePos.append(QVector3D(1.5f, 0.0f, 7.5f));
    m_whitePos.append(QVector3D(4.5f, 0.0f, 7.5f));

    //m_disc = spielbare Disc
    m_disc = new GLDisc("My disc", QVector3D(0.0, 0.0, 0.0));
    //m_disc_other = abwählen der aktuellen Disc
    m_disc_other = new GLDisc("My disc Other", QVector3D(100.0, 0.0, 100.0));
    m_disc_temp = m_disc_other;

    //Spielfeld
    m_field = new GLField();
    setPlayer(true);
    setIsMoveCorrect(true);
    m_sounds = new music(this);
    m_sounds->setEnabled(true);
    newgame = true;
    alarmtimer = new QTimer(this);
    connect(alarmtimer, SIGNAL(timeout()), this, SLOT(alarmOff()));
}

/**
 * Methodenname : paintUnderQmlScene
 * Funktion     : Zeichnen des Feldes
 * Parameter    : keine
 */
void MyGLItem::paintUnderQmlScene()
{
    m_field->draw(renderer());
}

/**
 * Methodenname : paintOnTopOfQmlScene
 * Funktion     : Zeichnen der Steine
 * Parameter    : keine
 */
void MyGLItem::paintOnTopOfQmlScene()
{
    //Zeichnen von Schwarzen Spielsteinen
    m_disc_black_stein->draw(renderer());
    m_disc_black_schere->draw(renderer());
    m_disc_black_papier->draw(renderer());
    m_disc_black_brunnen->draw(renderer());

    //Zeichnen von Weißen Spielsteinen
    m_disc_white_stein->draw(renderer());
    m_disc_white_schere->draw(renderer());
    m_disc_white_papier->draw(renderer());
    m_disc_white_brunnen->draw(renderer());

    m_disc_other->draw(renderer());
}

/**
 * Methodenname : setupGeometry
 * Funktion     : Textursetzung und Modellsetzung
 * Parameter    : keine
 */
void MyGLItem::setupGeometry()
{
    GLItem::setupGeometry();
    if(newgame){
        //Texturesetzung muss nur einmal gemacht werden
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

    //Modellsetzung bei jedem neuen Spielstart
    m_disc_white_schere->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_white_stein->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_white_papier->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_white_brunnen->readBinaryModelFile(":/models/Stein_weiss1.dat");

    m_disc_black_stein->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_black_schere->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_black_papier->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_black_brunnen->readBinaryModelFile(":/models/Stein_weiss1.dat");

    //Farbsetzung der Steine
    m_disc_black_schere->setDisc_Color("black");
    m_disc_black_stein->setDisc_Color("black");
    m_disc_black_papier->setDisc_Color("black");
    m_disc_black_brunnen->setDisc_Color("black");

    m_disc_white_brunnen->setDisc_Color("white");
    m_disc_white_papier->setDisc_Color("white");
    m_disc_white_stein->setDisc_Color("white");
    m_disc_white_schere->setDisc_Color("white");

    //Disc Startposition setzen
    setDiscs();

    // Fake disc
    m_disc_other->setTextureFile(":/textures/Stein_schwarz.png");
    m_disc_other->readBinaryModelFile(":/models/Stein_weiss1.dat");
    m_disc_other->move(QVector3D(100.0f, 0.0f, 100.0f));
    m_disc_other->setHoldCoordinates(QVector3D(100.0f, 0.0f, 100.0f));
}


/**
 * Methodenname : setupBuffers
 * Funktion     : Bufferinitialisierung
 * Parameter    : keine
 */
void MyGLItem::setupBuffers()
{
    //setup vertexbuffer
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if(!m_vertexBuffer->create()){
        exit(1);
    }
    m_vertexBuffer->bind();
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

/**
 * Methodenname : doSynchronizeThreads
 * Funktion     : Realisierung der verschiedenen Threads
 * Parameter    : keine
 */
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
        //Stein ist noch nicht bewegt
        m_disc->setIsMoved(false);
        //Zuweisung aktueller Position zu moving Position
        m_disc->setMoveCoordinates(m_disc->getHoldCoordinates());
        //Stein wird hochgehoben
        m_disc_temp = m_disc;
        m_disc_temp->jumpUp();
        m_totalAnimationSteps = 10;
        m_animationActive = true;
        m_lastMouseEvent->setAccepted(true);
    }

    //Mouse move
    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseMove) && !m_lastMouseEvent->isAccepted()){
        m_lastMouseEvent->setAccepted(true);
        //Wir können Stein nur bewegen, wenn geklickt und der Stein hoch gehoben ist
        if (m_disc->getFinalLiftVector().y() == 1.0f){
            renderer()->mouseIntersection(&discPosition, v_Y, 0.0f, m_lastMouseEvent->pos());
            moving(m_disc, discPosition);
            update();
        }
    }

    //Mouse release
    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseButtonRelease) && !m_lastMouseEvent->isAccepted()){
        alarmOff();
        m_disc->setSelected(false);

        figth(m_disc);

        //Stein wird runter gesetzt
        m_disc_temp->jumpDown();
        m_totalAnimationSteps = 10;
        m_animationActive = true;
        m_lastMouseEvent->setAccepted(true);
    }

    //Animation
    if(m_animationActive){
        if(m_animationStep < m_totalAnimationSteps){
            m_animationStep++;
            m_disc_temp->updateAnimatedProperties(static_cast<float>(m_animationStep) / static_cast<float>(m_totalAnimationSteps) );
        }else{
            m_disc_temp->finishAnimation();
            m_animationStep = 0;
            m_animationActive = false;
        }
    }
}

/**
 * Methodenname : mousePressed
 * Funktion     : Umsetzung der Funktionen beim Drücken der Maustaste
 * Parameter    : int x, y , button
 */
void MyGLItem::mousePressed(int x, int y, int button)
{
    //Meldung wird ausgeschaltet
    alarmOff();

    if(m_lastMouseEvent)
        delete m_lastMouseEvent;
    m_lastMouseEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, QPointF(x, y), QPointF(x, y),
                                       static_cast<Qt::MouseButton>(button),static_cast<Qt::MouseButton>(button), Qt::NoModifier);
    m_lastMouseEvent->setAccepted(false);
    if(window())
        window()->update();
    QList<GLDisc*> discs_list;

    //Zuweisung Liste zu Spielerfarbe
    if (player){
        discs_list = m_blackdiscs_list;
    } else {
        discs_list = m_whitediscs_list;
    }
    setIsMoveCorrect(true);
    // Ob Spieler richtige Farbe trifft
    for (int d = 0; d < discs_list.size(); d++) {
        if(discs_list[d]->isHit(QPoint(x,y),renderer())){
            showErrorMesage("Falsche Farbe");
            setIsMoveCorrect(false);
        }
    }
    // Setzen der Disc als m_Disc aus der gewünschten Liste
    for (int w = 0; w < m_whitediscs_list.size(); w++) {
        if(m_whitediscs_list[w]->isHit(QPoint(x,y),renderer())){
            m_disc = m_whitediscs_list[w];
        }
    }
    for (int b = 0; b < m_blackdiscs_list.size(); b++) {
        if(m_blackdiscs_list[b]->isHit(QPoint(x,y),renderer())){
            m_disc = m_blackdiscs_list[b];
        }
    }
}

/**
 * Methodenname : mouseMoved
 * Funktion     : Umsetzung der Funktionen beim Bewegen der Maus
 * Parameter    : int x, y , button
 */
void MyGLItem::mouseMoved(int x, int y, int button)
{
    if(m_lastMouseEvent)
        delete m_lastMouseEvent;
    m_lastMouseEvent = new QMouseEvent(QMouseEvent::MouseMove, QPointF(x, y), QPointF(x, y),
                                       static_cast<Qt::MouseButton>(button),static_cast<Qt::MouseButton>(button), Qt::NoModifier);
    m_lastMouseEvent->setAccepted(false);

    if(window())
        window()->update();
}

/**
 * Methodenname : mouseReleased
 * Funktion     : Umsetzung der Funktionen beim Loslassen der Maustaste
 * Parameter    : int x, y , button
 */
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
}

/**
 * Methodenname : setDiscs
 * Funktion     : Setzen der zufälligen Position der Steine
 * Parameter    : keine
 */
void MyGLItem::setDiscs()
{
    QList<GLDisc*> blackdiscs = m_blackdiscs_list;
    QList<QVector3D> blackPositions = m_blackPos;
    QList<GLDisc*> whitediscs = m_whitediscs_list;
    QList<QVector3D> whitePositions = m_whitePos;
    // Shuffle
    qsrand(uint(time(nullptr)));
    for (int i = 0; i < 3; i++) {
        std::random_shuffle(whitePositions.begin()+i, whitePositions.begin()+4);
        std::random_shuffle(blackPositions.begin() + i, blackPositions.begin() + 4);
    }
    // Schwarze Steine
    for (int b = 0; b < blackdiscs.size(); b++) {
        blackdiscs[b]->move(blackPositions[b]);
        blackdiscs[b]->setHoldCoordinates(blackPositions[b]);
        blackdiscs[b]->setMoveCoordinates(blackPositions[b]);
        blackdiscs[b]->setXZ();
    }
    // Weiße Steine
    for (int w = 0; w < whitediscs.size(); w++) {
        whitediscs[w]->move(whitePositions[w]);
        whitediscs[w]->setHoldCoordinates(whitePositions[w]);
        whitediscs[w]->setMoveCoordinates(whitePositions[w]);
        whitediscs[w]->setXZ();
    }
}

/**
 * Methodenname : moving
 * Funktion     : Bewegung der Spielsteine
 * Parameter    : GLDisc * disc, QVector3D MousePos
 */
void MyGLItem::moving(GLDisc * disc, QVector3D MousePos)
{
    if (isMoveCorrect){
        alarmOff();
        QList<GLDisc*> friends_list;
        QList<GLDisc*> enemy_list;
        //Zuweisung von Listen
        if (disc->getDisc_Color() == "black"){
            friends_list = m_blackdiscs_list;
            enemy_list = m_whitediscs_list;
        } else {
            friends_list = m_whitediscs_list;
            enemy_list = m_blackdiscs_list;
        }

        float mouse_x = MousePos.x();
        float mouse_z = MousePos.z();

        QString buch = QString(disc->getDXZ_temp()[0]);
        QString zahl = QString(disc->getDXZ_temp()[1]);

        // Out of Range
        if (mouse_x > 6.0f || mouse_x < -6.0f || mouse_z > 9.0f || mouse_z < -9.0f){
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

        //Überprüfung ob Maus bewegt wurde und ob das Feld, auf den die Maus zeigt, frei ist
        if ((disc->getDXZ_temp() != buch+zahl) && isFree(disc->getDXZ(), buch+zahl, disc->getDisc_Name(), friends_list, enemy_list)){
            //Überprüfung ob Zielposition von Stein erreichbar ist
            if (disc->isMovementOk(buch+zahl)){
                //Wenn es nicht im gleichen Zug die erste Stein Bewegung ist, dann muss backStep gemacht werden
                if(disc->isMoved()){
                   disc->backStep();
                }
                //Vektor um wie viel der Stein bewegt werden muss
                QVector3D movedisc = disc->getVector(disc->getList(), buch+zahl);
                //Zuweisung neuer Move Koordinaten
                disc->setMoveCoordinates(disc->getHoldCoordinates() + movedisc);
                disc->move(movedisc);
                //Aktualisierung temporärer Koordinaten
                disc->setDXZ_temp(buch+zahl);
                m_sounds->playSound(":/music/clearly.wav");
                //Stein ist in diesem Zug schon mindestens einmal beweget worden
                disc->setIsMoved(true);
            }
        }
    }
}

/**
 * Methodenname : isFree
 * Funktion     : Überprüfung, ob Position frei ist
 * Parameter    : QString start, zelle, disc_name; QList<GLDisc*> frends_list, enemy_list
 */
bool MyGLItem::isFree(QString start, QString zelle, QString disc_name, QList<GLDisc*> frends_list, QList<GLDisc*> enemy_list)
{
    QString stein = "";
    for (int i = 0; i < frends_list.size(); i++) {
        stein = frends_list[i]->getDXZ();
        if (stein == zelle && stein != start){
            showErrorMesage("Gleiche Farbe!");
            m_sounds->playSound(":/music/when.wav");
            return false;
        }
    }
    for (int i = 0; i < enemy_list.size(); i++) {
        stein = enemy_list[i]->getDXZ();
        if (stein == zelle && enemy_list[i]->getDisc_Name() == disc_name){
            showErrorMesage("Gleiche Stein!");
            m_sounds->playSound(":/music/when.wav");
            return false;
        }
    }
    return true;
}

/**
 * Methodenname : figth
 * Funktion     : Umsetzung der Spielsteinkämpfe
 * Parameter    : GLDisc *disc
 */
bool MyGLItem::figth(GLDisc *disc)
{
    if (disc->getDXZ() == disc->getDXZ_temp()){
        return false;
    }
    QList<GLDisc*> f_discs_list;
    QList<GLDisc*> e_discs_list;
    bool figth = false;
    // Listen
    if (disc->getDisc_Color() == "black"){
        f_discs_list = m_blackdiscs_list;
        e_discs_list = m_whitediscs_list;
    } else {
        f_discs_list = m_whitediscs_list;
        e_discs_list = m_blackdiscs_list;
    }
    // Kampf
    qDebug() << "Kampf";
    for (int i = 0; i < e_discs_list.size(); i++) {
        if(e_discs_list[i]->getHoldCoordinates() == disc->getMoveCoordinates() && disc->isFigth(e_discs_list[i]->getDXZ()))
        {
            if(disc->getDisc_Name() == "stein" && e_discs_list[i]->getDisc_Name() == "schere"){
                qDebug() << "Stein gegen Schere";
                move_away(e_discs_list[i]);
                e_discs_list = deletediscFromList(e_discs_list, e_discs_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disc->getDisc_Name() == "stein" && e_discs_list[i]->getDisc_Name() == "papier"){
                qDebug() << "Stein gegen Papier";
                move_away(disc);
                f_discs_list = deletediscFromList(f_discs_list, disc->getDisc_Name());
                figth = true;
                break;
            }
            if(disc->getDisc_Name() == "stein" && e_discs_list[i]->getDisc_Name() == "brunnen"){
                qDebug() << "Stein gegen Brunnen";
                move_away(disc);
                f_discs_list = deletediscFromList(f_discs_list, disc->getDisc_Name());
                figth = true;
                break;
            }
            if(disc->getDisc_Name() == "schere" && e_discs_list[i]->getDisc_Name() == "stein"){
                qDebug() << "Schere gegen Stein";
                move_away(disc);
                f_discs_list = deletediscFromList(f_discs_list, disc->getDisc_Name());
                figth = true;
                break;
            }
            if(disc->getDisc_Name() == "schere" && e_discs_list[i]->getDisc_Name() == "papier"){
                qDebug() << "Schere gegen Papier";
                move_away(e_discs_list[i]);
                e_discs_list = deletediscFromList(e_discs_list, e_discs_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disc->getDisc_Name() == "schere" && e_discs_list[i]->getDisc_Name() == "brunnen"){
                qDebug() << "Schere gegen Brunnen";
                move_away(disc);
                f_discs_list = deletediscFromList(f_discs_list, disc->getDisc_Name());
                figth = true;
                break;
            }
            if(disc->getDisc_Name() == "papier" && e_discs_list[i]->getDisc_Name() == "stein"){
                qDebug() << "Papier gegen Stein";
                move_away(e_discs_list[i]);
                e_discs_list = deletediscFromList(e_discs_list, e_discs_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disc->getDisc_Name() == "papier" && e_discs_list[i]->getDisc_Name() == "schere"){
                qDebug() << "Papier gegen Schere";
                move_away(disc);
                f_discs_list = deletediscFromList(f_discs_list, disc->getDisc_Name());
                figth = true;
                break;
            }
            if(disc->getDisc_Name() == "papier" && e_discs_list[i]->getDisc_Name() == "brunnen"){
                qDebug() << "Papier gegen Brunnen";
                move_away(e_discs_list[i]);
                e_discs_list = deletediscFromList(e_discs_list, e_discs_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disc->getDisc_Name() == "brunnen" && e_discs_list[i]->getDisc_Name() == "stein"){
                qDebug() << "Brunnen gegen Stein";
                move_away(e_discs_list[i]);
                e_discs_list = deletediscFromList(e_discs_list, e_discs_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disc->getDisc_Name() == "brunnen" && e_discs_list[i]->getDisc_Name() == "schere"){
                qDebug() << "Brunnen gegen Schere";
                move_away(e_discs_list[i]);
                e_discs_list = deletediscFromList(e_discs_list, e_discs_list[i]->getDisc_Name());
                figth = true;
                break;
            }
            if(disc->getDisc_Name() == "brunnen" && e_discs_list[i]->getDisc_Name() == "papier"){
                qDebug() << "Brunnen gegen Papier";
                move_away(disc);
                f_discs_list = deletediscFromList(f_discs_list, disc->getDisc_Name());
                figth = true;
                break;
            }
        }
    }
    // Liste aktualisieren
    if (figth) {
        if (disc->getDisc_Color() == "black"){
            m_whitediscs_list = e_discs_list;
            m_blackdiscs_list = f_discs_list;
        } else {
            m_blackdiscs_list = e_discs_list;
            m_whitediscs_list = f_discs_list;
        }
    }
    // set Moved
    disc->setIsMoved(false);
    // Koordinaten setzen
    disc->setHoldCoordinates(m_disc->getMoveCoordinates());
    // Update XZ
    disc->setStepVector(QVector3D(0.0f, 0.0f, 0.0f));
    disc->updateXZ();
    turnEnd();
    return true;
}

/**
 * Methodenname : move_away
 * Funktion     : Spielstein vom Spielfeld entfernen
 * Parameter    : GLDisc *disc
 */
void MyGLItem::move_away(GLDisc *disc)
{
    disc->move(QVector3D(+100.0f, 0.0f, +100.0f));
    disc->setHoldCoordinates(disc->getHoldCoordinates() + QVector3D(+100.0f, 0.0f, +100.0f));
    m_sounds->playSound(":/music/Blop.wav");
    qDebug() << "Disc " << disc->getDisc_Color() << " " << disc->getDisc_Name() << " ist gelöscht";
}

/**
 * Methodenname : deletediscFromList
 * Funktion     : Entfernen des gelöschten Spielsteins von der Liste
 * Parameter    : QList<GLDisc *> m_discs_list, QString disc_name
 */
QList<GLDisc *> MyGLItem::deletediscFromList(QList<GLDisc *> m_discs_list, QString disc_name)
{
    for (int i = 0; i < m_discs_list.size(); i++) {
        if (m_discs_list[i]->getDisc_Name() == disc_name){
            m_discs_list.removeAt(i);
        }
    }
    return m_discs_list;
}

/**
 * Methodenname : turnEnd
 * Funktion     : Beenden des Zuges und tauschen des Spielers oder bei Spielende Spiel Neustarten
 * Parameter    : keine
 */
void MyGLItem::turnEnd()
{
    if (isGameOver()){
        restartGame();
    }  else {
        // Actual disc to Fake disc
        m_disc = m_disc_other;
        // Spieler wechsel
        changePlayer(player);
        // Board umdrehen
        rotateBoard();
    }
}

/**
 * Methodenname : isGameOver
 * Funktion     : Überprüfung, ob es einen Sieger gibt
 * Parameter    : keine
 */
bool MyGLItem::isGameOver()
{
    //Überprüfung ob ein schwarzer Stein in den Startpositionen der weißen Steine ist
    for (int b = 0; b < m_blackdiscs_list.size(); b++) {
        for (int i = 0; i < 4; i++) {
            if (m_blackdiscs_list[b]->getMoveCoordinates() == m_whitePos[i]){
                qDebug() << "Schwarzer Spieler gewinnt!";
                m_sounds->playSound(":/music/applauses.wav");
                return true;
            }
        }
    }
    //Überprüfung ob ein weißer Stein in den Startpositionen der schwarzen Steine ist
    for (int w = 0; w < m_whitediscs_list.size(); w++) {
        for (int j = 0; j < 4; j++) {
            if (m_whitediscs_list[w]->getMoveCoordinates() == m_blackPos[j]){
                qDebug() << "Weißer Spieler gewinnt!";
                m_sounds->playSound(":/music/applauses.wav");
                return true;
            }
        }
    }
    //Überprüfung ob es noch weiße Spielsteine gibt
    if (m_whitediscs_list.isEmpty()){
        qDebug() << "Schwarzer Spieler gewinnt!";
        m_sounds->playSound(":/music/applauses.wav");
        return true;
    }
    //Überprüfung ob es noch schwarze Spielsteine gibt
    if (m_blackdiscs_list.isEmpty()){
        qDebug() << "Weißer Spieler gewinnt!";
        m_sounds->playSound(":/music/applauses.wav");
        return true;
    }
    return false;
}

/**
 * Methodenname : changePlayer
 * Funktion     : Spieler wechseln
 * Parameter    : bool player
 */
void MyGLItem::changePlayer(bool player)
{
    setPlayer(!player);
}

/**
 * Methodenname : rotateBoard
 * Funktion     : Drehen des Spielfeldes
 * Parameter    : keine
 */
void MyGLItem::rotateBoard()
{
    QVector3D factor = QVector3D(1.0f, 1.0f, -1.0f);
    qDebug() << "Seiten wechseln";
    if (player){
        emit textChanged("Weißer Spieler ist am Zug");
        emit textColorChanged("white");
        emit textBackgroundColorChanged("black");
        qDebug() << "Weißer Spieler ist am Zug";
    } else {
        emit textChanged("Schwarzer Spieler ist am Zug");
        emit textColorChanged("black");
        emit textBackgroundColorChanged("white");
        qDebug() << "Schwarzer Spieler ist am Zug";
    }
    m_eye = m_eye * factor;
    update();
}

/**
 * Methodenname : restartGame
 * Funktion     : Neustarten des Spiels
 * Parameter    : keine
 */
void MyGLItem::restartGame()
{
    qDebug() << "Spiel neustarten";
    // Liste löschen
    m_whitediscs_list.clear();
    m_blackdiscs_list.clear();
    // Spiel neustarten
    setPlayer(true);
    setIsMoveCorrect(true);
    m_eye = QVector3D(0.0, 1.0, 1.0)* 17.0;
    emit textChanged("Let's fight begin!!!!");
    emit textColorChanged("black");
    emit textBackgroundColorChanged("yellow");
    //Liste wird erneuert
    m_whitediscs_list.append(m_disc_white_stein);
    m_whitediscs_list.append(m_disc_white_schere);
    m_whitediscs_list.append(m_disc_white_papier);
    m_whitediscs_list.append(m_disc_white_brunnen);
    m_blackdiscs_list.append(m_disc_black_stein);
    m_blackdiscs_list.append(m_disc_black_schere);
    m_blackdiscs_list.append(m_disc_black_papier);
    m_blackdiscs_list.append(m_disc_black_brunnen);
    setupGeometry();
    update();
}

/**
 * Methodenname : showErrorMesage
 * Funktion     : Anzeigen der Textanzeige
 * Parameter    : QString text
 */
void MyGLItem::showErrorMesage(QString text)
{
    int alarmTime = 5000;
    emit errorIcon(true);
    emit errorMessage(text);
    alarmtimer->start(alarmTime);
}

/**
 * Methodenname : getPlayer
 * Funktion     : Getterfunktion des Spielers
 * Parameter    : keine
 */
bool MyGLItem::getPlayer() const
{
    return player;
}

/**
 * Methodenname : setPlayer
 * Funktion     : Setterfunktion des Spielers
 * Parameter    : bool value
 */
void MyGLItem::setPlayer(bool value)
{
    player = value;
}

/**
 * Methodenname : getIsMoveCorrect
 * Funktion     : Getterfunktion von isMoveCorrect
 * Parameter    : keine
 */
bool MyGLItem::getIsMoveCorrect() const
{
    return isMoveCorrect;
}

/**
 * Methodenname : setIsMoveCorrect
 * Funktion     : Setterfunktion von isMoveCorrect
 * Parameter    : bool value
 */
void MyGLItem::setIsMoveCorrect(bool value)
{
    isMoveCorrect = value;
}
