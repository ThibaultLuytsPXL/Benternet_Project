#ifndef WARHAMMERLORE_H
#define WARHAMMERLORE_H
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "nzmqt/nzmqt.hpp"
#include <nzmqt/impl.hpp>
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QObject>


class WarhammerLore: public QObject
{
public:
    WarhammerLore(QCoreApplication *a);
    void ReciveMessage(const QList<QByteArray>& messages);
private:
    nzmqt::ZMQSocket *pusher;
    nzmqt::ZMQSocket *subscriber;
};

#endif // WARHAMMERLORE_H
