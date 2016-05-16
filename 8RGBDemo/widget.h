#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <QPainter>
#include <QPaintDevice>
#include <QTimer>
#include <QColor.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;
    QString message;  //存放从服务器接收到的字符串
    quint16 blockSize; //存放文件的大小信息
    //QPainter *painter0;
    //QPainter *painter1;
    //QBrush *brush;

private slots:
    void newConnect(); //连接服务器
    void readMessage();  //接收数据
    void displayError(QAbstractSocket::SocketError);  //显示错误
    void connectButton_clicked();
    void disconnectButton_clicked();
    QString creatJson(uint8_t opcode,uint8_t id,uint16_t Timer,uint8_t r,uint8_t g,uint8_t b);
    void silder0Event();
    void silder1Event();
    void silder2Event();
    void silder3Event();
    void silder4Event();
    void silder5Event();
    void silder6Event();
    void silder7Event();


protected:
    void paintEvent(QPaintEvent *);



};

#endif // WIDGET_H
