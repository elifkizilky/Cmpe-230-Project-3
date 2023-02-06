#include "CryptoVals.h"
#include <QtGui>
#include <QNetworkAccessManager>
#include <QtCore>
#include <QTableWidget>
#include "Qt"
#include <QPushButton>
#include <QHBoxLayout>
#include <QtGui>
#include <QSizePolicy>
#include <QHeaderView>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QList>
#include <QVector>
#include <QRegExp>
#include <QAbstractScrollArea>

using namespace std;

CryptoVals::CryptoVals(QWidget *parent) : QLabel(parent)
{
    table = new QTableWidget();

    // if you already have the environment variable comment out this line or set it accordingly

    // -------------
    qputenv("MYCRYPTOCONVERT","/home/cmpe250student/Desktop/testcases_v2/testcase47.txt");
    // ------------

    QString fileName = QString::fromLocal8Bit(qgetenv("MYCRYPTOCONVERT"));
    ifstream infile;
    infile.open(fileName.toStdString());

    string line = "";
    while(getline(infile, line)){
        QRegExp rx("\\s*(.+)\\s*");//trim
        QString crypto;
        int pos = 0;
        if ( rx.indexIn(QString::fromStdString(line), pos) != -1 ) {
            crypto = rx.cap(1);    // coin name
            (*this->coins).push_back(crypto.toStdString());
        }
    }
    //table adjustments
    table->setColumnCount(3);
    table->setRowCount((*this->coins).size());
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->resizeColumnsToContents();
    QStringList fonts = {"USD", "EUR", "GBP" };
    table->setHorizontalHeaderLabels(fonts);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(table);
    setLayout(layout);
    resize(660,480);
    coinlist();
}
/**
 * @brief CryptoVals::replyFinished
 * @param reply
 * Gets the conversion rate of given coins from the price request to the website
 * and adds them to table accordingly
 */
void CryptoVals::replyFinished(QNetworkReply *reply)  {

    // read the data fetched from the web site
    QJsonDocument data = QJsonDocument::fromJson(reply->readAll());
    // use json structure to get values
    QJsonObject json = data.object();
    int row = 0;

    for(string&  coin : (*this->coins)){

        QString id = QString::fromStdString(coin);
        //taking the price info of the coin
        QJsonValue valList = json.value(id);
        QJsonObject obj = valList.toObject();
        QJsonValue valobjusd = obj.value("usd");
        QJsonValue valobjeur = obj.value("eur");
        QJsonValue valobjgbp = obj.value("gbp");
        //converting the values to suitable type for table
        double doubleValusd = valobjusd.toDouble();
        QString strusd = QString::number(doubleValusd);

        double doubleValeur = valobjeur.toDouble();
        QString streur = QString::number(doubleValeur);

        double doubleValgbp = valobjgbp.toDouble();
        QString strgbp = QString::number(doubleValgbp);

        //adding values to table
        QTableWidgetItem *itab1= new QTableWidgetItem;
        QTableWidgetItem *itab2= new QTableWidgetItem;
        QTableWidgetItem *itab3= new QTableWidgetItem;
        itab1->setText(strusd);
        table->setItem(row, 0, itab1);
        itab2->setText(streur);
        table->setItem(row, 1, itab2);
        itab3->setText(strgbp);
        table->setItem(row, 2, itab3);

        row++;
    }
    table->resizeColumnsToContents();
}
/**
 * @brief CryptoVals::replyFinished2
 * @param reply
 * Gets the list of all coins to match the given names and symbols with an appropriate id
 */
void CryptoVals::replyFinished2(QNetworkReply *reply)  {
    // read the data fetched from the web site
    QJsonDocument data = QJsonDocument::fromJson(reply->readAll());
    this->coinList = data.array();// turn it into jsonarray
    QStringList coinsqstr;

    for(string&  coin : (*this->coins))
    {
        //traverse the json array to find the coin ids
        for (const QJsonValue data_element : this->coinList)
        {
            QString coinData  = QString::fromStdString(coin);
            if(coinData.toLower() == data_element.toObject()["symbol"].toString().toLower()){
                //given coin data is symbol
                coinsqstr.append(data_element.toObject()["id"].toString());
                break;
            }
            else if(coinData.toLower() == data_element.toObject()["name"].toString().toLower()){
                //given coin data is name
                coinsqstr.append(data_element.toObject()["id"].toString());
                break;
            }
            else if(coinData.toLower() == data_element.toObject()["id"].toString().toLower()){
                coinsqstr.append(data_element.toObject()["id"].toString());
                break;
            }
        }
    }
    delete this->coins;//clearing the coin list
    this->coins = new vector<string>;
    //refilling it with coin ids
    for(auto& coin : coinsqstr){
        QString str = coin;
        this->coins->push_back(str.toStdString());
    }
    table->setVerticalHeaderLabels(coinsqstr);
    QString coinstr = coinsqstr.join(",");
    prices(coinstr);//website requests for coin prices
}
/**
 * @brief CryptoVals::coinlist
 * Making network request for coin list
 */
void CryptoVals::coinlist(){

    manager = new QNetworkAccessManager(this) ;
    connect(manager, SIGNAL(finished(QNetworkReply *)),this, SLOT(replyFinished2(QNetworkReply *)));
    QString url = "https://api.coingecko.com/api/v3/coins/list";
    // Initiate retrieval of  the data from the web page.
    // When completed, slot replyFinished will be called.
    manager->get(QNetworkRequest(QUrl(url)));

}
/**
 * @brief CryptoVals::prices
 * @param coinstr
 * Making network request for coin price
 */
void CryptoVals::prices(QString coinstr){

    manager = new QNetworkAccessManager(this) ;
    connect(manager, SIGNAL(finished(QNetworkReply *)),this, SLOT(replyFinished(QNetworkReply *)));
    QString url = "https://api.coingecko.com/api/v3/simple/price?ids="+coinstr+"&vs_currencies=usd%2Ceur%2Cgbp";
    // Initiate retrieval of  the data from the web page.
    // When completed, slot replyFinished will be called.
    manager->get(QNetworkRequest(QUrl(url)));
}


