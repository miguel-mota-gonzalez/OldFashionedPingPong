#include "gameboard.h"
#include "virtualplayer.h"

#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>
#include <QDebug>

const int MAGIC_BOARD_SIZE_X = 640;
const int MAGIC_BOARD_SIZE_Y = 480;
const int LINE_HEIGHT        = 10;
const int HORIZONTAL_MARGIN  = 15;
const int RACKET_WIDTH       = 10;
const int RACKET_HEIGHT      = 100;

const int LEFT_RACKET_UP     = 87;
const int LEFT_RACKET_DOWN   = 83;
const int RIGHT_RACKET_UP    = 79;
const int RiGT_RACKET_DOWN   = 75;

const int BALL_SIZE          = 15;
const int MIN_BALL_POS_Y     = 10;
const int MIN_BALL_POS_X     = HORIZONTAL_MARGIN + RACKET_WIDTH;
const int MAX_BALL_POS_Y     = MAGIC_BOARD_SIZE_Y - LINE_HEIGHT - BALL_SIZE;
const int MAX_BALL_POS_X     = MAGIC_BOARD_SIZE_X - HORIZONTAL_MARGIN  - RACKET_WIDTH - BALL_SIZE;

const int BALL_DIR_RIGHT     = 0;
const int BALL_DIR_LEFT      = 1;
const int BALL_DIR_UP        = 0;
const int BALL_DIR_DOWN      = 1;
const int BALL_STEP_SIZE     = 10;

const int RACKET_DIR_NONE    = 0;
const int RACKET_DIR_UP      = 1;
const int RACKET_DIR_DOWN    = 2;
const int RACKET_STEP_SIZE   = 10;

const int GAME_SPEED         = 40;

gameBoard *gameBoard::mInstance = nullptr;

gameBoard *gameBoard::singleInstance(QWidget *parent)
{
    if(mInstance == nullptr){
        mInstance = new gameBoard(parent);
    }
    return mInstance;
}

gameBoard *gameBoard::singleInstance()
{
    return mInstance;
}

gameBoard::gameBoard(QWidget *parent) : QOpenGLWidget(parent),
    mBallPosX(MIN_BALL_POS_X),
    mBallPosY(MAGIC_BOARD_SIZE_Y/2),
    mBallDirectionH(BALL_DIR_RIGHT),
    mBallDirectionV(BALL_DIR_DOWN),
    mBallAngle(45),
    mLRacketPos(MAGIC_BOARD_SIZE_Y/2),
    mLRacketDirection(RACKET_DIR_NONE),
    mRRacketPos(MAGIC_BOARD_SIZE_Y/2),
    mRRacketDirection(RACKET_DIR_NONE)

{
    qDebug() << "game board created!";
    setFixedSize(MAGIC_BOARD_SIZE_X , MAGIC_BOARD_SIZE_Y);

    mTimer = new QTimer(this);

    // setup signal and slot
    connect(mTimer, SIGNAL(timeout()), this, SLOT(updateScreenTimerSlot()));

    // msec
    mTimer->start(GAME_SPEED);
}

void gameBoard::updateScreenTimerSlot()
{
    update();
}

void gameBoard::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);

    painter.setBackground(QBrush(Qt::black));

    // Clear Brush
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::black));

    clearPrevElements(painter);

    // Paint Brush
    painter.setPen(QPen(Qt::green));
    painter.setBrush(QBrush(Qt::green));

    drawFixedElements(painter);

    processNextRacketsPositions(mLRacketPos, mLRacketDirection);
    processNextRacketsPositions(mRRacketPos, mRRacketDirection);
    processNextBallXPostion();
    processNextBallYPostion();

    drawNewElements(painter);

    painter.end();
}

void gameBoard::processNextBallYPostion()
{
    switch(mBallDirectionV){
    case BALL_DIR_DOWN:

        if(mBallPosY + BALL_STEP_SIZE >= MAX_BALL_POS_Y){
            mBallPosY = MAX_BALL_POS_Y;
            mBallDirectionV = BALL_DIR_UP;
        }
        else{
            mBallPosY+=BALL_STEP_SIZE;
        }
        break;
    case BALL_DIR_UP:

        if(mBallPosY - BALL_STEP_SIZE <= MIN_BALL_POS_Y){
            mBallPosY = MIN_BALL_POS_Y;
            mBallDirectionV = BALL_DIR_DOWN;
        }
        else{
            mBallPosY-=BALL_STEP_SIZE;
        }
        break;
    }
}

