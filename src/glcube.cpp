#include "glcube.h"

GLCube::GLCube(const QString &name, float radius, const GLColorRgba &color, const QString textureFile, const QVector3D &lbb, const QVector3D rtf)
    :GLBody(name, radius, color, textureFile)
{
   m_lbb = lbb;
   m_rtf = rtf;

}

void GLCube::makeSurface(QVector<GLPoint> *pointContainer, QVector<GLushort> *indexContainer)
{
    /**
          Corner vertices of the cube

          ltb_____________rtb
            /            /|
           /            / |
       ltf/____________/rtf
          |   |        |  |
          |   |        |  |
          |lbb|________|__|rbb
          |  /         |  /
          | /          | /
       lbf|/___________|/rbf

          */
    GLBody::makeSurface(pointContainer, indexContainer);
    m_firstPoint = pointsSize();
    m_firstIndex = indicesSize();
    //corners
    QVector3D lbb = m_lbb;
    QVector3D rbb = QVector3D(m_rtf.x(), m_lbb.y(), m_lbb.z());
    QVector3D ltb = QVector3D(m_lbb.x(), m_rtf.y(), m_lbb.z());
    QVector3D lbf = QVector3D(m_lbb.x(), m_lbb.y(), m_rtf.z());
    QVector3D rtb = QVector3D(m_rtf.x(), m_rtf.y(), m_lbb.z());
    QVector3D rbf = QVector3D(m_rtf.x(), m_lbb.y(), m_rtf.z());
    QVector3D ltf = QVector3D(m_lbb.x(), m_rtf.y(), m_rtf.z());
    QVector3D rtf = m_rtf;

    //Left
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltb, -v_X, v_X * 0.125, GLColorRgba::clMagenta));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbb, -v_X, v_Zero, GLColorRgba::clMagenta));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltf, -v_X, v_Y + v_X * 0.125, GLColorRgba::clMagenta));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbf, -v_X, v_Y, GLColorRgba::clMagenta));
    m_indices->append(pointsSize()); //One degenerate triangle, we have to turn around the corner

    //Front
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbf, v_Z, v_X * 0.125, GLColorRgba::clRed));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbf, v_Z, v_X * 0.25, GLColorRgba::clRed));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltf, v_Z, v_Y + v_X * 0.125, GLColorRgba::clRed));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtf, v_Z, v_Y + v_X * 0.25, GLColorRgba::clRed));

    //Top
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtf, v_Y, v_Y+ v_X * 0.375, GLColorRgba::clYellow));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltf, v_Y, v_Y + v_X * 0.25, GLColorRgba::clYellow));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtb, v_Y, v_X * 0.375, GLColorRgba::clYellow));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltb, v_Y,  v_X * 0.25, GLColorRgba::clYellow));

    //Back
    m_indices->append(pointsSize());
    m_points->append(GLPoint(ltb, -v_Z, v_Y + v_X * 0.375, GLColorRgba::clGreen));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtb, -v_Z, v_Y + v_X * 0.5, GLColorRgba::clGreen));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbb, -v_Z, v_X * 0.375, GLColorRgba::clGreen));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbb, -v_Z, v_X * 0.5, GLColorRgba::clGreen));

    //Bottom
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbb, -v_Y,  v_X * 0.625, GLColorRgba::clBlue));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbb, -v_Y, v_X * 0.5, GLColorRgba::clBlue));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbf, -v_Y, v_Y + v_X * 0.625, GLColorRgba::clBlue));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(lbf, -v_Y, v_Y + v_X * 0.5, GLColorRgba::clBlue));
    m_indices->append(pointsSize()); //One degenerate triangle, we have to turn around the corner

    //Right
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbf, v_X, v_X * 0.625, GLColorRgba::clCyan));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rbb, v_X,  v_X * 0.75, GLColorRgba::clCyan));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtf, v_X, v_Y + v_X * 0.625, GLColorRgba::clCyan));
    m_indices->append(pointsSize());
    m_points->append(GLPoint(rtb, v_X, v_Y +  v_X * 0.75, GLColorRgba::clCyan));

    m_nextIndex = indicesSize();
    m_nextPoint = pointsSize();
    if(m_showNormals)
        createNormals();
}
