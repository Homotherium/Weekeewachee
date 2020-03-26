#ifndef GLDISC2_H
#define GLDISC2_H

#include "glbody.h"

class GLDisc2 : public GLBody
{
public:
    GLDisc2(float radius, float height);

    void setHeight(float height);
    void setRadius(float radius);
    void makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer) override;

private:
    float m_height;

};

#endif // GLDISC2_H
