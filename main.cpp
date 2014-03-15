#include "snake_game.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    snake_game w;
    w.run_in_loop();
    w.show();
    
    return a.exec();
}
