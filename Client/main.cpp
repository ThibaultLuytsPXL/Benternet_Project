#include <iostream>
#include <nzmqt/nzmqt.hpp>
#include <nzmqt/impl.hpp>
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QDateTime>


int main( int argc, char *argv[] )
{
	QCoreApplication a(argc, argv);
	std::cout << "Prep!" << std::endl;
	try
	{
		nzmqt::ZMQContext *context = nzmqt::createDefaultContext( &a );
		nzmqt::ZMQSocket *pusher = context->createSocket( nzmqt::ZMQSocket::TYP_PUSH, context );
		nzmqt::ZMQSocket *subscriber = context->createSocket( nzmqt::ZMQSocket::TYP_SUB, context );
		//QObject::connect( subscriber, &nzmqt::ZMQSocket::messageReceived, printMessages );
		QObject::connect( subscriber, &nzmqt::ZMQSocket::messageReceived, []( const QList<QByteArray>& messages )
		{
			if( messages.size() < 1 )
			{
				std::cout << "Received empty message !" << std::endl;
			}
			else if( messages.size() == 1 )
			{
				std::cout << "Received (" << messages.constFirst().size() << ") : " << messages.constFirst().toStdString() << std::endl;
			}
			else
			{
				std::cout << "Received " << messages.size() << " parts" << std::endl;
				int part = 1;
				for( const QByteArray & message : messages )
				{
					std::cout << "Part " << part << " (" << message.size() << ") : " << message.toStdString() << std::endl;
					part++;
				}
			}
		} );


        QThread *thread = QThread::create([pusher]{
			QTextStream s(stdin);
            while( 1 )
			{
				QString input = s.readLine();
				nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
				pusher->sendMessage( message );
				std::cout << "Message send !" << std::endl;
            }
        });

        pusher->connectTo( "tcp://benternet.pxl-ea-ict.be:24041" );
		subscriber->connectTo( "tcp://benternet.pxl-ea-ict.be:24042" );

		if( argc > 1 )
		{
			for( int i = 1 ; i < argc ; i++ )
			{
				subscriber->subscribeTo( argv[i] );
			}
		}
		else
		{
            subscriber->subscribeTo( "LoreHammer!>Thibault>" );
            subscriber->subscribeTo( "LoreHammer!>" );
		}

		if( !pusher->isConnected() || !subscriber->isConnected() )
		{
			std::cerr << "NOT CONNECTED !!!" << std::endl;
		}

		context->start();
		//pinger->start();
		thread->start();

		/*nzmqt::ZMQMessage * incoming = new nzmqt::ZMQMessage();
		while( subscriber->isConnected() )
		{
			subscriber->receiveMessage( incoming, 0 );
			std::cout << incoming->toByteArray().toStdString() << std::endl;
		}*/
	}
	catch( nzmqt::ZMQException & ex )
	{
		std::cerr << "Caught an exception : " << ex.what();
	}
	std::cout << "Start!" << std::endl;
	return a.exec();
}
