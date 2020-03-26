#ifndef MYGLITEM_H
#define MYGLITEM_H
#include "glitem.h"
#include "glcube.h"
#include "glsphere.h"
#include <QOpenGLBuffer>
#include <QtGlobal>
#include "gldisc.h"
#include "glfield.h"
#include "glmouseray.h"

class MyGLItem : public GLItem
{
    Q_OBJECT

    int m_firstTrianglePoint;
    int m_lastTrianglePoint;
    int m_firstCubePoint;
    int m_lastCubePoint;
    int m_firstFPoint;
    int m_lastFPoint;
    int m_firstNormalPoint;
    int m_lastNormalPoint;
    GLCube *m_cube;
    GLSphere *m_sphere;
    QOpenGLBuffer *m_vertexBuffer;
    QOpenGLBuffer * m_indexBuffer;

    GLDisc *m_disc;
    GLDisc *m_disc_other;

    GLDisc *m_disc_white_schere;
    GLDisc *m_disc_white_stein;
    GLDisc *m_disc_white_papier;
    GLDisc *m_disc_white_brunnen;

    GLDisc *m_disc_black_schere;
    GLDisc *m_disc_black_stein;
    GLDisc *m_disc_black_papier;
    GLDisc *m_disc_black_brunnen;

    GLField *m_field;

    void createTriangles();
    void drawTriangles();
    void createCube(const QVector3D & llb, const QVector3D & rtf);
    void createNormals();
    void createNormalsF();
    void drawCube();
    void createF(float height);
    void drawF();
    void drawF2(float height);

    void jumpUp();
    void jumpDown();

    //Animation
    int m_animationStep;
    int m_totalAnimationSteps;
    bool m_animationActive;

    QMouseEvent * m_lastMouseEvent;

    QVector3D m_pressPosToDiscPos;
    bool m_mouseEventProcessed;
    bool m_mouseRaySet;

private:
    GLMouseRay * m_mouseRay;

public:
    //explicit MyGLItem(QQuickItem *parent = 0);
    MyGLItem();
    void paintUnderQmlScene() Q_DECL_OVERRIDE;
    void paintOnTopOfQmlScene() Q_DECL_OVERRIDE;
    void setupGeometry() Q_DECL_OVERRIDE;

public slots:
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseMoved(int x, int y, int button);

protected:
    /**
    * @brief wheelEvent zoom in/out
    * @param e
    */
    void wheelEvent (QWheelEvent * e) Q_DECL_OVERRIDE;
    /**
     * @brief doSynchronizeThreads Copy data from GUI-thread to render-thread and vice versa.
     * Virtual function to be overridden by subclasses
     */
    virtual void doSynchronizeThreads()Q_DECL_OVERRIDE;

    /**
     * @brief setupBuffers Create buffer objects and copy data to buffers.
     *
     */
    virtual void setupBuffers();





};

#endif // MYGLITEM_H
