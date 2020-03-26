#ifndef GLSPHERE_H
#define GLSPHERE_H

#include "src/glbody.h"

class GLSphere : public GLBody
{
public:
    GLSphere(float radius, int stacks, int slices);
    void makeSurface(QVector<GLPoint> * pointContainer, QVector<GLushort> *indexContainer) Q_DECL_OVERRIDE;
protected:
    int m_stacks;
    int m_slices;
};

#endif // GLSPHERE_H
