#include "glsphere.h"

GLSphere::GLSphere(float radius, int stacks, int slices) : GLBody("mySphere",radius)
{
    m_stacks = stacks;
    m_slices = slices;
    m_drawingMode = GL_LINE_STRIP;
}

void GLSphere::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    GLBody::makeSurface(pointContainer, indexContainer);
    m_firstPoint = static_cast<GLushort>(m_points->size());
    m_firstIndex = static_cast<GLushort>(m_indices->size());
    m_points->append(GLPoint(-v_Y * m_radius, -v_Y, QVector3D(0.5,0.0,0.0), m_color)); //south pole
    QMatrix4x4 zRotator;
    QMatrix4x4 yRotator;
    for(int slice = 0; slice < m_slices; slice++){
        yRotator.setToIdentity();
        yRotator.rotate(slice * 360.0f / static_cast<float>(m_slices), v_Y);

        for(int stack = 0; stack <= m_stacks; stack ++){
            QVector3D texCoord = QVector3D(slice * 1.0f / m_slices, stack * 1.0f / m_stacks, 0.0f);
            QVector3D vertex = -v_Y;
            zRotator.setToIdentity();
            float zAngle = stack * 180.0f / static_cast<float>(m_stacks);
            zRotator.rotate(zAngle, v_Z);
            vertex= yRotator * zRotator * vertex;
            m_points->append(GLPoint(vertex * m_radius, vertex,texCoord, m_color));
        }
    }

    //add last column as a copy of the first column.
    //however change texCoords to end of texture
    for (int stack = 0; stack < m_stacks; stack++){
        int i = m_firstPoint + stack + 1;
        QVector3D texCoord = QVector3D(1.0f, stack * 1.0f / m_stacks, 0.0);
        GLPoint point = GLPoint(m_points->at(i).vertex(), m_points->at(i).normal(), texCoord, m_color);
        m_points->append(point);
    }

    if(m_drawingMode != GL_POINTS)
    {

        for(GLushort slice = 0; slice < m_slices; slice += 2){

            GLushort stack = 0;
            for(stack = 0; stack <= m_stacks; stack++){
                m_indices->append(slice * (m_stacks + 1) + stack + m_firstPoint);
                m_indices->append((slice + 1) * (m_stacks + 1) + stack + m_firstPoint);
            }
            m_indices->append((slice + 1) * (m_stacks + 1) + stack + m_firstPoint);
            for(int stack = m_stacks; stack >= 0; stack--){
                m_indices->append((slice + 2) * (m_stacks + 1) + static_cast<GLushort>(stack) + m_firstPoint);
                m_indices->append((slice + 1) * (m_stacks + 1) + static_cast<GLushort>(stack) + m_firstPoint);
            }
            m_indices->append((slice + 2) * (m_stacks + 1) + stack + m_firstPoint);
        }
    }
    m_nextPoint = static_cast<GLushort>(m_points->size());
    m_nextIndex = static_cast<GLushort>(m_indices->size());
}
