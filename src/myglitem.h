/*!
 * \class MyGLItem myglitem.h "src/myglitem.h"
 * \brief Itemklasse für das Spiel.
 * \author Baranov Kostyantyn
 * \author Behrenbeck David
 * \author Peddinghaus Mike Frank
 * \version 1.0
 * \date 27 April 2020
 */

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

    // Spielsteine
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

    // Liste der Spielsteine
    QList<GLDisc*> m_whitediscs_list;
    QList<GLDisc*> m_blackdiscs_list;
    // Liste der Startpositionen der Spielsteine
    QList<QVector3D> m_blackPos;
    QList<QVector3D> m_whitePos;

    // Spielfeld
    GLField *m_field;

    QTimer *alarmtimer;



    //Animation
    int m_animationStep;
    int m_totalAnimationSteps;
    bool m_animationActive;
    void jumpUp();
    void jumpDown();

    QMouseEvent * m_lastMouseEvent;

    QVector3D discPosition;
    bool m_mouseEventProcessed;
    bool m_mouseRaySet;

    // Spieler
    bool player;
    // Darf sich Spielstein bewegen
    bool isMoveCorrect;
    // Ist es erster Spielstart
    bool newgame;
    // Sounds
    music * m_sounds;

private:
    GLMouseRay * m_mouseRay;

public:
    MyGLItem();
    void paintUnderQmlScene() Q_DECL_OVERRIDE;
    void paintOnTopOfQmlScene() Q_DECL_OVERRIDE;
    void setupGeometry() Q_DECL_OVERRIDE;
    void setDiscs();
    void moving(GLDisc * disc, QVector3D MousePos);
    bool isFree(QString start, QString zelle, QString disc_name, QList<GLDisc*> frends_list, QList<GLDisc*> enemy_list);
    bool figth(GLDisc * disc);
    void move_away(GLDisc * disc);
    QList<GLDisc*> deletediscFromList(QList<GLDisc*> m_discs_list, QString disc_name);
    void turnEnd();
    bool isGameOver();
    void changePlayer(bool player);
    void rotateBoard();
    void restartGame();
    void showErrorMesage(QString errorMessage);
    bool getPlayer() const;
    void setPlayer(bool value);
    bool getIsMoveCorrect() const;
    void setIsMoveCorrect(bool value);

signals:
    /**
    * \fn void textChanged(const QString & text)
    * \brief Labeltext.
    * \param text a QString
    */
    void textChanged(const QString & text);
    /**
    * \fn void textColorChanged(const QString & color)
    * \brief Labeltextfarbe.
    * \param color a QString
    */
    void textColorChanged(const QString & color);
    /**
    * \fn void textBackgroundColorChanged(const QString & color)
    * \brief Labelhintergrungfarbe.
    * \param color a QString
    */
    void textBackgroundColorChanged(const QString & color);
    /**
    * \fn void errorIcon(const bool & visibility)
    * \brief Fehlermeldung Icon.
    * \param visibility a boolean
    */
    void errorIcon(const bool & visibility);
    /**
    * \fn void errorMessage(const QString & text)
    * \brief Fehlermeldung Text.
    * \param text a QString
    */
    void errorMessage(const QString & text);

public slots:
    void mousePressed(int x, int y, int button);
    void mouseMoved(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    /**
    * \fn void alarmOff()
    * \brief Fehlermeldung ausschalten.
    */
    void alarmOff() {emit errorIcon(false); emit errorMessage("");}

protected:
    /**
     * @brief setupBuffers Create buffer objects and copy data to buffers.
     *
     */
    virtual void setupBuffers();

    /**
     * @brief doSynchronizeThreads Copy data from GUI-thread to render-thread and vice versa.
     * Virtual function to be overridden by subclasses
     */
    virtual void doSynchronizeThreads()Q_DECL_OVERRIDE;
};

#endif // MYGLITEM_H
