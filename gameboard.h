#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QOpenGLWidget>
#include <QDebug>
#include <QTimer>
#include <QStack>

class gameBoard : public QOpenGLWidget
{
    Q_OBJECT

public:
    static gameBoard *singleInstance(QWidget *parent);
    static gameBoard *singleInstance();

    static void destroySingleton(){
                                    if(mInstance!=nullptr){
                                        delete mInstance;
                                    }
                                    mInstance=nullptr;
                                  };

protected:

    QTimer *mTimer;

    // Board status
    int mBallPosX;
    int mBallPosY;
    int mBallDirectionH;
    int mBallDirectionV;
    int mBallAngle;
    int mLRacketPos;
    int mLRacketDirection;
    int mRRacketPos;
    int mRRacketDirection;
    int mLScore;
    int mRScore;
    int mPointBlinkCounter;
    int mLastWinner;

    // Singleton instance, No smart pointer here
    // 'cause the window wodget will destroy it
    static gameBoard *mInstance;

    // Events 
    void paintEvent(QPaintEvent *event) override;

    // Constructors - Destructors  TODO: Do I need a virtual destructor?
    gameBoard(QWidget *parent);
    ~gameBoard() { qDebug() << "game board destroyed!"; };

    // Internal draw methods
    void drawFixedElements(QPainter &painter);
    void clearPrevElements(QPainter &painter);
    void drawNewElements(QPainter &painter);

    // Game movement methods
    void processNextRacketsPositions(int &racketPos, int &racketDir);
    int processNextBallXPostion();
    void processNextBallYPostion();
    bool shallBallReturn();

protected slots:

    // Time for updating screen events
    void updateScreenTimerSlot();

public:

    void setLastKeyPressed(int keyCode);

};

#endif // GAMEBOARD_H
