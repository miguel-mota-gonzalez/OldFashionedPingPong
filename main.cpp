
#include "window.h"

#include <QApplication>
#include <QDebug>

class ticTacToeApp: public QApplication {
public:
   ticTacToeApp(int argc, char *argv[]) : QApplication(argc, argv){
   }

    ~ticTacToeApp() {
       window::destroySingleton();

       qDebug() << "App finished!!!";
    }
};

int main(int argc, char *argv[])
{
    ticTacToeApp app(argc, argv);

    qDebug() << "App started!";

    window::singleInstance()->show();

    return app.exec();
}
