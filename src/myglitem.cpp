#include "myglitem.h"
#include <QQuickWindow>
#include "src/gldefines.h"
#include "src/glmouseray.h"
#include <QVector3D>
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



    m_disc_white_stein = new GLDisc("disc_white_stein",QVector3D(2,0.0,-5));
    m_disc_white_schere = new GLDisc("disc_white_schere", QVector3D(4.5,0.0,-5));
    m_disc_white_papier = new GLDisc("disc_white_papier",QVector3D(-2,0.0,-5));
    m_disc_white_brunnen = new GLDisc("disc_white_brunnen",QVector3D(-4.5,0.0,-5));

    m_disc_black_stein = new GLDisc("disc_black_stein",QVector3D(-2.0,0.0,5));
    m_disc_black_schere = new GLDisc("disc_black_schere",QVector3D(-4.5, 0.0, 5));
    m_disc_black_papier = new GLDisc("disc_black_papier",QVector3D(2.0,0.0,5));
    m_disc_black_brunnen = new GLDisc("disc_black_brunnen",QVector3D(4.5,0.0,5));

    m_disc = new GLDisc("My disc",QVector3D(0.0, 0.0,0.0));
    m_disc_other = new GLDisc("My disc Other",QVector3D(0, 0.0, 0));

    //m_disc_black = new GLDisc("disc_black",QPoint(0,0));



    m_field = new GLField();


}

void MyGLItem::paintUnderQmlScene()
{
    //drawTriangles();
    //drawCube();
    m_field->draw(renderer());

}

