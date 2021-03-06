#include "music.h"
#include <QFile>

/**
* \fn music::music(QObject *parent)
* \brief Konstruktor für Soundklasse.
*/
music::music(QObject *parent) :
    QObject(parent)
{
    enabled = false;
}

/**
* \fn void music::playSound(const QString & fileName)
* \brief Ladet und spielt Audiodatei.
* \param fileName a QString
*/
void music::playSound(const QString & fileName)
{
    if(enabled)
    {
        QMap<QString, QSound * >::iterator it = m_sounds.find(fileName);
        if(it == m_sounds.end())
        {
            if(!QFile::exists(fileName))
            {
                return;
            }
            QSound * sound = new QSound(fileName, this);
            it = m_sounds.insert( fileName, sound);
        }
        it.value()->play();
    }
}
