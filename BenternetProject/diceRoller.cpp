#include "diceRoller.h"




DiceRoller::DiceRoller(QCoreApplication *a)
{
    std::cout << "Prep!" << std::endl;
    try
    {
    nzmqt::ZMQContext *context = nzmqt::createDefaultContext( a );
    pusher = context->createSocket( nzmqt::ZMQSocket::TYP_PUSH, context );
    subscriber = context->createSocket( nzmqt::ZMQSocket::TYP_SUB, context );
    //QObject::connect( subscriber, &nzmqt::ZMQSocket::messageReceived, printMessages );
    QObject::connect( subscriber, &nzmqt::ZMQSocket::messageReceived, this, &DiceRoller::ReciveMessage);
    pusher->connectTo( "tcp://benternet.pxl-ea-ict.be:24041" );
    subscriber->connectTo( "tcp://benternet.pxl-ea-ict.be:24042" );
    if (!subscriber->isConnected() || !pusher->isConnected())
        std::cout << "not connected\n" << std::endl;

    subscriber->subscribeTo( "DiceRoller" );





    context->start();
    }
    catch( nzmqt::ZMQException & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }
    std::cout << "Start!" << std::endl;


}

void DiceRoller::ReciveMessage(const QList<QByteArray>& messages)
{
    for(QByteArray msgByteArray : messages) {
        QString msg = QString::fromUtf8(msgByteArray);

        if(msg == "DiceRoller>"){
            DiceRoll();
        }else{
            std::cout << "Wrong topic" << std::endl;
            QString error = "Wrong topic";
            nzmqt::ZMQMessage message = nzmqt::ZMQMessage( error.toUtf8() );
            pusher->sendMessage( message );
        }
    }
}

void DiceRoller::DiceRoll()
{
    srand(time(0));
    int dice = rand() %6 +1 ;
    QString Dice = QString::number(dice);
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( Dice.toUtf8() );
    pusher->sendMessage( message );
}