void gameBoard::processNextBallXPostion()
{
    switch(mBallDirectionH){
    case BALL_DIR_RIGHT:

        if(mBallPosX+BALL_STEP_SIZE>=MAX_BALL_POS_X){
            mBallPosX=MAX_BALL_POS_X;
            mBallDirectionH=BALL_DIR_LEFT;
        }
        else{
            mBallPosX+=BALL_STEP_SIZE;
        }

        break;
    case BALL_DIR_LEFT:

        if(mBallPosX-BALL_STEP_SIZE <= MIN_BALL_POS_X){
            mBallPosX=MIN_BALL_POS_X;
            mBallDirectionH=BALL_DIR_RIGHT;
        }
        else{
            mBallPosX-=BALL_STEP_SIZE;
        }

        break;
    }
}

void gameBoard::processNextRacketsPositions(int &racketPos, int &racketDir)
{
    // Left Racket
    if(racketPos!=RACKET_DIR_NONE){
        switch(racketDir){
        case RACKET_DIR_UP:

            if((racketPos-RACKET_STEP_SIZE) - (RACKET_HEIGHT/2) <= MIN_BALL_POS_Y){
                racketPos=MIN_BALL_POS_Y+(RACKET_HEIGHT/2);
                racketDir=RACKET_DIR_DOWN;
            }
            else{
                racketPos-=RACKET_STEP_SIZE;
            }

            break;
        case RACKET_DIR_DOWN:

            if((racketPos+RACKET_STEP_SIZE) + (RACKET_HEIGHT/2) >= MAX_BALL_POS_Y+BALL_SIZE){
                racketPos=(MAX_BALL_POS_Y+BALL_SIZE)-(RACKET_HEIGHT/2);
                racketDir=RACKET_DIR_UP;
            }
            else
            {
                racketPos+=RACKET_STEP_SIZE;
            }

            break;
        }
    }
}

void gameBoard::setLastKeyPressed(int keyCode)
{
    switch(keyCode){
        case LEFT_RACKET_UP:
        mLRacketDirection = RACKET_DIR_UP;
        break;
        case LEFT_RACKET_DOWN:
        mLRacketDirection = RACKET_DIR_DOWN;
        break;
        case RIGHT_RACKET_UP:
        mRRacketDirection = RACKET_DIR_UP;
        break;
        case RiGT_RACKET_DOWN:
        mRRacketDirection = RACKET_DIR_DOWN;
        break;
    }
}

void gameBoard::drawFixedElements(QPainter &painter)
{
    // Bard limits
    painter.drawRect(0, 0, MAGIC_BOARD_SIZE_X, LINE_HEIGHT);
    painter.drawRect(0, MAGIC_BOARD_SIZE_Y-LINE_HEIGHT, MAGIC_BOARD_SIZE_X, LINE_HEIGHT);
}

void gameBoard::clearPrevElements(QPainter &painter)
{
    // Racket s
    painter.drawRect(HORIZONTAL_MARGIN,
                     mLRacketPos-(RACKET_HEIGHT/2),
                     RACKET_WIDTH,
                     RACKET_HEIGHT);
    painter.drawRect(MAGIC_BOARD_SIZE_X - (RACKET_WIDTH + HORIZONTAL_MARGIN),
                     mRRacketPos-(RACKET_HEIGHT/2),
                     RACKET_WIDTH,
                     RACKET_HEIGHT);

    // Ball
    painter.drawEllipse(QRectF(mBallPosX, mBallPosY, BALL_SIZE, BALL_SIZE));
}

void gameBoard::drawNewElements(QPainter &painter)
{
    // Racket s
    painter.drawRect(HORIZONTAL_MARGIN,
                     mLRacketPos-(RACKET_HEIGHT/2),
                     RACKET_WIDTH,
                     RACKET_HEIGHT);
    painter.drawRect(MAGIC_BOARD_SIZE_X - (RACKET_WIDTH + HORIZONTAL_MARGIN),
                     mRRacketPos-(RACKET_HEIGHT/2),
                     RACKET_WIDTH,
                     RACKET_HEIGHT);

    // Ball
    painter.drawEllipse(QRectF(mBallPosX, mBallPosY, BALL_SIZE, BALL_SIZE));
}



