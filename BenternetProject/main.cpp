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
#include <DiceRoller.h>
#include <WarhammerLore.h>
int main( int argc, char *argv[] )
{
    /*CURL *hnd = curl_easy_init();

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_URL, "https://dice-roll.p.rapidapi.com/roll/d/20");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "X-RapidAPI-Key: 6e6186f675msh38a8d432c2b9e9bp14105bjsn6827b6f25026");
    headers = curl_slist_append(headers, "X-RapidAPI-Host: dice-roll.p.rapidapi.com");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    CURLcode ret = curl_easy_perform(hnd);
*/
    QCoreApplication a(argc, argv);
    DiceRoller * roller = new DiceRoller(&a);
    WarhammerLore * WarLore = new WarhammerLore(%b);

    return a.exec(), b.exec();
}
