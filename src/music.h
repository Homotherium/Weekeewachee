/*!
 * \class music music.h "src/music.h"
 * \brief Soundklasse für das Spiel.
 * \author Baranov Kostyantyn
 * \author Behrenbeck David
 * \author Peddinghaus Mike Frank
 * \version 1.0
 * \date 27 April 2020
 */
#ifndef MUSIC_H
#define MUSIC_H

#include <QObject>
#include <QString>
#include <QSound>
#include <QMap>

class music : public QObject
{
    Q_OBJECT
public:
    explicit music(QObject *parent = nullptr);

    void setEnabled(bool newVal){enabled = newVal;}
    bool isEnabled(){return enabled;}

public slots:
    void playSound(const QString & fileName);

private:
    QMap<QString, QSound *> m_sounds;
    bool enabled;
};

#endif // MUSIC_H