void MyGLItem::paintOnTopOfQmlScene()
{
    //drawF();
    //drawF2(5);
    //m_cube->draw(renderer());
    //m_sphere->draw(renderer());

    //m_renderer->scale(QVector3D(2, 2, 2));

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
        qDebug() << "GLItem::drawAxes(): No axes data available.";
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


    //    int stride = sizeof(GLPoint);
    //    m_renderer->activateAttributeArray(GLESRenderer::VERTEX_LOCATION,
    //                                       m_points[0].vertexPointer(), stride);
    //    m_renderer->activateAttributeArray(GLESRenderer::COLOR_LOCATION,
    //                                       m_points[0].colorPointer(), stride);
    //    glDrawArrays(GL_TRIANGLES, m_firstTrianglePoint, m_lastTrianglePoint - m_firstTrianglePoint + 1); //Coordinate Axes
    //    m_renderer->disableAttributeArrays();
    //    //restore old settings
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
        qDebug() <<"Vertex Buffer create failed";
        exit(1);
    }
    //createTriangles();
    //    createCube(QVector3D(0,0,0),QVector3D(1,1,1));
    //    //createF(5);
    //    //createNormalsF();

    //    m_vertexBuffer->bind();
    //    m_vertexBuffer->allocate(m_points.data(), m_points.size() *
    //    static_cast<int>(sizeof(GLPoint)) );
    //    m_vertexBuffer->release();

    //    qDebug() << "readBinaryModelFile" << m_disc_black->readBinaryModelFile(":/models/Stein_weiss1.dat", nullptr, nullptr);
    //    qDebug() << "readBinaryModelFile" << m_disc_white->readBinaryModelFile(":/models/Stein_weiss1.dat", nullptr, nullptr);
    //    qDebug() << m_disc_white->setTextureFile("D:/Studium/5_Semester/MM/Jannik_Finke_mm2019/textures/Stein_weiss.png");
    //    qDebug() << m_disc_black->setTextureFile("D:/Studium/5_Semester/MM/Jannik_Finke_mm2019/textures/Stein_schwarz.png");
    //    m_disc_black->createTextureObjects();
    //    m_disc_white->createTextureObjects();

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

    m_disc_black_schere->setDisc_Coordinates(QVector3D(-4.5f, 0.0f, -7.5f));
    m_disc_black_stein->setDisc_Coordinates(QVector3D(-1.5f, 0.0f, -7.5f));
    m_disc_black_papier->setDisc_Coordinates(QVector3D(1.5f, 0.0f, -7.5f));
    m_disc_black_brunnen->setDisc_Coordinates(QVector3D(4.5f, 0.0f, -7.5f));

    m_disc_white_brunnen->setDisc_Coordinates(QVector3D(-4.5f, 0.0f, 7.5f));
    m_disc_white_papier->setDisc_Coordinates(QVector3D(-1.5f, 0.0f, 7.5f));
    m_disc_white_stein->setDisc_Coordinates(QVector3D(1.5f, 0.0f, 7.5f));
    m_disc_white_schere->setDisc_Coordinates(QVector3D(4.5f, 0.0f, 7.5f));

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
    qDebug() << " mouse released";

    //Kampf für White Stein
    if(m_disc == m_disc_white_stein){

        if(m_disc_black_schere->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "Ready to Party :D Black Steine wurde getroffen";
            m_disc_black_schere->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_black_stein->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "nichts passiert";
        }

        if(m_disc_black_papier->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "Black Stein disc was hit!";
            qDebug() << "Ready to Party :D Black Steine wurde getroffen";
            m_disc_white_stein->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_black_brunnen->isHit(QPoint(x,y),renderer()))
        {
            m_disc_white_stein->move(QVector3D(100.0, 100.0, 106.25));
        }
    }

    //Kampf für White Schere
    if(m_disc == m_disc_white_schere){

        if(m_disc_black_schere->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "nichts passiert";
        }

        if(m_disc_black_stein->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "nichts passiert";
            m_disc_white_schere->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_black_papier->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "Black Stein disc was hit!";
            qDebug() << "Ready to Party :D Black Steine wurde getroffen";
            m_disc_black_papier->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_black_brunnen->isHit(QPoint(x,y),renderer()))
        {
            m_disc_white_schere->move(QVector3D(100.0, 100.0, 106.25));
        }
    }

    //Kampf für White Papier
    if(m_disc == m_disc_white_papier){

        if(m_disc_black_schere->isHit(QPoint(x,y),renderer()))
        {
            m_disc_white_papier->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_black_stein->isHit(QPoint(x,y),renderer()))
        {
            m_disc_black_stein->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_black_papier->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "nichts passiert";

        }

        if(m_disc_black_brunnen->isHit(QPoint(x,y),renderer()))
        {
            m_disc_black_brunnen->move(QVector3D(100.0, 100.0, 106.25));
        }
    }

    //Kampf für White Brunnen
    if(m_disc == m_disc_white_brunnen){

        if(m_disc_black_schere->isHit(QPoint(x,y),renderer()))
        {
            m_disc_black_schere->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_black_stein->isHit(QPoint(x,y),renderer()))
        {
            m_disc_black_stein->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_black_papier->isHit(QPoint(x,y),renderer()))
        {
            m_disc_white_brunnen->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_black_brunnen->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "nichts passiert";
        }
    }


    //Kampf Schwarz

    //Kampf für Black Stein
    if(m_disc == m_disc_black_stein){

        if(m_disc_white_schere->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "Ready to Party :D Black Steine wurde getroffen";
            m_disc_white_schere->move(QVector3D(100.0, 100.0, 106.25));

        }

        if(m_disc_black_stein->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "nichts passiert";
        }

        if(m_disc_white_papier->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "Black Stein disc was hit!";
            qDebug() << "Ready to Party :D Black Steine wurde getroffen";
            m_disc_black_stein->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_white_brunnen->isHit(QPoint(x,y),renderer()))
        {
            m_disc_black_stein->move(QVector3D(100.0, 100.0, 106.25));
        }
    }

    //Kampf für Black Schere
    if(m_disc == m_disc_black_schere){

        if(m_disc_white_schere->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "nichts passiert";
        }

        if(m_disc_white_stein->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "nichts passiert";
            m_disc_black_schere->move(QVector3D(100.0, 100.0, 106.25));


        }

        if(m_disc_white_papier->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "Black Stein disc was hit!";
            qDebug() << "Ready to Party :D Black Steine wurde getroffen";
            m_disc_white_papier->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_white_brunnen->isHit(QPoint(x,y),renderer()))
        {
            m_disc_black_schere->move(QVector3D(100.0, 100.0, 106.25));
        }
    }

    //Kampf für Black Papier
    if(m_disc == m_disc_black_papier){

        if(m_disc_white_schere->isHit(QPoint(x,y),renderer()))
        {
            m_disc_black_papier->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_white_stein->isHit(QPoint(x,y),renderer()))
        {
            m_disc_white_stein->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_white_papier->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "nichts passiert";

        }

        if(m_disc_white_brunnen->isHit(QPoint(x,y),renderer()))
        {
            m_disc_white_brunnen->move(QVector3D(100.0, 100.0, 106.25));
        }
    }

    //Kampf für Black Brunnen
    if(m_disc == m_disc_black_brunnen){

        if(m_disc_white_schere->isHit(QPoint(x,y),renderer()))
        {
            m_disc_white_schere->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_white_stein->isHit(QPoint(x,y),renderer()))
        {
            m_disc_white_stein->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_white_papier->isHit(QPoint(x,y),renderer()))
        {
            m_disc_black_brunnen->move(QVector3D(100.0, 100.0, 106.25));
        }

        if(m_disc_white_brunnen->isHit(QPoint(x,y),renderer()))
        {
            qDebug() << "nichts passiert";
        }
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

    qDebug() << " MyGLItem::mousePressEvent running on thread: " << QThread::currentThreadId();
    if(m_lastMouseEvent)
        delete m_lastMouseEvent;
    m_lastMouseEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, QPointF(x, y), QPointF(x, y),
                                       static_cast<Qt::MouseButton>(button),static_cast<Qt::MouseButton>(button), Qt::NoModifier);
    m_lastMouseEvent->setAccepted(false);

    if(window())
        window()->update();



    //qDebug() << "Myglitem mouse pressed" << x << y;
    if(m_disc_black_schere->isHit(QPoint(x,y),renderer()))
    {
        /*
       qDebug() << "Black Schere disc was hit!";
       m_disc_black_schere->jumpUp();
       qDebug() << "Test1";
       m_totalAnimationSteps = 50;
       m_animationActive = true;

       m_lastMouseEvent->setAccepted(true);
       qDebug() << "Test2";
       */
        qDebug() << "Black Schere disc was hit!";
        m_disc = m_disc_black_schere;

    }


    if(m_disc_black_stein->isHit(QPoint(x,y),renderer()))
    {
        qDebug() << "Black Stein disc was hit!";
        m_disc = m_disc_black_stein;
    }

    if(m_disc_black_papier->isHit(QPoint(x,y),renderer()))
    {
        qDebug() << "Black Papier disc was hit!";
        m_disc = m_disc_black_papier;
    }

    if(m_disc_black_brunnen->isHit(QPoint(x,y),renderer()))
    {
        qDebug() << "Black Brunnen disc was hit!";
        m_disc = m_disc_black_brunnen;
    }

    if(m_disc_white_schere->isHit(QPoint(x,y),renderer()))
    {
        qDebug() << "White Schere disc was hit!";
        m_disc = m_disc_white_schere;
    }

    if(m_disc_white_papier->isHit(QPoint(x,y),renderer()))
    {
        qDebug() << "White Papier disc was hit!";
        m_disc = m_disc_white_papier;
    }

    if(m_disc_white_stein->isHit(QPoint(x,y),renderer()))
    {
        qDebug() << "White Stein disc was hit!";
        m_disc = m_disc_white_stein;
    }

    if(m_disc_white_brunnen->isHit(QPoint(x,y),renderer()))
    {
        qDebug() << "White Brunnen disc was hit!";
        m_disc = m_disc_white_brunnen;
    }


}

