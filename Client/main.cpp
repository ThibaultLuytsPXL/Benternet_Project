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

        QTextStream t(stdin);
        std::cout <<  "send h for a help page"  << std::endl;
        std::cout <<  "Pleas give your name or another ID"  << std::endl;
        QString name = t.readLine();

        QThread *thread = QThread::create([pusher, name]{
            while( 1 )
            {
                QTextStream s(stdin);
                //std::cout <<  "Pleas give your name or another ID"  << std::endl;
                //QString name = s.readLine();
                //subscriber->subscribeTo( QString("LoreHammer!>%1").arg(name) );
                QString input = s.readLine();
                if (input.size()==0)
                    {
                        std::cout << "No command found" << std::endl;
                    }
                else if (input.at(0)=='h')
                    {
                    input = QString("LoreHammer?>%1>help").arg(name);
                    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                    pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else if (input.at(0)=='d')
                    {
                        QList<QString> msgSplit = input.split(" ");
                        input = QString("LoreHammer?>%1>dice>%2").arg(name).arg(msgSplit[1]);
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else if (input.contains("add"))
                    {
                        QList<QString> msgSplit = input.split(" ");
                        QList<QString> msgSplit2 = input.split(">");
                        input = QString("LoreHammer?>%1>add %2>%3>%4").arg(name).arg(msgSplit[1]).arg(msgSplit2[0]).arg(msgSplit2[1]);
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else if (input.contains("link"))
                    {
                        //QList<QString> msgSplit = input.split(" ");
                        QList<QString> msgSplit = input.split(">");
                        input = QString("LoreHammer?>%1>link>%2>%3").arg(name).arg(msgSplit[1]).arg(msgSplit[2]);
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else if (input.contains("overview"))
                    {
                        //QList<QString> msgSplit = input.split(" ");
                        //QList<QString> msgSplit2 = input.split(">");
                        input = QString("LoreHammer?>%1>overview").arg(name);
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else if (input.compare("lore") == 0)
                    {
                        input = QString("LoreHammer?>%1>lore").arg(name);
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else if (input.toLower().compare("loyallist") == 0)
                    {
                        input = QString("LoreHammer?>%1>list L").arg(name);
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else if (input.toLower().compare("traitorlist") == 0)
                    {
                        input = QString("LoreHammer?>%1>list T").arg(name);
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else if (input.toLower().compare("alphalist") == 0)
                    {
                        input = QString("LoreHammer?>%1>list @").arg(name);
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else if (input.toLower().contains("loyal"))
                    {
                        QList<QString> msgSplit = input.split(">");
                        input = QString("LoreHammer?>%1>l>%2").arg(name).arg(msgSplit[1]);
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else if (input.toLower().contains("traitor"))
                    {
                        QList<QString> msgSplit = input.split(">");
                        input = QString("LoreHammer?>%1>t>%2").arg(name).arg(msgSplit[1]);
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else if (input.toLower().contains("alpha"))
                    {
                        QList<QString> msgSplit = input.split(">");
                        input = QString("LoreHammer?>%1>@>%2").arg(name).arg(msgSplit[1]);
                        nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                        pusher->sendMessage( message );
                        //std::cout << "Message send !" << std::endl;
                    }
                else
                    {
                        std::cout <<  "wrong input! try again"  << std::endl;
                    }

                //nzmqt::ZMQMessage message = nzmqt::ZMQMessage( input.toUtf8() );
                //pusher->sendMessage( message );
                //std::cout << "Message send !" << std::endl;
            }
        });


//		pusher->connectTo( "tcp://localhost:24041" );
//		subscriber->connectTo( "tcp://localhost:24042" );
//		pusher->connectTo( "tcp://84.193.238.142:24041" );
//		subscriber->connectTo( "tcp://84.193.238.142:24042" );
		pusher->connectTo( "tcp://benternet.pxl-ea-ict.be:24041" );
		subscriber->connectTo( "tcp://benternet.pxl-ea-ict.be:24042" );
//		pusher->connectTo( "tcp://benternet.backup.pxl-ea-ict.be:24041" );
//		subscriber->connectTo( "tcp://benternet.backup.pxl-ea-ict.be:24042" );
//		pusher->connectTo( "tcp://np.pxl-ea-ict.be:24041" );
//		subscriber->connectTo( "tcp://np.pxl-ea-ict.be:24042" );
//		pusher->connectTo( "tcp://192.168.1.8:24041" );
//		subscriber->connectTo( "tcp://192.168.1.8:24042" );


		if( argc > 1 )
		{
			for( int i = 1 ; i < argc ; i++ )
			{
				subscriber->subscribeTo( argv[i] );
			}
		}
		else
		{
            subscriber->subscribeTo( QString("LoreHammer!>%1").arg(name) );
            //subscriber->subscribeTo( "LoreHammer!>" );
		}

		if( !pusher->isConnected() || !subscriber->isConnected() )
		{
			std::cerr << "NOT CONNECTED !!!" << std::endl;
		}

		context->start();
		thread->start();

	}
	catch( nzmqt::ZMQException & ex )
	{
		std::cerr << "Caught an exception : " << ex.what();
	}
	std::cout << "Start!" << std::endl;
	return a.exec();
}
