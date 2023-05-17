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


class WarHammerLore: public QObject
{
public:
    WarHammerLore(QCoreApplication *b);
    void ReciveMessage(const QList<QByteArray>& messages);
    void DiceRoll(int, QList<QString>);
    void LoreList();
    void Help(QList<QString>);
    QMap<QString, QString> loreListLoyal;
    QMap<QString, QString> loreListTraitor;
    QMap<QString, QString> loreListAlpha;
private:
    nzmqt::ZMQSocket *pusher;
    nzmqt::ZMQSocket *subscriber;

};

#endif // WARHAMMERLORE_H