void MyGLItem::doSynchronizeThreads()
{
    GLItem::doSynchronizeThreads();
    setupRenderer();


    //Mouse pressed
    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseButtonPress) && !m_lastMouseEvent->isAccepted()){

        //m_disc->setShowFrame(m_disc->isHit(m_lastMouseEvent->pos(), renderer()));
        m_lastMouseEvent->setAccepted(true);
        //Calculate distance from press point to disc center

        //        QVector3D pressPos;
        //        renderer()->mouseIntersection(&pressPos, v_Y, 0.0f, m_lastMouseEvent->pos());
        //        //m_disc->setDisc_Coordinates(pressPos);
        //        //QVector3D discPos = m_disc->getTransformation() * v_Zero; //translation of disc
        //        QVector3D discPos = m_disc->getDisc_Coordinates();
        //        m_pressPosToDiscPos = discPos - pressPos;
        //        //m_pressPosToDiscPos = m_pressPosToDiscPos * -1;
        //        qDebug() << "m_pressPosToDiscPos: " << m_pressPosToDiscPos << "pressPos" << pressPos;
        QVector3D nearPoint;
        QVector3D farPoint;
        //    //    qDebug() << " MyGLItem::paintOnTop running on thread: " << QThread::currentThreadId();

        //        /*
        if( m_lastMouseEvent){
            qDebug() << "Test62";
            renderer()->calculateMousePoints(&nearPoint, &farPoint, m_lastMouseEvent->pos());
            qDebug() << "Test63" << nearPoint << " far point" << farPoint;
            //m_mouseRay->setPoints(nearPoint, farPoint);
            qDebug() << "Test64";
        }
        //        */

        if(m_disc->isHit(nearPoint, farPoint)){
            qDebug() << "Test65";
            m_disc->setSelected(true);
            //m_disc->setShowFrame(true);
        }

        m_disc->jumpUp();
        m_totalAnimationSteps = 50;
        m_animationActive = true;
        m_lastMouseEvent->setAccepted(true);

        //m_disc->setFieldCoord(pressPos);


    }

    //mouse release
    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseButtonRelease) && !m_lastMouseEvent->isAccepted()){
        //m_disc->setShowFrame(false);
        m_disc->setSelected(false);
        m_disc->jumpDown();

        QVector3D end;
        renderer()->mouseIntersection(&end, v_Y, 0.0f, m_lastMouseEvent->pos());

        QVector3D start = m_disc->getDisc_Coordinates();
        qDebug() << "start: " << start << "end: " << end;

        float x_diff = start.x() - end.x();
        float z_diff = start.z() - end.z();

        float end_x = end.x();
        float end_z = end.z();

        if (end_x > 6.0f || end_x < -6.0f || end_z > 9.0f || end_z < -9.0f){
            //m_disc->move(start);
            qDebug() << "Out of Range, new disc_Coordinates: " << m_disc->getDisc_Coordinates();
        } else {
            // oben-rechts
            if (x_diff < -1.5f && z_diff > 1.5f) {
                m_disc->move(QVector3D(+3.0f, 0.0f, -3.0f));
                m_disc->setDisc_Coordinates(start + QVector3D(+3.0f, 0.0f, -3.0f));
                qDebug() << "new isc_Coordinates: " << m_disc->getDisc_Coordinates();
            }
            // oben-links
            if (x_diff > 1.5f && z_diff > 1.5f) {
                m_disc->move(QVector3D(-3.0f, 0.0f, -3.0f));
                m_disc->setDisc_Coordinates(start + QVector3D(-3.0f, 0.0f, -3.0f));
                qDebug() << "new isc_Coordinates: " << m_disc->getDisc_Coordinates();
            }
            // unten-links
            if (x_diff > 1.5f && z_diff < -1.5f) {
                m_disc->move(QVector3D(-3.0f, 0.0f, +3.0f));
                m_disc->setDisc_Coordinates(start + QVector3D(-3.0f, 0.0f, +3.0f));
                qDebug() << "new isc_Coordinates: " << m_disc->getDisc_Coordinates();
            }
            // unten-rechts
            if (x_diff < -1.5f && z_diff < -1.5f) {
                m_disc->move(QVector3D(+3.0f, 0.0f, +3.0f));
                m_disc->setDisc_Coordinates(start + QVector3D(+3.0f, 0.0f, +3.0f));
                qDebug() << "new isc_Coordinates: " << m_disc->getDisc_Coordinates();
            }
            // oben
            if (x_diff < 1.5f && x_diff > -1.5f && z_diff > 1.5f) {
                m_disc->move(QVector3D(0.0f, 0.0f, -3.0f));
                m_disc->setDisc_Coordinates(start + QVector3D(0.0f, 0.0f, -3.0f));
                qDebug() << "new isc_Coordinates: " << m_disc->getDisc_Coordinates();
            }
            // unten
            if (x_diff < 1.5f && x_diff > -1.5f && z_diff < -1.5f) {
                m_disc->move(QVector3D(0.0f, 0.0f, +3.0f));
                m_disc->setDisc_Coordinates(start + QVector3D(0.0f, 0.0f, +3.0f));
                qDebug() << "new isc_Coordinates: " << m_disc->getDisc_Coordinates();
            }
            // links
            if (x_diff > 1.5f && z_diff > -1.5f && z_diff < 1.5f) {
                m_disc->move(QVector3D(-3.0f, 0.0f, 0.0f));
                m_disc->setDisc_Coordinates(start + QVector3D(-3.0f, 0.0f, 0.0f));
                qDebug() << "new isc_Coordinates: " << m_disc->getDisc_Coordinates();
            }
            // rechts
            if (x_diff < -1.5f && z_diff > -1.5f && z_diff < 1.5f) {
                m_disc->move(QVector3D(+3.0f, 0.0f, 0.0f));
                m_disc->setDisc_Coordinates(start + QVector3D(+3.0f, 0.0f, 0.0f));
                qDebug() << "new isc_Coordinates: " << m_disc->getDisc_Coordinates();
            }
            renderer()->rotate(180.0f, QVector3D(1.0f, 0.0f, 0.0f));
        }

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
        qDebug() <<"Vertex Buffer create failed";
        exit(1);
    }
    m_vertexBuffer->bind();
    //  m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw); // StaticDraw is default. However we set it for clearity.
    m_vertexBuffer->allocate(m_points.data(), m_points.size() * static_cast<int>(sizeof(GLPoint)) );
    m_vertexBuffer->release();

    //setup indexbuffer
    m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    if(!m_indexBuffer->create()){
        qDebug() <<"Index Buffer create failed";
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
    //    if (window()) {
    //       window()->update();
    ////    }
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
        qDebug() << "GLItem::drawAxes(): No axes data available.";
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

void MyGLItem::createF(float height)
{
    float length = height/5;

    m_firstFPoint = m_points.size();
    createCube(QVector3D(0,0,0),QVector3D(length,height,length));
    createCube(QVector3D(0,2*length,0),QVector3D(2*length,3*length,length));
    createCube(QVector3D(0,4*length,0),QVector3D(3*length,height,length));
    m_lastFPoint = m_points.size() -1;
    createNormals();
}



void MyGLItem::drawF()
{
    if(m_lastAxesPoint - m_firstAxesPoint == 0) //We have no axes
    {
        qDebug() << "GLItem::drawAxes(): No axes data available.";
        return;
    }

    m_renderer->setColorArrayEnabled(true);
    m_renderer->setLightingEnabled(false);


    m_vertexBuffer->bind();
    //setup renderer
    m_renderer->activateAttributeBuffer(GLESRenderer::VERTEX_LOCATION);
    m_renderer->activateAttributeBuffer(GLESRenderer::COLOR_LOCATION);
    //draw
    // for(int i  = m_firstFPoint; i < m_lastFPoint; i += 3)
    glDrawArrays(GL_TRIANGLES,m_firstFPoint,m_lastFPoint - m_firstFPoint +1);
    //draw Normals
    glDrawArrays(GL_LINES,m_firstNormalPoint, m_lastNormalPoint - m_firstNormalPoint +1);
    //cleanup
    m_renderer->disableAttributeArrays();
    m_vertexBuffer->release();

    m_renderer->setColorArrayEnabled(m_colorArrayEnabled);
    m_renderer->setLightingEnabled(m_lightingEnabled);
}

void MyGLItem::drawF2(float height)
{
    renderer()->pushMvMatrix();
    renderer()->scale(QVector3D(1.0f, height, 1.0f));
    drawCube();
    renderer()->popMvMatrix();

    renderer()->pushMvMatrix();
    renderer()->translate(QVector3D(1.0f, 2.0f, 0.0f));
    drawCube();
    renderer()->popMvMatrix();

    renderer()->pushMvMatrix();
    renderer()->translate(QVector3D(1.0f, 4.0f, 0.0f));
    renderer()->scale(QVector3D(2.0f, 1.0f, 1.0f));
    drawCube();
    renderer()->popMvMatrix();
}
