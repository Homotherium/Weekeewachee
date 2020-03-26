#ifndef GLPLANE_H
#define GLPLANE_H

#include "glbody.h"

class GLField : public GLBody
{
public:
    GLField();
    void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer) override;

    /**
     * parameter x = Spalte des Spielfeldes
     * parameter y = Zeile des Spielfeldes
     * ( von 0 bis 7 )
     *
     * \return Koordinaten der jeweiligen Spielfeldmitte oder (-9.0, -9.0, -9.0) bei Fehler
    */
    QVector3D fieldToPosition(const QVector3D & field);


private:
    QVector3D m_ll;
    QVector3D m_ur;
protected:
    /**
      * The normal on the chessboard plane
      */
    QVector3D m_normal;
    /**
      * The size of the black and white squares
      */
    GLfloat m_squareSize;
};

#endif // GLPLANE_H
