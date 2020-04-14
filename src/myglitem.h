#ifndef MYGLITEM_H
#define MYGLITEM_H
#include "glitem.h"
#include "glcube.h"
#include "glsphere.h"
#include <time.h>
#include <QOpenGLBuffer>
#include <QtGlobal>
#include "gldisc.h"
#include "glfield.h"
#include "glmouseray.h"
#include "music.h"
#include <algorithm>

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
    GLDisc *m_disc_temp;

    GLDisc *m_disc_white_schere;
    GLDisc *m_disc_white_stein;
    GLDisc *m_disc_white_papier;
    GLDisc *m_disc_white_brunnen;

    GLDisc *m_disc_black_schere;
    GLDisc *m_disc_black_stein;
    GLDisc *m_disc_black_papier;
    GLDisc *m_disc_black_brunnen;

    QList<GLDisc*> m_whitedisks_list;
    QList<GLDisc*> m_blackdisks_list;
    QList<QVector3D> m_blackPos;
    QList<QVector3D> m_whitePos;

    GLField *m_field;

    void createTriangles();
    void drawTriangles();
    void createNormals();
    void createNormalsF();
    void drawCube();

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

    bool player;
    bool isMoveCorrect;
    bool newgame;
    music * m_sounds;

private:
    GLMouseRay * m_mouseRay;

public:
    //explicit MyGLItem(QQuickItem *parent = 0);
    MyGLItem();
    void paintUnderQmlScene() Q_DECL_OVERRIDE;
    void paintOnTopOfQmlScene() Q_DECL_OVERRIDE;
    void setupGeometry() Q_DECL_OVERRIDE;

    bool kampf(GLDisc * disk, QVector3D stein, QPoint hit_coordinaten);
    void moveDisk(GLDisc * disk, QVector3D start, QVector3D end);
    void move_away(GLDisc * disk);
    void move_back(GLDisc * disk);
    QList<GLDisc*> deleteDiskFromList(QList<GLDisc*> m_disks_list, QString disk_name);
    void changePlayer(bool player);

    bool getPlayer() const;
    void setPlayer(bool value);

    void setDisks();

    bool getIsMoveCorrect() const;
    void setIsMoveCorrect(bool value);
    bool isFar(QVector3D disc, QVector3D klickPunkt);
    bool isNear(QVector3D disc, QVector3D klickPunkt);
    void collisionKampf(GLDisc * disc1, GLDisc * disc2);
    bool gameOverTest();
    bool diskCollision(GLDisc * disk);
    void rotateBoard();
    void turnEnd();
    void printDiskLists();

signals:
    void textChanged(const QString & text);
    void textColorChanged(const QString & color);
    void textBackgroundColorChanged(const QString & color);

public slots:
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseMoved(int x, int y, int button);
    void spielNeustarten();

protected:
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
