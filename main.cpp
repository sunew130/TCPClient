#include <QApplication>
#include <QTextCodec>   //�������֧��2-1

#include "client.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    //QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//�������֧��2-2

//    Widget w;
//    w.show();
    client w1;
    w1.show();

    return a.exec();
}
