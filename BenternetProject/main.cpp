#include <iostream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <nzmqt/nzmqt.hpp>
#include <nzmqt/impl.hpp>
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QDateTime>
#include <WarHammerLore.h>
int main( int argc, char *argv[] )
{
    QCoreApplication a(argc, argv);

    WarHammerLore * LoreHammer = new WarHammerLore(&a);

    return a.exec();
}
