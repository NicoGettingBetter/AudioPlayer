#include "player.hpp"
#include <QApplication>
#include <QIcon>
#include <QStyleFactory>


int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("fusion"));
    QApplication a(argc, argv);
    a.setApplicationName("NicoPlayer");
    a.setWindowIcon(QIcon(":/Icon.png"));

    Player player;
    player.show();

    return a.exec();
}
