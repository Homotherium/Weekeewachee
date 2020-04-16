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

    int getDx() const;
    void setDx(int value);

    int getDz() const;
    void setDz(int value);

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
    int dx;
    int dz;

    //Animation
    QVector3D m_liftVector;
    QVector3D m_finalLiftVector;
    QVector3D m_startLiftVector;

};

#endif // GLDISC_H
