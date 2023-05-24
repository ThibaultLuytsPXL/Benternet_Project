#include "WarHammerLore.h"




WarHammerLore::WarHammerLore(QCoreApplication *a)
{
    std::cout << "Prep!" << std::endl;
    LoreList();
    try
    {
    nzmqt::ZMQContext *context = nzmqt::createDefaultContext( a );
    pusher = context->createSocket( nzmqt::ZMQSocket::TYP_PUSH, context );
    subscriber = context->createSocket( nzmqt::ZMQSocket::TYP_SUB, context );
    //QObject::connect( subscriber, &nzmqt::ZMQSocket::messageReceived, printMessages );
    QObject::connect( subscriber, &nzmqt::ZMQSocket::messageReceived, this, &WarHammerLore::ReciveMessage);
    pusher->connectTo( "tcp://benternet.pxl-ea-ict.be:24041" );
    subscriber->connectTo( "tcp://benternet.pxl-ea-ict.be:24042" );
    if (!subscriber->isConnected() || !pusher->isConnected())
        std::cout << "not connected\n" << std::endl;
    subscriber->subscribeTo( "LoreHammer?" );





    context->start();
    }
    catch( nzmqt::ZMQException & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }
    std::cout << "Start!" << std::endl;


}

void WarHammerLore::ReciveMessage(const QList<QByteArray>& messages)
{

    for(QByteArray msgByteArray : messages) {
        QString msg = QString::fromUtf8(msgByteArray);
        QList<QString> msgSplit = msg.split(">");

        if(msgSplit[2].toLower().compare("help") == 0)
        {
            Help(msgSplit);
        }
        else if(msgSplit[2].toLower().compare( "dice") == 0)
        {
            int number = msgSplit[3].toInt();
            DiceRoll(number, msgSplit);
        }
        else if(msgSplit[2].toLower().compare( "lore") == 0)
        {
            LoreHelp(msgSplit);
        }
        else if(msgSplit[2].toLower().contains("list"))
        {
            if(msgSplit[2].toLower().contains("loyal")|| msgSplit[2].contains("L"))
            {
                QString lore = QString("LoreHammer!>%1>L>\n%2").arg(msgSplit[1]).arg(loreListLoyal.keys().join('\n'));
                SendMessage(lore);
            }
            else if(msgSplit[2].toLower().contains("traitor")|| msgSplit[2].contains("T"))
            {
                QString lore = QString("LoreHammer!>%1>T>\n%2").arg(msgSplit[1]).arg(loreListTraitor.keys().join('\n'));
                SendMessage(lore);
            }
            else if(msgSplit[2].toLower().contains("alpha")|| msgSplit[2].contains("@"))
            {
                QString lore = QString("LoreHammer!>%1>@>\n%2").arg(msgSplit[1]).arg(loreListAlpha.keys().join('\n'));
                SendMessage(lore);
            }
        }
        else if(msgSplit[2].toLower().compare("l") == 0)

        {
                if( loreListLoyal.contains(msgSplit[3]))
                {
                    QString lore = QString("LoreHammer!>%1>L>%2>%3").arg(msgSplit[1]).arg(msgSplit[3]).arg(loreListLoyal.value(msgSplit[3]));
                    SendMessage(lore);
                }

                else

                {
                    QString error = QString("LoreHammer!>%1>L>Vox Communications error").arg(msgSplit[1]);
                    SendMessage(error);
                }

        }else if(msgSplit[2].toLower().compare("t") == 0)
        {
            if( loreListTraitor.contains(msgSplit[3]))
            {
                QString lore = QString("LoreHammer!>%1>T>%2>%3").arg(msgSplit[1]).arg(msgSplit[3]).arg(loreListTraitor.value(msgSplit[3]));
                SendMessage(lore);
            }

            else

            {
                QString error =QString("LoreHammer!>%1>T>Vox Communications error").arg(msgSplit[1]);
                SendMessage(error);
            }

        }else if(msgSplit[2].compare("@") == 0){
            if( loreListAlpha.contains(msgSplit[3]))
            {
                QString lore = QString("LoreHammer!>%1>@>%2>%3").arg(msgSplit[1]).arg(msgSplit[3]).arg(loreListAlpha.value(msgSplit[3]));
                SendMessage(lore);
            }

            else

            {
                QString error = QString("LoreHammer!>%1>@>Vox Communications error").arg(msgSplit[1]);
                SendMessage(error);
            }
        }

        else
        {
            //std::cout << "Wrong topic" << std::endl;
            QString error = QString("LoreHammer!>%1>Wrong topic").arg(msgSplit[1]);
            SendMessage(error);
        }
    }
}

void WarHammerLore::SendMessage(QString msg)
{
    nzmqt::ZMQMessage message = nzmqt::ZMQMessage( msg.toUtf8() );
    pusher->sendMessage( message );
}

void WarHammerLore::DiceRoll(int number, QList<QString> id)
{
    srand(time(0));
    int dice = rand() %number +1 ;
    QString diceMsg = QString("LoreHammer!>%1>Dice>\n%2").arg(id[1]).arg(dice);
    SendMessage(diceMsg);
}

