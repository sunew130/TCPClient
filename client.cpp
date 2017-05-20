#include <QMessageBox>
#include <QString>

#include "client.h"
#include "ui_client.h"


client::client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);

    ui->clientSendpushButton->setEnabled (false);
    ui->disconnectpushButton->setEnabled (false);
    tcp=NULL;
    message=tr("<span style=\"color:red\">%1</span>");
}

client::~client()
{
    delete tcp;
    delete ui;
}


//�ͻ��˷�����Ϣ
void client::clientSendMessage ()
{
    QByteArray data;
    data.append(ui->clientMessagelineEdit->text());
    tcp->write(data);
    ui->messagetextBrowser->insertPlainText (tr("send message: %1 \n").arg (QString(data)));
}

void client::on_cCleanpushButton_clicked()
{
    ui->messagetextBrowser->setText ("");
}

//�ͻ������Ӱ�ť�ۺ���
void client::on_connectpushButton_clicked()
{
    serverIP = ui->clientIPlineEdit->text ();
    clientPort = ui->clientPortlineEdit->text ();
    if(serverIP.isEmpty () || clientPort.isEmpty ())
    {
        QMessageBox::warning (this, tr("Warnning"), tr("������IP��˿ںŲ���Ϊ��"));
        return;
    }

    if(tcp)
    {
        delete tcp;
    }
    tcp=new QTcpSocket(this);
    tcp->connectToHost (serverIP, clientPort.toInt ()); //���ӵ�����
    connect (tcp, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect (tcp, SIGNAL(connected()), this, SLOT(updateClientStatusConnect())); //��������״̬
    //connect (tcp, SIGNAL(disconnected()), this, SLOT(updateClientStatusDisonnect())); //���¶Ͽ�����״̬
    connect(tcp,SIGNAL(readyRead()),this,SLOT(readMessage()));
    ui->connectpushButton->setEnabled (false);
    ui->disconnectpushButton->setEnabled (true);
}

void client::on_disconnectpushButton_clicked()
{
    ui->clientMessagelineEdit->setText (tr("clientStop"));
    clientSendMessage ();
    ui->clientMessagelineEdit->setText (tr(""));
    ui->cStatuslabel->setText (tr("���ӶϿ�"));
    tcp->abort();
    delete tcp;
    tcp=NULL;
    ui->connectpushButton->setEnabled (true);
    ui->disconnectpushButton->setEnabled (false);
    ui->clientSendpushButton->setEnabled (false);


}

//�ͻ��˴�����ʾ
void client::displayError (QAbstractSocket::SocketError)
{
      QMessageBox::warning (this, tr("Warnning"), tcp->errorString ());
      tcp->close ();
      ui->connectpushButton->setEnabled (true);
      ui->disconnectpushButton->setEnabled (false);
      ui->clientSendpushButton->setEnabled (false);
}

void client::updateClientStatusConnect ()
{
    ui->cStatuslabel->setText (tr("������"));
    ui->connectpushButton->setEnabled (false);
    ui->disconnectpushButton->setEnabled (true);
    ui->clientSendpushButton->setEnabled (true);
}

//void client::updateClientStatusDisonnect()
//{
//    ui->cStatuslabel->setText (tr("������ֹͣ����"));
//    ui->connectpushButton->setEnabled (true);
//    ui->disconnectpushButton->setEnabled (false);
//    ui->clientSendpushButton->setEnabled (false);
//}

//��Ҫ�Լ���ӵĺ���
void client::updateStatus()
{
    ui->connectpushButton->setEnabled (false);
    ui->disconnectpushButton->setEnabled (true);
    ui->clientSendpushButton->setEnabled (true);
}

//�ͻ��˶�ȡ��Ϣ
void client::readMessage ()
{
    QByteArray data=tcp->readAll();
//    message = QString(data);
//    if(message.contains ("serverStop")) //����յ��ǿͻ��˶Ͽ����ӵ���Ϣ
//    {
//        ui->cStatuslabel->setText (tr("������ֹͣ����"));
//        ui->connectpushButton->setEnabled (true);
//        ui->disconnectpushButton->setEnabled (false);
//        ui->clientSendpushButton->setEnabled (false);
//       return;
//    }
    ui->messagetextBrowser->append(message.arg(tr("&lt;&lt;�յ���Ϣ:"))+QString(data));
}

void client::on_clientSendpushButton_clicked()
{
    QByteArray data;
    data.append(ui->clientMessagelineEdit->text());
    tcp->write(data);
    ui->messagetextBrowser->insertPlainText (tr("send message: %1 \n").arg (QString(data)));
}
