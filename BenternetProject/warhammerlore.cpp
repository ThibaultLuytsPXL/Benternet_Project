#include "WarHammerLore.h"




WarHammerLore::WarHammerLore(QCoreApplication *a)
{
    std::cout << "Prep!" << std::endl;
    LoreList();//Create the Lore map
    srand(time(0));//moved srand here for only running it once
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
        std::cout << msg.toStdString() << std::endl;
        if(msgSplit[2].toLower().compare("help") == 0)
        {
            Help(msgSplit);
        }
        else if(msgSplit[2].toLower().compare("dice") == 0)
        {
            int number = msgSplit[3].toInt();
            DiceRoll(number, msgSplit);
        }
        else if(msgSplit[2].toLower().compare("lore") == 0)
        {
            LoreHelp(msgSplit);
        }
        else if(msgSplit[2].toLower().contains("add"))
        {
            AddListItem(msgSplit);
        }
        else if(msgSplit[2].toLower().contains("link"))
        {
            Link(msgSplit);
        }
        else if(msgSplit[2].toLower().contains("overview"))
        {
            Overview(msgSplit);
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
        else if(msgSplit[2].contains("l"))

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

        }else if(msgSplit[2].contains("t"))
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

        }else if(msgSplit[2].contains("@")){
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
    //srand(time(0));
    int dice = rand() %number +1 ;
    QString diceMsg = QString("LoreHammer!>%1>Dice>\n%2").arg(id[1]).arg(dice);
    SendMessage(diceMsg);
}

void WarHammerLore::Help(QList<QString> id)
{
    QString help = QString("LoreHammer!>%1>Help\n\nFor Rolling Dice, send:\nDice or d # for rolling a dice between 0 and #\n\nFor a list of the main Lore categories about Warhammer, send:\nLore").arg(id[1]);
    SendMessage(help);
}



void WarHammerLore::LoreHelp(QList<QString> id)
{
    QString loreHelp = QString("LoreHammer!>%1>Lore\n\nThe categories are Loyal, Traitor and Alpha Legion\n\nTo see each individual list, send:\n\nloyallist, traitorlist, alphalist for the complete list of all topics\n\nFor getting information of a topic send (loyal/traitor/alpha)>topic form the list (the topics are case sensitive)").arg(id[1]);
    SendMessage(loreHelp);
}

void WarHammerLore::Link(QList<QString> msg)
{
    QString key = QString(msg[4]);
    QString value = QString(msg[1]);
    QString faction = QString(msg[3]);
    if(faction.compare("L") == 0)
    {
        if(loreListLoyal.contains(msg[4]))
        {
            linkinglist.insert(key, value);
        }else
            SendMessage("wrong topic");

    }
    else if(faction.compare("T") == 0)
    {
        if(loreListTraitor.contains(msg[4]))
        {
            linkinglist.insert(key, value);
        }else
            SendMessage("wrong topic");
    }
    else if(faction.compare("@") == 0)
    {
        if(loreListAlpha.contains(msg[4]))
        {
            linkinglist.insert(key, value);
        }else
            SendMessage("wrong topic");
    }
}

void WarHammerLore::Overview(QList<QString> msg)
{
    QString overview = QString("LoreHammer!>%1>\n").arg(msg[1]);
    for( QString key : linkinglist.keys() )
    {
        overview.append( key );
        overview.append("-----");
        overview.append( linkinglist.value(key) );
        overview.append('\n');
    }
     SendMessage(overview);
}
void WarHammerLore::AddListItem(QList<QString> msg)
{
    QString key = QString(msg[3]);
    QString value = QString(msg[4]);
    if(msg[2].contains("L"))
    {
         loreListLoyal.insert(key, value);
    }
    else if (msg[2].contains("T"))
    {
        loreListTraitor.insert(key, value);
    }
    else if (msg[2].contains("@"))
    {
        loreListAlpha.insert(key, value);
    }

}


