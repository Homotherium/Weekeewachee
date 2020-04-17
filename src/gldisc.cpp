#include "gldisc.h"
#include <math.h>

GLDisc::GLDisc(const QString & name, const QVector3D &fieldCoord, float radius, const GLColorRgba &color, const QString textureFile, float height, IndexType slices)
    :GLBody(name, radius, color, textureFile)
{
    m_disc_Name = name;
    m_FieldCoord = fieldCoord;
    m_height = height;
    m_slices = slices;
    m_drawingMode = GL_TRIANGLE_STRIP;
    diskLastmove = "";
    m_isKing = false;
    isMove = false;
    setShowFrame(false);
    dx = "O";
    dz = "0";
    dx_temp = "O";
    dz_temp = "0";
    stepVector = {0.0f, 0.0f, 0.0f};
}

void GLDisc::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{

    GLBody::makeSurface(pointContainer, indexContainer);
    QVector3D vCenter = m_center;
    QVector3D vRadius = v_X * m_radius;
    QVector3D vUp = v_Y * m_height;

    QMatrix4x4 m;
    m.rotate(360.0f / m_slices, v_Y );
    QVector3D vNextRadius = m * vRadius;
    m_firstPoint = pointsSize();
    //make the surface as a series of pie pieces
    for(int i = 0; i <= m_slices; i++)
    {
        //bottom triangle, face downwards
        m_points->append(GLPoint (vCenter, -v_Y));
        m_points->append(GLPoint (vCenter + vNextRadius, -v_Y));
        m_points->append(GLPoint (vCenter + vRadius, -v_Y));

        //side triangles
        m_points->append(GLPoint(vCenter + vRadius, vRadius.normalized()));
        m_points->append(GLPoint(vCenter + vNextRadius, vNextRadius.normalized()));
        m_points->append(GLPoint(vCenter + vRadius + vUp, vRadius.normalized()));

        m_points->append(GLPoint(vCenter + vNextRadius, vNextRadius.normalized()));
        m_points->append(GLPoint(vCenter + vNextRadius + vUp, vNextRadius.normalized()));
        m_points->append(GLPoint(vCenter + vRadius + vUp, vRadius.normalized()));

        //top triangle, face upwards
        m_points->append(GLPoint (vCenter + vUp, v_Y));
        m_points->append(GLPoint (vCenter + vRadius + vUp, v_Y));
        m_points->append(GLPoint (vCenter + vNextRadius + vUp, v_Y));

        vRadius = vNextRadius;
        if(i < m_slices - 1){
            m.setToIdentity();
            m.rotate((i + 1) * 360.0f / m_slices, v_Y);
            vNextRadius = m * (v_X * m_radius);
        }
        else vNextRadius = (v_X * m_radius); //last point = first point
    }
    m_nextPoint = pointsSize();
    findMinMaxCoordinates();

}


void GLDisc::draw(GLESRenderer *renderer, bool useBuffers)
{
    GLBody::draw(renderer,useBuffers);
    if(isKing()){
        renderer->pushMvMatrix();
        renderer->translate(v_Y * 2.0f);
        GLBody::draw(renderer,useBuffers);
        renderer->popMvMatrix();
    }
}

void GLDisc::calculateDrawMatrix()
{
  m_drawMatrix = m_transformationMatrix;
  m_drawMatrix.translate(m_liftVector);
}

QVector3D GLDisc::getFieldCoord() const
{
    return m_FieldCoord;
}

void GLDisc::setFieldCoord(const QVector3D &FieldCoord)
{
    m_FieldCoord = FieldCoord;
}

void GLDisc::becomeKing()
{
    m_height = 2* m_height;
    m_isKing = true;
    m_surfaceIsValid = false;
}

bool GLDisc::isColliding(const GLDisc *other)
{
    if(this == other)
        return false;

    if((getCenter() - other->getCenter()).length() > getRadius() + other->getRadius())
        return false;
    else
    {
        if(fabs(getCenter().y() - other->getCenter().y()) >((m_height + other->m_height) / 2.0))
            return false;
        else return true;
    }
}

void GLDisc::jumpUp()
{
    m_finalLiftVector = v_Y;
    m_startLiftVector = v_Zero;
}

void GLDisc::jumpDown()
{
    m_finalLiftVector = v_Zero;
    m_startLiftVector = m_liftVector;
}

QVector3D GLDisc::getDisc_Coordinates() const
{
    return m_disc_Coordinates;
}

void GLDisc::setDisc_Coordinates(const QVector3D &disc_Coordinates)
{
    m_disc_Coordinates = disc_Coordinates;
}

QString GLDisc::getDisc_Color() const
{
    return m_disc_Color;
}

void GLDisc::setDisc_Color(const QString &disc_Color)
{
    m_disc_Color = disc_Color;
}

QString GLDisc::getDisc_Name() const
{
    return m_disc_Name;
}

QString GLDisc::getDiskLastmove() const
{
    return diskLastmove;
}

