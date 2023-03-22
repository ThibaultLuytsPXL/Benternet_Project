#ifndef DICEROLLER_H
#define DICEROLLER_H
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

class DiceRoller: public QObject
{
public:
    DiceRoller(QCoreApplication *a);
    void ReciveMessage(const QList<QByteArray>& messages);
    void DiceRoll();
private:
    nzmqt::ZMQSocket *pusher;
    nzmqt::ZMQSocket *subscriber;
};

#endif // DICEROLLER_H