void WarHammerLore::Help(QList<QString> id)
{
    QString help = QString("LoreHammer!>%1>Help\n\nFor Rolling Dice, send:\nLoreHammer?>%1>Dice># for rolling a dice between 0 and #\n\nFor a list of the main Lore categories about Warhammer, send:\nLoreHammer?>%1>Lore").arg(id[1]);
    SendMessage(help);
}

void WarHammerLore::LoreHelp(QList<QString> id)
{
    QString loreHelp = QString("LoreHammer!>%1>Lore\n\nThe categories are Loyal, Traitor and Alpha Legion\n\nTo see each individual list, send:\n\nLoreHammer?>%1>(Loyal/L)or(Traitor/T)or(Alpha Legion/@) list").arg(id[1]);
    SendMessage(loreHelp);
}

void WarHammerLore::LoreList()
{
    //Loyal loreList
    loreListLoyal.insert("The God Emperor of Mankind","");
    loreListLoyal.insert("Astra Militarum/Imperial Guard","");
    loreListLoyal.insert("Imperial Guard Regiments","");
    loreListLoyal.insert("Imperial Guard Notable Characters","");
    loreListLoyal.insert("Astartes/SpaceMarines","");
    loreListLoyal.insert("SpaceMarines Legions","");
    loreListLoyal.insert("SpaceMarines Primarchs","");
    loreListLoyal.insert("SpaceMarines Notable Characters","");
    loreListLoyal.insert("Adeptus mechanicus","");
    loreListLoyal.insert("Imperial Knights","");
    loreListLoyal.insert("Collegia titanica","");
    loreListLoyal.insert("Adeptus custodes","");
    loreListLoyal.insert("Adeptus sororitas","");
    loreListLoyal.insert("The Inquisition","");
    loreListLoyal.insert("Assasins","");
    loreListLoyal.insert("Suprise me","");
    //Traitor loreList
    loreListTraitor.insert("Chaos Gods","\nThere are four great Powers of Chaos. Each one is given form and fed by the emotions manifested by sentient beings in the material universe \n\nKhorne, the Blood God, is the Chaos God of anger, violence, and hate. Khorne was the first to awake of the four Gods of Chaos, fully coming into existence during Terra's Middle Ages - an era of wars blooming in the wake of his birth. Every act of violence gives Khorne power, whether committed by his followers or by their enemies. His titles include the Lord of Rage, the Taker of Skulls, the Lord of Battle and the Master of the Brazen Throne.\n\nTzeentch is a God of Chaos who represents the vitality and volatility of change. Tzeentch is closely associated with sorcery and magic, as well as dynamic mutation, and grand, convoluted scheming. The domains of history, destiny, intrigue and plots are his chief interests, and in pursuit of these aspects he listens to the dreams and hopes of all and watches their plans take form. He is not content to merely observe, however, and chooses to interfere in the skeins of fate in order to fulfill his own, unknowably complex schemes. Tzeentch is known by an endless multitude of names, but the chief titles he bears are the Changer of the Ways, the Master of Fortune, the Great Conspirator and the Architect of Fate.\n\nNurgle is the Chaos God of despair, decay and disease. He was the third to awake of the four Gods of Chaos, fully coming into existence during Terra's Middle Ages, with plagues sweeping across continents in the wake of his birth. His titles include the Plague Father, Fly Lord, Great Corruptor, Plague Lord, Master of Pestilence, Lord of Decay.\n\nSlaanesh (eldar - Sai'lanthresh) is the Chaos God of lust, greed, excess, pain, pleasure, perfection and hedonism. Slaanesh was the last of the major Chaos Gods to be born. Coming into existence with the collapse of the Eldar civilisation. ");
    loreListTraitor.insert("Chaos Demons","");
    loreListTraitor.insert("Traitor Guard","");
    loreListTraitor.insert("Traitor Guard Regiments","");
    loreListTraitor.insert("Traitor Guard Notable Characters","");
    loreListTraitor.insert("Chaos SpaceMarine","");
    loreListTraitor.insert("Chaos SpaceMarine Legions","");
    loreListTraitor.insert("Chaos SpaceMarine Primarchs","");
    loreListTraitor.insert("Chaos SpaceMarine Notable Characters","");
    loreListTraitor.insert("Dark Mechanicum","");
    loreListTraitor.insert("Suprise me","");
    //Alpha loreList
    loreListAlpha.insert("Primarchs","Alpharius and his twin Omegon (sometimes referred to collectively as just 'Alpharius Omegon')");
    loreListAlpha.insert("Notable Characters","Alpharius - Primarch,Omegon - Leader of the Effrit Stealth Squad / Primarch, Ingo Pech - First Captain, Mathias Herzog - Captain of the 2nd Company, Armillus Dynat - Harrowmaster, Exodus - Assassin, Phocron - Headhunter Prime, Siridor Vhen - Praetor, Sheed Ranko - Captain of the Lernaen Terminator Squad Killed during the Tenebrae Mission, Autilon Skorr - 'The Hydra's Headman' - Consul Delegatus of the Alpha Legion");
    loreListAlpha.insert("Suprise you!","");
}


