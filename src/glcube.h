#ifndef GLCUBE_H
#define GLCUBE_H
#include "glbody.h"

class GLCube : public GLBody
{
public:
    GLCube(const QString & name, float radius = 1.0, const GLColorRgba & color = GLColorRgba::clBlue, const QString textureFile = "",
           const QVector3D & lbb = v_Zero, const QVector3D rtf = v_XYZ);

    void makeSurface(QVector<GLPoint> * pointContainer, QVector<GLushort> *indexContainer) Q_DECL_OVERRIDE;
private:
    QVector3D m_lbb; //left bottom back
    QVector3D m_rtf; //right top front
};

#endif // GLCUBE_H
