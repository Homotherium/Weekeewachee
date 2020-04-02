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

signals:

public slots:
    void playSound(const QString & fileName);

private:
    QMap<QString, QSound *> m_sounds;
    bool enabled;
};

#endif // MUSIC_H