void WarHammerLore::LoreList()
{
    //Loyal loreList
    loreListLoyal.insert("The God Emperor of Mankind","The Emperor of Mankind is the sovereign ruler of the Imperium of Man, and Father, Guardian, and God of the human race. He has sat immobile within the Golden Throne of Terra for ten thousand years. Although once a living man, His shattered body can no longer support life, and remains intact only by a combination of ancient technology and the sheer force of His will, itself sustained by the soul-sacrifice of countless millions of psykers.");
    loreListLoyal.insert("Astra Militarum/Imperial Guard","The Astra Militarum, commonly known as Imperial Guard, is the primary fighting force of the Imperium, so numerous in size that even the Departmento Munitorum cannot place a figure on the number of Guardsmen under arms at any one time; the lists of new recruits and toll of casualties can run into the millions in a single day. It would be unfeasible trying to put any exact number on the strength of the Guard; however, it is believed that there must be many billions of Guardsmen, divided into millions of regiments. This absolute numeracy provides the Guard with its main power; their ability to deploy in numbers that, eventually, result in victory. Attacking in seemingly endless influxes across battle-zones, charging forth under the cover of massive barrages and delivering massed lasgun volleys, in the Guard the individual Human soldier may appear a lost thing, almost forgotten. Yet the actions of these anonymous soldiers daily decide the fate of worlds. In times of crisis the Imperial Guard will call upon their deadliest of soldiers, whether they are the Imperium's famed Storm Troopers or the regimental elite Kasrkin and Grenadiers.");
    loreListLoyal.insert("Imperial Guard Regiments","Armageddon Steel Legion - Attilan Rough Riders - Cadian Shock Troopers - Catachan Jungle Fighters - Death Korps of Krieg - Elysian Drop Troops - Mordian Iron Guard - Tallarn Desert Raiders - Tanith First and Only - Valhallan Ice Warriors - Vostroyan Firstborn");
    loreListLoyal.insert("Imperial Guard Notable Characters","Commissar Sebastian Yarrick - Commissar Ciaphas Cain - Nork Deddog - Catachan Ogryn commander and legendary bodyguard - Sly Marbo - Catachan sniper known as the One Man Army - Ollanius Pius - Patron saint of the Astra Militarum");
    loreListLoyal.insert("Astartes/SpaceMarines","The Adeptus Astartes (commonly known as Space Marines, and colloquially as Angels of Death) are one of the most elite and feared fighting forces in the Imperium of Man. The primary unit of organisation is the Chapter, a self-contained army fully equipped with its own transport, non-combatant support staff, etc. There are around a thousand Chapters, each comprising a thousand Space Marines. There are far too few Space Marines to form the Imperium's main military forces; instead they operate as highly mobile strike forces. They are entrusted with the most dangerous missions, such as lightning raids behind enemy lines, infiltration, and tunnel fighting[5]. Through selection, training, and conditioning, Marines are physically, mentally, and spiritually superior to any other Imperial soldier.");
    loreListLoyal.insert("SpaceMarines Legions","Dark Angels - White Scars - Space Wolves - Imperial Fists - Blood Angels - Iron Hands - Ultramarines - Salamanders - Raven Guard - Alpha Legion(we think/hope)");
    loreListLoyal.insert("SpaceMarines Primarchs","Lion El'Jonson - Jaghatai Khan - Leman Russ - Rogal Dorn - Sanguinius - Ferrus Manus - Roboute Guilliman - Vulkan - Corvus Corax - Omegon(we think)");
    loreListLoyal.insert("Adeptus mechanicus","The Adeptus Mechanicus, also formerly known as the Mechanicum, is a technological organisation, often known as the Priesthood of Mars. It holds a monopoly on technological knowledge in the Imperium. Their Forge Worlds turn out the Imperium's most powerful and advanced weaponry and equipment. The organisation's adepts, the Tech-priests, are vital in maintaining much of the Imperium's more technologically advanced equipment, not the least of which is the Emperor's life-sustaining Golden Throne");
    loreListLoyal.insert("Collegia titanica","The Collegia Titanica is the section of the Adeptus Mechanicus that includes the Imperial Titans, the colossal god-machines. The Collegia is also known as the 'Adeptus Titanicus' (a contraction of 'Adeptus Mechanicus Collegia Titanica')");
    loreListLoyal.insert("Adeptus custodes","The Adeptus Custodes, renowned as the Brotherhood of Demigods, the Golden Legion and a host of other titles, (The Custodian Guard, The Guardians, the Emperor’s Saints, the Watchers of The Throne, The Thousand Companions, The Ten Thousand), but most commonly known as Custodians, are the guardians of the Imperial Palace and the Golden Throne, as well as being the personal bodyguard to the Emperor. Due to the vast size of the Imperial Palace, the Custodes always act as a defensive army. Together with the Sisters of Silence they represented the Talons of the Emperor being the right hand of the Emperor. For much of their history the Custodes rarely left the Imperial Palace and even more rarely left Terra. It is the Adeptus Custodes that decide who may enter the throne room of the Emperor, and when. Such is their authority in this matter that Space Marines and Inquisitors must kneel before them. Custodians are the mightiest of mankind's warriors. They are to the Space Marines what those transhuman warriors are to common Guardsmen, an elevated elite whose raw strength and willpower are wholly insurmountable.");
    loreListLoyal.insert("Suprise me","Legion of the Dammed");
    //Traitor loreList
    loreListTraitor.insert("Chaos Gods","\nThere are four great Powers of Chaos. Each one is given form and fed by the emotions manifested by sentient beings in the material universe \n\nKhorne, the Blood God, is the Chaos God of anger, violence, and hate. Khorne was the first to awake of the four Gods of Chaos, fully coming into existence during Terra's Middle Ages - an era of wars blooming in the wake of his birth. Every act of violence gives Khorne power, whether committed by his followers or by their enemies. His titles include the Lord of Rage, the Taker of Skulls, the Lord of Battle and the Master of the Brazen Throne.\n\nTzeentch is a God of Chaos who represents the vitality and volatility of change. Tzeentch is closely associated with sorcery and magic, as well as dynamic mutation, and grand, convoluted scheming. The domains of history, destiny, intrigue and plots are his chief interests, and in pursuit of these aspects he listens to the dreams and hopes of all and watches their plans take form. He is not content to merely observe, however, and chooses to interfere in the skeins of fate in order to fulfill his own, unknowably complex schemes. Tzeentch is known by an endless multitude of names, but the chief titles he bears are the Changer of the Ways, the Master of Fortune, the Great Conspirator and the Architect of Fate.\n\nNurgle is the Chaos God of despair, decay and disease. He was the third to awake of the four Gods of Chaos, fully coming into existence during Terra's Middle Ages, with plagues sweeping across continents in the wake of his birth. His titles include the Plague Father, Fly Lord, Great Corruptor, Plague Lord, Master of Pestilence, Lord of Decay.\n\nSlaanesh (eldar - Sai'lanthresh) is the Chaos God of lust, greed, excess, pain, pleasure, perfection and hedonism. Slaanesh was the last of the major Chaos Gods to be born. Coming into existence with the collapse of the Eldar civilisation. ");
    loreListTraitor.insert("Chaos Demons","Daemons are entities of the Warp, horrific creatures brought to the battlefield by the guile of sorcerers. They are often armed simply with claws that rip and tear the enemy to shreds; some use psychic-based powers. The lesser daemons tend to be very poorly armoured and fall in great numbers to rapid-fire weaponry.");
    loreListTraitor.insert("Traitor Guard","Traitor Guard (or guardsmen) is a term given to the vast majority of disloyal mortals who have left the Imperial Guard and Planetary Defence Forces in order to fight for Chaos. They are not a single coherent force, but rather a faction of the infinite collection of warbands and hosts under the leadership of Chaos Champions and part of the The Lost and the Damned.");
    loreListTraitor.insert("Traitor Guard Notable Characters","Rutgher Kaine 'General' - Recoba 'Commander' - Seriadne Hekht 'Traitor General'");
    loreListTraitor.insert("Chaos SpaceMarine","The Chaos Space Marines are formed of the Adeptus Astartes who fell under the sway of Warmaster Horus and rebelled against the Emperor in an attempt to bring the material galaxy under the control of Chaos. A full nine legions left the side of the Emperor and joined Chaos, some choosing specific deities to worship while others went to Chaos Undivided. In the millennia since, other formations of Adeptus Astartes have also fallen to Chaos, from single squads to entire chapters.");
    loreListTraitor.insert("Chaos SpaceMarine Legions","Emperor's Children - Iron Warriors - Night Lords - World Eaters - Death Guard - Thousand Sons - Luna Wolves/Sons of Horus - Word Bearers - Alpha Legion (maybe?)");
    loreListTraitor.insert("Chaos SpaceMarine Primarchs","Fulgrim is the Primarch of the Emperor's Children - Perturabo is the Primarch of the Iron Warriors - Konrad Curze, also known as Night Haunter primarch of The Night Lords - Angron is the Primarch of the World Eaters (angry dude) - Mortarion (also known as the Death Lord, The Pale King, or the Reaper of Men). He was given command of the Death Guard Legion - Magnus the Red is the Primarch of the Thousand Sons - Horus Lupercal, Warmaster of the Great Crusade, (was)Primarch of the Sons of Horus - Lorgar Aurelian is the Primarch of the Word Bearers - (Alpharius technically)");
    loreListTraitor.insert("Chaos SpaceMarine Notable Characters","Abaddon the Despoiler, Warmaster of Chaos - Khârn the Betrayer - Ahriman, Exile of the Thousand Sons - Fabius Bile - Lucius the Eternal - Typhus, Host of the Destroyer Hive - Huron Blackheart, Master of the Red Corsairs");
    loreListTraitor.insert("Dark Mechanicum","The Dark Mechanicum, or Dark Mechanicus, referred to by themselves as the New Mechanicum, is a Chaos-affiliated counterpart to the Adeptus Mechanicus. Its origins trace back to those who pledged loyalty to Warmaster Horus at the end of the Great Crusade. The Dark Mechanicum serves as a dark 'mirror' to their counterparts within the Imperium.");
    loreListTraitor.insert("Suprise me","Magnus did nothing wrong (exept he kinda did)");
    //Alpha loreList
    loreListAlpha.insert("Primarchs","Alpharius and his twin Omegon (sometimes referred to collectively as just 'Alpharius Omegon')");
    loreListAlpha.insert("Notable Characters","Alpharius - Primarch,Omegon - Leader of the Effrit Stealth Squad / Primarch, Ingo Pech - First Captain, Mathias Herzog - Captain of the 2nd Company, Armillus Dynat - Harrowmaster, Exodus - Assassin, Phocron - Headhunter Prime, Siridor Vhen - Praetor, Sheed Ranko - Captain of the Lernaen Terminator Squad Killed during the Tenebrae Mission, Autilon Skorr - 'The Hydra's Headman' - Consul Delegatus of the Alpha Legion");
    loreListAlpha.insert("Suprise you!","We are all Alpharius");
    //linkinglist
    linkinglist.insert("test", "test");
}


