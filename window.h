#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QCloseEvent>
#include <QDebug>

class window : public QWidget
{
    Q_OBJECT

public:
    static window* singleInstance();
    static void destroySingleton() {
                                    if(mInstance!=nullptr)
                                        delete mInstance;
                                    mInstance = nullptr;
                                   };
    ~window() { qDebug() << "window destroyed!"; };

protected:
    static window *mInstance;

    window();
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif
