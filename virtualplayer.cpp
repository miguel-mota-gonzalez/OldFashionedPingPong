#include "virtualplayer.h"

#include <QVector>

QSharedPointer<virtualPlayer> virtualPlayer::mInstance = nullptr;

QSharedPointer<virtualPlayer> virtualPlayer::singleInstance()
{
    if(mInstance==nullptr){
        mInstance = QSharedPointer<virtualPlayer>(new virtualPlayer());
    }

    return mInstance;
}

virtualPlayer::virtualPlayer()
{

}
