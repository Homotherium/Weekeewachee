#ifndef GLDISC_H
#define GLDISC_H

#include "glbody.h"
#include "glfield.h"
class GLDisc : public GLBody
{
public:
    GLDisc(const QString &name, const QVector3D & fieldCoord, float m_radius = 1.0, const GLColorRgba & m_color = GLColorRgba::clBlue, const QString m_textureFile = "",
           float height = 0.5, IndexType slices = 36);
    /**
    *
    * @param pointContainer The container for the geometry data. If NULL, a new one is created.
    * @param indexContainer The container for the index data. If NULL, a new one is created.
    * Created containers will be deleted by destructor.
    */
    void makeSurface(QVector<GLPoint> * pointContainer = nullptr, QVector<GLushort> *indexContainer = nullptr)Q_DECL_OVERRIDE;

    void draw(GLESRenderer *renderer, bool useBuffers = false)Q_DECL_OVERRIDE;

    bool isMovementOk(GLDisc * GLDisc, const QVector3D vMove, GLField * m_field);
    /** Returns true, if movement is parallel to z or x
      */
    bool xzMovement(const QVector3D & vMove);
    /** Returns a vector with |x| = |z|
      */
    QVector3D makeDiagonalMovement(QVector3D vMove);


    float getHeight(){ return m_height;}
    QVector3D getFieldCoord() const;
    void setFieldCoord(const QVector3D &FieldCoord);
    void becomeKing();
    bool isKing() {return m_isKing;}
    /**
      * Returns true, if discs are colliding
      */
    bool isColliding(const GLDisc * other);
    void calculateDrawMatrix() Q_DECL_OVERRIDE;

    /**
     * @brief finishAnimation ensure final state
     */
    void finishAnimation() override;
    /**
     * @brief updateAnimatedProperties updates lift vector
     * @param animationState
     */
    void updateAnimatedProperties(float animationState)Q_DECL_OVERRIDE;

    void jumpUp();
    void jumpDown();

    QVector3D getDisc_Coordinates() const;
    void setDisc_Coordinates(const QVector3D &disc_Coordinates);

    QString getDisc_Color() const;
    void setDisc_Color(const QString &disc_Color);

    QString getDisc_Name() const;
    QString diskLastmove;

    QString getDiskLastmove() const;
    void setDiskLastmove(const QString &value);

    QVector3D getDisc_MoveCoordinates() const;
    void setDisc_MoveCoordinates(const QVector3D &disc_MoveCoordinates);

    bool isMoved(){return isMove;}
    void setIsMoved(bool value);

    QVector3D getStartCoordinates() const;
    void setStartCoordinates(const QVector3D &value);

    QVector3D getMoveCoordinates() const;
    void setMoveCoordinates(const QVector3D &value);

    QVector3D getEndCoordinates() const;
    void setEndCoordinates(const QVector3D &value);

    QVector3D getTempCoordinates() const;
    void setTempCoordinates(const QVector3D &value);


    QString getDx() const;
    void setDx(const QString &value);

    QString getDz() const;
    void setDz(QString value);

    QString getDx_temp() const;
    void setDx_temp(const QString &value);

    QString getDz_temp() const;
    void setDz_temp(QString value);

    void setXZ();
    void updateXZ();

    QList<QString> getList();
    QVector3D getVector(QList<QString> list);
    bool isMovementOk();
    void backStep();

    QVector3D getStepVector() const;
    void setStepVector(const QVector3D &value);

private:
    float m_height;
    IndexType m_slices;
    //QPoint m_FieldCoord;
    QVector3D startCoordinates;
    QVector3D moveCoordinates;
    QVector3D tempCoordinates;
    QVector3D endCoordinates;
    QVector3D m_FieldCoord;
    QVector3D m_disc_Coordinates;
    QVector3D m_disc_MoveCoordinates;
    QString m_disc_Name;
    QString m_disc_Color;
    bool m_isKing;
    bool isMove;
    QString dx;
    QString dz;
    QString dx_temp;
    QString dz_temp;
    QVector3D stepVector;


    //Animation
    QVector3D m_liftVector;
    QVector3D m_finalLiftVector;
    QVector3D m_startLiftVector;

    //Listen
    QList<QString> A1 = {{"A2"},{"B1"},{"B2"}};
    QList<QString> A2 = {{"A1"},{"A3"},{"B1"},{"B2"},{"B3"}};
    QList<QString> A3 = {{"A2"},{"A4"},{"B2"},{"B3"},{"B4"}};
    QList<QString> A4 = {{"A3"},{"A5"},{"B3"},{"B4"},{"B5"}};
    QList<QString> A5 = {{"A4"},{"A6"},{"B4"},{"B5"},{"B6"}};
    QList<QString> A6 = {{"A5"},{"B5"},{"B6"}};

