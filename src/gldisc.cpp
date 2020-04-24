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
    discLastmove = "";
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

QString GLDisc::getdiscLastmove() const
{
    return discLastmove;
}

void GLDisc::setdiscLastmove(const QString &value)
{
    discLastmove = value;
}

void GLDisc::setIsMoved(bool value)
{
    isMove = value;
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
    // Weise Steine
    if (getHoldCoordinates() == QVector3D(-4.5f, 0.0f, 7.5f)){
        setDXZ("A1");
        setDXZ_temp("A1");
    }
    if (getHoldCoordinates() == QVector3D(-1.5f, 0.0f, 7.5f)){
        setDXZ("B1");
        setDXZ_temp("B1");
    }
    if (getHoldCoordinates() == QVector3D(1.5f, 0.0f, 7.5f)){
        setDXZ("C1");
        setDXZ_temp("C1");
    }
    if (getHoldCoordinates() == QVector3D(4.5f, 0.0f, 7.5f)){
        setDXZ("D1");
        setDXZ_temp("D1");
    }
    // Schwarze Steine
    if (getHoldCoordinates() == QVector3D(-4.5f, 0.0f, -7.5f)){
        setDXZ("A6");
        setDXZ_temp("A6");
    }
    if (getHoldCoordinates() == QVector3D(-1.5f, 0.0f, -7.5f)){
        setDXZ("B6");
        setDXZ_temp("B6");
    }
    if (getHoldCoordinates() == QVector3D(1.5f, 0.0f, -7.5f)){
        setDXZ("C6");
        setDXZ_temp("C6");
    }
    if (getHoldCoordinates() == QVector3D(4.5f, 0.0f, -7.5f)){
        setDXZ("D6");
        setDXZ_temp("D6");
    }
}

void GLDisc::updateXZ()
{
    setDXZ(getDXZ_temp());
}

QList<QString> GLDisc::getList()
{
    QString listName = getDXZ();
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
    if (listName == "A5"){
        list = A5;
    }
    if (listName == "A6"){
        list = A6;
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
    if (listName == "B4"){
        list = B4;
    }
    if (listName == "B5"){
        list = B5;
    }
    if (listName == "B6"){
        list = B6;
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
    if (listName == "C5"){
        list = C5;
    }
    if (listName == "C6"){
        list = C6;
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
    if (listName == "D5"){
        list = D5;
    }
    if (listName == "D6"){
        list = D6;
    }
    return list;
}

QVector3D GLDisc::getVector(QList<QString> list, QString moveXZ)
{
    QVector3D vector = {0.0f, 0.0f, 0.0f};
    int pos = 0;
    for (int i = 0; i < list.size(); i++){
        if (list[i] == moveXZ) {
            qDebug() << "Coord: " << moveXZ << ", pos: " << i;
            pos = i;
        }
    }
    QString startPos = getDXZ();
    QRegExp zelle;
    zelle.setPattern("A1");
    if (zelle.exactMatch(startPos)){
        qDebug() << "left_down";
        vector = left_down[pos];
    }
    zelle.setPattern("A[2-5]");
    if (zelle.exactMatch(startPos)){
        qDebug() << "left_centre";
        vector = left_centre[pos];
    }
    zelle.setPattern("A6");
    if (zelle.exactMatch(startPos)){
        qDebug() << "left_top";
        vector = left_top[pos];
    }
    zelle.setPattern("[B-C]1");
    if (zelle.exactMatch(startPos)){
        qDebug() << "middle_down";
        vector = middle_down[pos];
    }
    zelle.setPattern("[B-C][2-5]");
    if (zelle.exactMatch(startPos)){
        qDebug() << "middle_centre";
        vector = middle_centre[pos];
    }
    zelle.setPattern("[B-C]6");
    if (zelle.exactMatch(startPos)){
        qDebug() << "middle_top";
        vector = middle_top[pos];
    }
    zelle.setPattern("D1");
    if (zelle.exactMatch(startPos)){
        qDebug() << "right_down";
        vector = right_down[pos];
    }
    zelle.setPattern("D[2-5]");
    if (zelle.exactMatch(startPos)){
        qDebug() << "right_centre";
        vector = right_centre[pos];
    }
    zelle.setPattern("D6");
    if (zelle.exactMatch(startPos)){
        qDebug() << "right_top";
        vector = right_top[pos];
    }
    qDebug() << "add Vector: " << vector;
    setStepVector(-vector);
    return vector;
}

bool GLDisc::isMovementOk(QString moveXZ)
{
    qDebug() << "moveCoor:" << moveXZ;
    QList<QString> moveList = getList();
    qDebug() << "moveList:" << moveList;
    for (int i = 0; i < moveList.size(); i++){
        if (moveList[i] == moveXZ) {
            return true;
        }
    }
    return false;
}

bool GLDisc::isFigth(QString enemy)
{
    qDebug() << "isFigth";
    QList<QString> moveList = getList();
    qDebug() << "moveList:" << moveList;
    for (int i = 0; i < moveList.size(); i++){
        if (moveList[i] == enemy){
            qDebug() << "Gegener ist erreichtbar";
            return true;
        }
    }
    qDebug() << "Gegener ist nicht erreichtbar";
    return false;
}

void GLDisc::backStep()
{
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

void GLDisc::getInfo()
{
    qDebug() << "";
    qDebug() << "+++++++++disc Info+++++++++";
    qDebug() << "disc Name: " << getDisc_Name();
    qDebug() << "disc Farbe: " << getDisc_Color();
    qDebug() << "disc holdCoordinates: " << getHoldCoordinates();
    qDebug() << "disc moveCoordinates: " << getMoveCoordinates();
    qDebug() << "disc XZ: " << getXZ();
    qDebug() << "disc moveList: " << getList();
    qDebug() << "disc XZ_temp: " << getXZ_temp();
    qDebug() << "disc stepVector: " << getStepVector();
    qDebug() << "";


}

QVector3D GLDisc::getHoldCoordinates() const
{
    return holdCoordinates;
}

void GLDisc::setHoldCoordinates(const QVector3D &value)
{
    holdCoordinates = value;
}

QVector3D GLDisc::getMoveCoordinates() const
{
    return moveCoordinates;
}

void GLDisc::setMoveCoordinates(const QVector3D &value)
{
    moveCoordinates = value;
}

QVector3D GLDisc::getFinalLiftVector() const
{
    return m_finalLiftVector;
}

QString GLDisc::getDXZ() const
{
    return dXZ;
}

void GLDisc::setDXZ(const QString &value)
{
    dXZ = value;
}

QString GLDisc::getDXZ_temp() const
{
    return dXZ_temp;
}

void GLDisc::setDXZ_temp(const QString &value)
{
    dXZ_temp = value;
}

void GLDisc::finishAnimation()
{
    m_liftVector = m_finalLiftVector;
}

void GLDisc::updateAnimatedProperties(float animationState)
{
    m_liftVector = m_startLiftVector + localAnimationState(animationState) *(m_finalLiftVector - m_startLiftVector);
}
