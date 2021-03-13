#ifndef VIRTUALPLAYER_H
#define VIRTUALPLAYER_H

#include <QVector>
#include <QSharedPointer>

class virtualPlayer
{
public:
    static QSharedPointer<virtualPlayer> singleInstance();

    virtualPlayer();

protected:
    static QSharedPointer<virtualPlayer> mInstance;
};

#endif // VIRTUALPLAYER_H