void GLDisc::setDiskLastmove(const QString &value)
{
    diskLastmove = value;
}

QVector3D GLDisc::getDisc_MoveCoordinates() const
{
    return m_disc_MoveCoordinates;
}

void GLDisc::setDisc_MoveCoordinates(const QVector3D &disc_MoveCoordinates)
{
    m_disc_MoveCoordinates = disc_MoveCoordinates;
}

void GLDisc::setIsMoved(bool value)
{
    isMove = value;
}

QVector3D GLDisc::getStartCoordinates() const
{
    return startCoordinates;
}

void GLDisc::setStartCoordinates(const QVector3D &value)
{
    startCoordinates = value;
}

QVector3D GLDisc::getMoveCoordinates() const
{
    return moveCoordinates;
}

void GLDisc::setMoveCoordinates(const QVector3D &value)
{
    moveCoordinates = value;
}

QVector3D GLDisc::getEndCoordinates() const
{
    return endCoordinates;
}

void GLDisc::setEndCoordinates(const QVector3D &value)
{
    endCoordinates = value;
}

QVector3D GLDisc::getTempCoordinates() const
{
    return tempCoordinates;
}

void GLDisc::setTempCoordinates(const QVector3D &value)
{
    tempCoordinates = value;
}

QString GLDisc::getDx() const
{
    return dx;
}

void GLDisc::setDx(const QString &value)
{
    dx = value;
}

QString GLDisc::getDz() const
{
    return dz;
}

void GLDisc::setDz(QString value)
{
    dz = value;
}

QString GLDisc::getDx_temp() const
{
    return dx_temp;
}

void GLDisc::setDx_temp(const QString &value)
{
    dx_temp = value;
}

QString GLDisc::getDz_temp() const
{
    return dz_temp;
}

void GLDisc::setDz_temp(QString value)
{
    dz_temp = value;
}

void GLDisc::setXZ()
{
    float x = getStartCoordinates().x();
    float z = getStartCoordinates().z();
    // X-Werte
    if (x == -4.5f){
        setDx("A");
        setDx_temp("A");
    }
    if (x == -1.5f){
        setDx("B");
        setDx_temp("B");
    }
    if (x == 1.5f){
        setDx("C");
        setDx_temp("C");
    }
    if (x == 4.5f){
        setDx("D");
        setDx_temp("D");
    }
    // Z-Werte
    if (z == 7.5f){
        setDz("1");
        setDz_temp("1");
    }
    if (z == 4.5f){
        setDz("2");
        setDz_temp("2");
    }
    if (z == 1.5f){
        setDz("3");
        setDz_temp("3");
    }
    if (z == -1.5f){
        setDz("4");
        setDz_temp("4");
    }
    if (z == -4.5f){
        setDz("5");
        setDz_temp("5");
    }
    if (z == -7.5f){
        setDz("6");
        setDz_temp("6");
    }
}

void GLDisc::updateXZ()
{
    setDx(getDx_temp());
    setDz(getDz_temp());
}

QList<QString> GLDisc::getList()
{
    QString listName = getDx() + getDz();
    QList<QString> list = {};
    if (listName == "A1"){
        list = A1;
    }
    if (listName == "A2"){
        list = A2;
    }
    if (listName == "A3"){
        list = A3;
    }
    if (listName == "A4"){
        list = A4;
    }
    if (listName == "B1"){
        list = B1;
    }
    if (listName == "B2"){
        list = B2;
    }
    if (listName == "B3"){
        list = B3;
    }
    if (listName == "A4"){
        list = B4;
    }
    if (listName == "C1"){
        list = C1;
    }
    if (listName == "C2"){
        list = C2;
    }
    if (listName == "C3"){
        list = C3;
    }
    if (listName == "C4"){
        list = C4;
    }
    if (listName == "D1"){
        list = D1;
    }
    if (listName == "D2"){
        list = D2;
    }
    if (listName == "D3"){
        list = D3;
    }
    if (listName == "D4"){
        list = D4;
    }
    return list;
}

QVector3D GLDisc::getVector(QList<QString> list)
{
    QVector3D vector = {0.0f, 0.0f, 0.0f};
    QString Coord = getDx_temp() + getDz_temp();
    int pos = 0;
    for (int i = 0; i < list.size(); i++){
        if (list[i] == Coord) {
            qDebug() << "Coord: " << Coord << ", pos: " << i;
            pos = i;
        }
    }
    QString bucstabe = getDx();
    QString zahl = getDz();
    if (bucstabe == "A"){
        if (zahl == "1"){
            qDebug() << "left_down";
            vector = left_down[pos];
        } else if (zahl == "6") {
            qDebug() << "left_top";
            vector = left_top[pos];
        } else {
            qDebug() << "left_centre";
            vector = left_centre[pos];
        }
    } else if (bucstabe == "D"){
        if (zahl == "1"){
            qDebug() << "right_down";
            vector = right_down[pos];
        } else if (zahl == "6") {
            qDebug() << "right_top";
            vector = right_top[pos];
        } else {
            qDebug() << "right_centre";
            vector = right_centre[pos];
        }
    } else {
        if (zahl == "1"){
            qDebug() << "middle_down";
            vector = middle_down[pos];
        } else if (zahl == "6") {
            qDebug() << "middle_top";
            vector = middle_top[pos];
        } else {
            qDebug() << "middle_centre";
            vector = middle_centre[pos];
        }
    }
    setStepVector(-vector);
    return vector;
}

