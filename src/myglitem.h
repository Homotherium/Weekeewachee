#ifndef MYGLITEM_H
#define MYGLITEM_H
#include "glitem.h"
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

    QOpenGLBuffer *m_vertexBuffer;
    QOpenGLBuffer *m_indexBuffer;

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

    QList<GLDisc*> m_whitediscs_list;
    QList<GLDisc*> m_blackdiscs_list;
    QList<QVector3D> m_blackPos;
    QList<QVector3D> m_whitePos;

    GLField *m_field;

    QTimer *alarmtimer;
    QTimer *rotatetimer;


    void jumpUp();
    void jumpDown();

    //Animation
    int m_animationStep;
    int m_totalAnimationSteps;
    bool m_animationActive;

    QMouseEvent * m_lastMouseEvent;

    QVector3D m_pressPosToDiscPos;
    QVector3D discPosition;
    QPoint endPunkt;
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

    bool kampf(GLDisc * disc);
    void moving(GLDisc * disc, QVector3D MousePos);
    void move_away(GLDisc * disc);
    QList<GLDisc*> deletediscFromList(QList<GLDisc*> m_discs_list, QString disc_name);
    void changePlayer(bool player);

    bool getPlayer() const;
    void setPlayer(bool value);

    void setdiscs();

    bool getIsMoveCorrect() const;
    void setIsMoveCorrect(bool value);
    void collisionKampf(GLDisc * disc1, GLDisc * disc2);
    bool gameOverTest();
    bool discCollision(GLDisc * disc);
    void rotateBoard();
    void turnEnd();
    void showErrorMesage(QString errorMessage);
    void spielNeustarten();
    bool besetzt(QString start, QString zelle, QString disc_name, QList<GLDisc*> frends_list, QList<GLDisc*> enemy_list);

signals:
    void textChanged(const QString & text);
    void textColorChanged(const QString & color);
    void textBackgroundColorChanged(const QString & color);
    void errorIcon(const bool & visibility);
    void errorMessage(const QString & text);

public slots:
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseMoved(int x, int y, int button);
    void alarmOff() {emit errorIcon(false); emit errorMessage("");}

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
