#include "mainwindow.h"
#include <QApplication>
#include<QTranslator>
#include<QtDebug>
#include <QFile>
#include <QTextCodec>
#include"qreadconfig.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QApplication::setStyle("fusion");
//    QTranslator qtTranslator;
//   if(qtTranslator.load("testQtCreator_english.qm"))
//   {
 //       qDebug()<<"sucess";
//   }else
//       qDebug()<<"fail";
//   a.installTranslator(&qtTranslator);

    QTextCodec *codec=QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QFile *qss = new QFile("://style.qss");
      qss->open(QFile::ReadOnly);
      QString styleSheet = qss->readAll();
      a.setStyleSheet(styleSheet);
      qss->close();
      qss->deleteLater();
//      WaitDialog w;
//      w.exec();

      MainWindow w;
      //w.setFont(QFont());
      w.show();

    return a.exec();
}