bool GLDisc::isMovementOk()
{
    QString moveCoor = getDx_temp() + getDz_temp();
    QList<QString> moveList = getList();
    for (int i = 0; i < moveList.size(); i++){
        if (moveList[i] == moveCoor) {
            return true;
        }
    }
    return false;
}

void GLDisc::backStep()
{
    qDebug() << stepVector;
    move(stepVector);
}

QVector3D GLDisc::getStepVector() const
{
    return stepVector;
}

void GLDisc::setStepVector(const QVector3D &value)
{
    stepVector = value;
}

void GLDisc::finishAnimation()
{
    m_liftVector = m_finalLiftVector;
}

void GLDisc::updateAnimatedProperties(float animationState)
{
    m_liftVector = m_startLiftVector + localAnimationState(animationState) *(m_finalLiftVector - m_startLiftVector);
}

bool GLDisc::isMovementOk(GLDisc * disc, const QVector3D vMove, GLField * m_field)
{
    //QVector3D startingPosition = m_field->fieldToPosition(disc->getFieldCoord());
    QVector3D startingPosition = disc->getDisc_Coordinates();
    QVector3D targetPosition = vMove + disc->getDisc_Coordinates();

    QVector3D vTotalMove = targetPosition - startingPosition;

    int forward = 1.0;

    qDebug() << "startingPosition: " << startingPosition;
    qDebug() << "targetPosition: " << targetPosition;
    qDebug() << "vTotal Move: " << vTotalMove;

    disc->setFieldCoord(targetPosition);

    if(vTotalMove.z() < 3){
        return true;
    } else {
        return false;
    }


    /*
    int forward = 1.0;
    if(xzMovement(vTotalMove)) //we want to go straight ahead or sideways
        return true;

    if(vTotalMove.z() * forward < 0.0)
        return false;
    //Check for board limits
    //if(!m_field->isOnBoard( m_field->fieldToPosition( disc->getFieldCoord()) + vTotalMove))
    //    return false;

    //Get coordinates of jumpOverField and landingField in moving direction
    //LDisc * otherDisc = NULL;



    return true;




/*
    QVector3D startingPosition = m_field->fieldToPosition(disc->getFieldCoord());
    QVector3D targetPosition = vMove + disc->getCenter();
    QVector3D vTotalMove = targetPosition - startingPosition;

    QList<GLDisc *> * ourDiscs;
    QList<GLDisc *> * otherDiscs;
    int forward = 1.0; //forward direction for white discs i +Z, for black discs -Z
    /*
    if(disc->getColor() == whiteColor) //who are we
    {
       ourDiscs = &whiteDiscs;
       otherDiscs = &blackDiscs;
       forward = 1.0; //forward = Z
    }
    else
    {
        ourDiscs = &blackDiscs;
        otherDiscs = &whiteDiscs;
        forward = -1.0; //forward = -Z
    }
    */

   //Check for moving straight ahead
    //  if(xzMovement(vTotalMove)) //we want to go straight ahead or sideways
 //     return true;

    //we want to move diagonally
    //vTotalMove = makeDiagonalMovement(vTotalMove);
    //Check for backwards movement
    /*
    if(! disc->isKing() && (vTotalMove.z() * forward < 0.0))
        return false;
    //Check for board limits
    if(!chessBoard->isOnBoard( chessBoard->fieldToPosition( disc->getFieldCoord()) + vTotalMove))
        return false;
    */
    //Get coordinates of jumpOverField and landingField in moving direction
    //LDisc * otherDisc = NULL;



    //return true;
}

bool GLDisc::xzMovement(const QVector3D & vMove)
{
    GLfloat x = vMove.x();
    GLfloat z = vMove.z();
    return fabs(z) > 1.0 * fabs(x) || fabs(x) > 1.0 * fabs(z); //we want to go straight ahead or sideways
}

/** Returns a vector with |x| = |z|
  */
QVector3D GLDisc::makeDiagonalMovement(QVector3D vMove)
{
    bool negativeZ = vMove.z() < 0.0;
    bool negativeX = vMove.x() < 0.0;
    GLfloat absX = fabs(vMove.x());
    GLfloat absZ = fabs(vMove.z());
    GLfloat x = vMove.x();
    GLfloat z = vMove.z();
    if(absX > absZ)
    {
        if(negativeZ)
          z = -absX;
        else
          z = absX;
    }
    else
    {
        if(negativeX)
          x = -absZ;
        else
          x = absZ;
    }
    return QVector3D(x, 0.0, z);
}


