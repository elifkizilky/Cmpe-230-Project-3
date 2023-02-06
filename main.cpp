#include <QtCore>
#include <QApplication>
#include "CryptoVals.h"


int main(int argc,char *argv[])
{
   QApplication a(argc, argv); 
   CryptoVals my ;

   my.show(); 

   return a.exec();
}