    QList<QString> B1 = {{"A1"},{"A2"},{"B2"},{"C1"},{"C2"}};
    QList<QString> B2 = {{"A1"},{"A2"},{"A3"},{"B1"},{"B3"},{"C1"},{"C2"},{"C3"}};
    QList<QString> B3 = {{"A2"},{"A3"},{"A4"},{"B2"},{"B4"},{"C2"},{"C3"},{"C4"}};
    QList<QString> B4 = {{"A3"},{"A4"},{"A5"},{"B3"},{"B5"},{"C3"},{"C4"},{"C5"}};
    QList<QString> B5 = {{"A4"},{"A5"},{"A6"},{"B4"},{"B6"},{"C4"},{"C5"},{"C6"}};
    QList<QString> B6 = {{"A5"},{"A6"},{"B5"},{"C5"},{"C6"}};

    QList<QString> C1 = {{"B1"},{"B2"},{"C2"},{"D1"},{"D2"}};
    QList<QString> C2 = {{"B1"},{"B2"},{"B3"},{"C1"},{"C3"},{"D1"},{"D2"},{"D3"}};
    QList<QString> C3 = {{"B2"},{"B3"},{"B4"},{"C2"},{"C4"},{"D2"},{"D3"},{"D4"}};
    QList<QString> C4 = {{"B3"},{"B4"},{"B5"},{"C3"},{"C5"},{"D3"},{"D4"},{"D5"}};
    QList<QString> C5 = {{"B4"},{"B5"},{"B6"},{"C4"},{"C6"},{"D4"},{"D5"},{"D6"}};
    QList<QString> C6 = {{"B5"},{"B6"},{"C5"},{"D5"},{"D6"}};

    QList<QString> D1 = {{"C1"},{"C2"},{"D2"}};
    QList<QString> D2 = {{"C1"},{"C2"},{"C3"},{"D1"},{"D3"}};
    QList<QString> D3 = {{"C2"},{"C3"},{"C4"},{"D2"},{"D4"}};
    QList<QString> D4 = {{"C3"},{"C4"},{"C5"},{"D3"},{"D5"}};
    QList<QString> D5 = {{"C4"},{"C5"},{"C6"},{"D4"},{"D6"}};
    QList<QString> D6 = {{"C5"},{"C6"},{"D5"}};

    //Vectoren Listen
    QList<QVector3D> left_down = {{0.0f, 0.0f, +3.0f},{+3.0f, 0.0f, 0.0f},{+3.0f, 0.0f, +3.0f}};
    QList<QVector3D> left_centre = {{0.0f, 0.0f, +3.0f},{0.0f, 0.0f, -3.0f},{+3.0f, 0.0f, +3.0f},{+3.0f, 0.0f, 0.0f},{+3.0f, 0.0f, -3.0f}};
    QList<QVector3D> left_top = {{0.0f, 0.0f, +3.0f},{+3.0f, 0.0f, +3.0f},{+3.0f, 0.0f, 0.0f}};

    QList<QVector3D> middle_down = {{-3.0f, 0.0f, 0.0f},{-3.0f, 0.0f, -3.0f},{0.0f, 0.0f, -3.0f},{+3.0f, 0.0f, 0.0f},{+3.0f, 0.0f, -3.0f}};
    QList<QVector3D> middle_centre = {{-3.0f, 0.0f, +3.0f},{-3.0f, 0.0f, 0.0f},{-3.0f, 0.0f, -3.0f},{0.0f, 0.0f, +3.0f},{0.0f, 0.0f, -3.0f},{+3.0f, 0.0f, +3.0f},{+3.0f, 0.0f, 0.0f},{+3.0f, 0.0f, -3.0f}};
    QList<QVector3D> middle_top = {{-3.0f, 0.0f, +3.0f},{-3.0f, 0.0f, 0.0f},{0.0f, 0.0f, +3.0f},{+3.0f, 0.0f, +3.0f},{+3.0f, 0.0f, 0.0f}};

    QList<QVector3D> right_down = {{-3.0f, 0.0f, 0.0f},{-3.0f, 0.0f, -3.0f},{0.0f, 0.0f, -3.0f}};
    QList<QVector3D> right_centre = {{-3.0f, 0.0f, +3.0f},{-3.0f, 0.0f, 0.0f},{-3.0f, 0.0f, -3.0f},{0.0f, 0.0f, +3.0f},{0.0f, 0.0f, -3.0f}};
    QList<QVector3D> right_top = {{-3.0f, 0.0f, +3.0f},{-3.0f, 0.0f, 0.0f},{0.0f, 0.0f, +3.0f}};
};

#endif // GLDISC_H
