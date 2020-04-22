#include <QApplication>
#include "board.h"

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    Board board;
    board.setFixedSize(550,550);
    board.show();
    return app.exec();

}



