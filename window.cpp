
#include "window.h"
#include "gameboard.h"

#include <QGridLayout>
#include <QSharedPointer>
#include <QDebug>

window* window::mInstance = nullptr;

window* window::singleInstance()
{
    if(mInstance==nullptr){
        mInstance = new window();
    }
    return mInstance;
}

window::window()
{
    qDebug() << "window created!";

    setWindowTitle(tr("My Ping Pong in QT!"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(gameBoard::singleInstance(this), 0, 0);
    setLayout(layout);
}

void window::closeEvent(QCloseEvent *)
{
    qDebug("The close button was clicked");
    gameBoard::destroySingleton();
}

void window::keyPressEvent(QKeyEvent *event)
{
     qDebug() << "Key pressed : " << event->key();

     if(gameBoard::singleInstance()!=nullptr)
        gameBoard::singleInstance()->setLastKeyPressed(event->key());
}

