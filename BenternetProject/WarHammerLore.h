#ifndef WARHAMMERLORE_H
#define WARHAMMERLORE_H
#include <iostream>
#include <ctime>
#include <cstdlib>
//#include "nzmqt/nzmqt.hpp"
#include <nzmqt/impl.hpp>
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QObject>


class WarHammerLore: public QObject
{
public:
    WarHammerLore(QCoreApplication *a);
    void ReciveMessage(const QList<QByteArray>& messages);
    void SendMessage(QString msg);
    void DiceRoll(int, QList<QString>);
    void LoreList();
    void LoreHelp(QList<QString>);
    void Help(QList<QString>);
    void AddListItem(QList<QString>);
    void Link(QList<QString>);
    void Overview(QList<QString>);
private:
    nzmqt::ZMQSocket *pusher;
    nzmqt::ZMQSocket *subscriber;
    QMap<QString, QString> loreListLoyal;
    QMap<QString, QString> loreListTraitor;
    QMap<QString, QString> loreListAlpha;
    QMap<QString, QString> linkinglist;

};

#endif // WARHAMMERLORE_H
