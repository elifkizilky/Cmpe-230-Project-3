#include <QtGui>
#include <QNetworkAccessManager>
#include <QtCore>
#include <QApplication>
#include <QTableWidget>
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtGui>
#include <QSizePolicy>
#include <QHeaderView>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <vector>
using namespace std;
class CryptoVals : public QLabel
{
    Q_OBJECT

    public:
      CryptoVals(QWidget *parent = 0);
      vector<string> *coins = new vector<string>;//stores data from the environment variable
      QNetworkAccessManager *manager ;
      QJsonArray coinList;//id,name and symbols of all coins

    public slots:
      void coinlist(); //coin symbol and id check
      void prices(QString coinstr); //get price
      void replyFinished(QNetworkReply * reply) ;
      void replyFinished2(QNetworkReply * reply);

    private:
      QTableWidget *table;

} ;
