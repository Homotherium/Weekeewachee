#include "gldisc2.h"

GLDisc2::GLDisc2(float radius, float height) : GLBody ("Disc")
{
    m_radius = radius;
    m_height = height;
    setTextureFile("/home/d/Workspace/Cpp/Qt/OpenGL/danny_meier_mm2019/models/mstein_komplett_256.png");
    createTextureObjects();

    glPointSize(3.0f);
    setDrawingMode(GL_TRIANGLE_STRIP);
    //glPolygonMode(GL_FRONT, GL_LINE); glPolygonMode(GL_BACK, GL_LINE);
}

#include <iostream>
void GLDisc2::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    GLBody::makeSurface(pointContainer, indexContainer);
    m_firstPoint = m_points->size();
    // create points
    QMatrix4x4 matrix{};
    int slices = 16;
    QVector3D v_normal_down{0.0, -1.0, 0.0};
    QVector3D v_normal_up{0.0, 1.0, 0.0};

    // Unterseite
    for(int k=0; k<slices; ++k)
    {
        m_points->append(GLPoint{QVector3D{0.0f, 0.0f, 0.0f}, v_normal_down, QVector3D{0.5f, 0.59f, 0.0f}, GLColorRgba::clCyan});
    }
    // Mantel unten, normals unten
    matrix.setToIdentity();
    matrix.setToIdentity();
    QMatrix4x4 mat_tex{}; mat_tex.setToIdentity();
    for(int j=0; j<slices; ++j)
    {
        QVector3D point = QVector3D{m_radius, 0.0f, 0.0f} * matrix;
        QVector3D v_tex{0.5f, 0.0f, 0.0f};
        v_tex = v_tex * mat_tex;
        m_points->append(GLPoint{point, v_normal_down, QVector3D{(v_tex.x()+0.5f), (v_tex.y()+0.73f)*0.8f, 0.0f}, GLColorRgba::clCyan});
        matrix.rotate(360.0f/slices, v_Y);
        mat_tex.rotate(360.0f/slices,v_Z);
    }
    // Mantel unten, normals zur seite
    matrix.setToIdentity();
    for(int j=0; j<slices; ++j)
    {
        QVector3D point = QVector3D{m_radius, 0.0f, 0.0f} * matrix;
        QVector3D v_normal = QVector3D{m_radius, 0.0, 0.0}.normalized() * matrix;
        int a = j % 2 + 1;
        float tx =  ((float)a)/((float)slices-1.0f);
        float ty = 0.0f;
        m_points->append(GLPoint{point, v_normal, QVector3D{tx, ty, 0.0f}, GLColorRgba::clCyan});
        matrix.rotate(360.0f/slices, v_Y);
    }
    // Mantel oben, normals zur seite
    matrix.setToIdentity();
    for(int j=0; j<slices; ++j)
    {
        QVector3D point = QVector3D{m_radius, m_height, 0.0f} * matrix;
        QVector3D v_normal = QVector3D{m_radius, 0.0, 0.0}.normalized() * matrix;
        int a = j % 2 + 1;
        float tx =  (((float)a)/((float)slices-1.0f));
        float ty = 0.172f;
        m_points->append(GLPoint{point, v_normal, QVector3D{tx, ty, 0.0f}, GLColorRgba::clCyan});
        matrix.rotate(360.0f/slices, v_Y);
    }
    // Mantel oben, normals oben

    matrix.setToIdentity();
    mat_tex.setToIdentity();
    for(int j=0; j<slices; ++j)
    {
        QVector3D point = QVector3D{m_radius, m_height, 0.0f} * matrix;
        QVector3D v_tex{0.5f, 0.0f, 0.0f};
        v_tex = v_tex * mat_tex;
        m_points->append(GLPoint{point, v_normal_up, QVector3D{(v_tex.x()+0.5f), (v_tex.y()+0.73f)*0.8f, 0.0f}, GLColorRgba::clCyan});
        matrix.rotate(360.0f/slices, v_Y);
        mat_tex.rotate(360.0f/slices,v_Z);
    }
    // Oberseite
    for(int k=0; k<slices; ++k)
    {
        m_points->append(GLPoint{QVector3D{0.0f, m_height, 0.0f}, v_normal_up, QVector3D{0.5f, 0.59f, 0.0f}, GLColorRgba::clCyan});
    }


    m_nextPoint = m_points->size();

    m_firstIndex = m_indices->size();
    // change indices
    for(int i=0; i<5; ++i)
    {
        for (int j = 0; j<slices; ++j)
        {
            m_indices->append(slices*i+j);
            m_indices->append(slices*(i+1)+j);
        }
    }
    m_nextIndex = m_indices->size();

    std::cout << "disc2 indices:" << m_indices->size() << std::endl;

}
