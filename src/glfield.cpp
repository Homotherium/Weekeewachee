#include "glfield.h"

GLField::GLField() : GLBody("Plane")
{
    m_ll = QVector3D{-6.0f, 0.0f, -9.0f};
    m_ur = QVector3D{6.0f, 0.0f, 9.0f};
    setDrawingMode(GL_TRIANGLE_STRIP);
    createTextureObjects();
}

void GLField::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{

    GLBody::makeSurface(pointContainer, indexContainer);

    m_firstPoint = m_points->size();
    /*
     * ul             ur
     * +---------------+
     * |               |
     * |               |
     * |               |
     * |               |
     * |               |
     * |               |
     * +---------------+
     * ll             lr
     */
    QVector3D ll{m_ll};
    QVector3D lr{m_ur.x(), 0.0f, m_ll.z()};
    QVector3D ul{m_ll.x(), 0.0f, m_ur.z()};
    QVector3D ur{m_ur};

    m_points->append(GLPoint{ll, v_Y, QVector3D{0.0f, 0.0f, 0.0f}, GLColorRgba::clCyan}); unsigned short ill = m_points->size()-1;
    m_points->append(GLPoint{ul, v_Y, QVector3D{0.0f, 1.0f, 0.0f}, GLColorRgba::clRed}); unsigned short iul = m_points->size()-1;
    m_points->append(GLPoint{ur, v_Y, QVector3D{1.0f, 1.0f, 0.0f}, GLColorRgba::clCyan}); unsigned short iur = m_points->size()-1;
    m_points->append(GLPoint{lr, v_Y, QVector3D{1.0f, 0.0f, 0.0f}, GLColorRgba::clRed}); unsigned short ilr = m_points->size()-1;

    m_nextPoint = m_points->size();

    m_firstIndex = m_indices->size();

    m_indices->append(ill);
    m_indices->append(iul);
    m_indices->append(ilr);
    m_indices->append(iur);

    m_nextIndex = m_indices->size();
}

QVector3D GLField::fieldToPosition(const QVector3D &field)
{
    QVector3D result;
    QVector3D vx = v_X * m_squareSize;
    QVector3D vz = v_Z * m_squareSize;
    qDebug() <<"Test field to Position";
    result = (field.x() - 3.5f) * vx + (field.y() - 3.5f) * vz;
    qDebug() <<"Test2 field to Position";
#ifdef DEBUG_CHESSBOARD

    ShaderDebugger::debugVector3D(result, "Field" + QString::number(field.x()) +"," + QString::number(field.y())
                       + " is at position ");
#endif
    return result;
}



