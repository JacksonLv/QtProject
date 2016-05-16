#include "widget.h"
#include "ui_widget.h"

//static uint8_t rgb_transp = 170;
//static uint16_t rgb_x_base = 360;
//static uint8_t rgb_y_base = 100;
//static uint8_t rgb_w = 100;
//static uint8_t rgb_h = 20;

static int rgb_temp[8][3]={{0,0,0},{0,0,0}};
static uint16_t hue[8]={0,0,0,0,0,0};

QJsonObject RGB;
QJsonObject json;
QString string;
QColor bgclor;

// QString >> QJson
QJsonObject getJsonObjectFromString(const QString jsonString){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< jsonString.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}
// QJson >> QString
QString getStringFromJsonObject(const QJsonObject& jsonObject){
    return QString(QJsonDocument(jsonObject).toJson());
    //return QJsonDocument(jsonObject).toJson();
}

const char* getcharPFromQStirng(const QString jsonString){
    const char *jsdata = "";
    QByteArray ba = jsonString.toLatin1();

    jsdata = ba.data();

    return jsdata;
}



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    QTimer *reflashTimer = new QTimer(this);
    connect( ui->ConnectButton, SIGNAL( released() ), this, SLOT( connectButton_clicked() ) );
    connect( ui->DisconnectButton, SIGNAL( released() ), this, SLOT( disconnectButton_clicked() ) );
    connect( ui->horizontalSlider0,SIGNAL( sliderPressed() ), this, SLOT( silder0Event() ) );
    connect( ui->horizontalSlider1,SIGNAL( sliderPressed() ), this, SLOT( silder1Event() ) );
    connect( ui->horizontalSlider2,SIGNAL( sliderPressed() ), this, SLOT( silder2Event() ) );
    connect( ui->horizontalSlider3,SIGNAL( sliderPressed() ), this, SLOT( silder3Event() ) );
    connect( ui->horizontalSlider4,SIGNAL( sliderPressed() ), this, SLOT( silder4Event() ) );
    connect( ui->horizontalSlider5,SIGNAL( sliderPressed() ), this, SLOT( silder5Event() ) );
    connect( ui->horizontalSlider6,SIGNAL( sliderPressed() ), this, SLOT( silder6Event() ) );
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
             this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(reflashTimer,SIGNAL(timeout()),SLOT(paintEvent()));
    //QTimer::singleShot(100000, &app, SLOT(quit()));
    //reflashTimer->start(10);

}

Widget::~Widget()
{
    delete ui;
}


void Widget::newConnect()
{
    blockSize = 0; //初始化其为0
    tcpSocket->abort(); //取消已有的连接

    //连接到主机，这里从界面获取主机地址和端口号
    tcpSocket->connectToHost(ui->hostLineEdit->text(),
                             ui->portLineEdit->text().toInt());
    QDateTime n=QDateTime::currentDateTime();
    QDateTime now;
    do{
    now=QDateTime::currentDateTime();
    } while (n.secsTo(now)<=3);//6为需要延时的秒数
    if(tcpSocket->state() == 3)
    {
        ui->TextEdit->append("Connected");
        //tcpSocket->write(getcharPFromQStirng(creatJson(1,2,3,4,5,6)));
    }
    else ui->TextEdit->append("error");

}

void Widget::readMessage()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_6);
    //设置数据流版本，这里要和服务器端相同
    if(blockSize==0) //如果是刚开始接收数据
    {
       //判断接收的数据是否有两字节，也就是文件的大小信息
       //如果有则保存到blockSize变量中，没有则返回，继续接收数据
       if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
       in >> blockSize;
    }
    if(tcpSocket->bytesAvailable() < blockSize) return;
    //如果没有得到全部的数据，则返回，继续接收数据
    in >> message;
    //将接收到的数据存放到变量中
    ui->messageLabel->setText(message);
    //显示接收到的数据
}

void Widget::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString(); //输出错误信息
}

void Widget::connectButton_clicked() //连接按钮
{
    newConnect(); //请求连接
}

void Widget::disconnectButton_clicked()
{
    tcpSocket->abort();
    //QString output = tr("DisConnected");
    if(tcpSocket->state() == 0)
    {
        ui->TextEdit->append("DisConnected");
    }
    else ui->TextEdit->append("Still Connected");
}


QString Widget::creatJson(uint8_t opcode,uint8_t id,uint16_t timer,uint8_t r,uint8_t g,uint8_t b)
{

    RGB.insert("OpCode", opcode);
    RGB.insert("ID",id);
    RGB.insert("Timer",timer);
    RGB.insert("R", r);
    RGB.insert("G", g);
    RGB.insert("B", b);
    string = getStringFromJsonObject(RGB);
    ui->TextEdit->append(string);

    //tcpSocket->write(getcharPFromQStirng(string));
    return string;

}


void Widget::paintEvent(QPaintEvent *)
{
    /*QPainter painter(this);
    QColor huecolor;

    QBrush brush(huecolor);

    hue[0] = ui->horizontalSlider0->value();
    hue[1] = ui->horizontalSlider1->value();
    hue[2] = ui->horizontalSlider2->value();
    hue[3] = ui->horizontalSlider3->value();
    hue[4] = ui->horizontalSlider4->value();
    hue[5] = ui->horizontalSlider5->value();
    hue[6] = ui->horizontalSlider6->value();
    hue[7] = ui->horizontalSlider7->value();*/
    if(hue[0] == 0)
    {
        ui->nano0ButtonOff->setChecked(true);
    }
    else if(hue[0] != 0)
    {
        ui->nano0ButtonOn->setChecked(true);
    }
    if(hue[1] == 0)
    {
        ui->nano1ButtonOff->setChecked(true);
    }
    else if(hue[1] != 0)
    {
        ui->nano1ButtonOn->setChecked(true);
    }
    if(hue[2] == 0)
    {
        ui->nano2ButtonOff->setChecked(true);
    }
    else if(hue[2] != 0)
    {
        ui->nano2ButtonOn->setChecked(true);
    }
    if(hue[3] == 0)
    {
        ui->nano3ButtonOff->setChecked(true);
    }
    else if(hue[3] != 0)
    {
        ui->nano3ButtonOn->setChecked(true);
    }
    if(hue[4] == 0)
    {
        ui->nano4ButtonOff->setChecked(true);
    }
    else if(hue[4] != 0)
    {
        ui->nano4ButtonOn->setChecked(true);
    }
    if(hue[5] == 0)
    {
        ui->nano5ButtonOff->setChecked(true);
    }
    else if(hue[5] != 0)
    {
        ui->nano5ButtonOn->setChecked(true);
    }
    if(hue[6] == 0)
    {
        ui->nano6ButtonOff->setChecked(true);
    }
    else if(hue[6] != 0)
    {
        ui->nano6ButtonOn->setChecked(true);
    }
    if(hue[7] == 0)
    {
        ui->nano7ButtonOff->setChecked(true);
    }
    else if(hue[7] != 0)
    {
        ui->nano7ButtonOn->setChecked(true);
    }
/*
//    for(uint8_t i = 0;i<8;i++)
    {

        //if(hue[i] == 0)
        {
            //huecolor.setHsl(hue[i],0,0);
            //huecolor.getRgb(rgb_temp[i][0],rgb_temp[i][0],rgb_temp[i][0])
        }
        //else if(hue[i] != 0)
        {
            huecolor.setHsl(hue[0],120,150);
        }
//        brush.setColor(huecolor);
//        painter.setBrush(brush);
//        if( i%2 != 0)
//        {
//            painter.drawRect(rgb_x_base+130,(i+1)/2*rgb_y_base,rgb_w,rgb_h);
//        }
//        else painter.drawRect(rgb_x_base,(i+2)/2*rgb_y_base,rgb_w,rgb_h);
        //ui->gridLayout0->setColor (QPalette::Background, QColor (0, 0 , 0, 255));
        QPalette pe;
        setAutoFillBackground(true);
        pe.setColor(QPalette::Window,huecolor);
        ui->gridWidget0->setAutoFillBackground(true);
        ui->gridWidget0->setPalette(pe);
    }*/


}

void Widget::silder0Event()
{

    QColor huecolor;
    QPalette pe;
    setAutoFillBackground(true);

    hue[0] = ui->horizontalSlider0->value();
    huecolor.setHsl(hue[0],120,150);
    if(hue[0] == 0)
    {
        ui->nano0ButtonOff->setChecked(true);
        huecolor.setHsl(hue[0],0,80);
        rgb_temp[0][0]=0;
        rgb_temp[0][1]=0;
        rgb_temp[0][2]=0;
    }
    else if(hue[0] != 0)
    {
        ui->nano0ButtonOn->setChecked(true);
        pe.setColor(QPalette::Window,huecolor);
        huecolor.getRgb(&rgb_temp[0][0],&rgb_temp[0][1],&rgb_temp[0][2]);
    }
    pe.setColor(QPalette::Window,huecolor);

    ui->gridWidget0->setAutoFillBackground(true);
    ui->gridWidget0->setPalette(pe);

    if(tcpSocket->state() == 3)
    {
        tcpSocket->write(getcharPFromQStirng(creatJson(1,0,0,rgb_temp[0][0],rgb_temp[0][1],rgb_temp[0][2])));
    }
    else ui->TextEdit->append("Socket not connected!!");
}

void Widget::silder1Event()
{
    QColor huecolor;
    QPalette pe;
    setAutoFillBackground(true);
    hue[1] = ui->horizontalSlider1->value();
    huecolor.setHsl(hue[1],120,150);
    if(hue[1] == 0)
    {
        ui->nano1ButtonOff->setChecked(true);
        huecolor.setHsl(hue[1],0,80);


    }
    else if(hue[1] != 0)
    {
        ui->nano1ButtonOn->setChecked(true);

    }
    pe.setColor(QPalette::Window,huecolor);

    ui->gridWidget1->setAutoFillBackground(true);
    ui->gridWidget1->setPalette(pe);
}

void Widget::silder2Event()
{
    QColor huecolor;

    hue[2] = ui->horizontalSlider2->value();
    huecolor.setHsl(hue[2],120,150);
    if(hue[2] == 0)
    {
        ui->nano2ButtonOff->setChecked(true);
        huecolor.setHsl(hue[2],0,80);
    }
    else if(hue[2] != 0)
    {
        ui->nano2ButtonOn->setChecked(true);
    }
    QPalette pe;
    setAutoFillBackground(true);
    pe.setColor(QPalette::Window,huecolor);
    ui->gridWidget2->setAutoFillBackground(true);
    ui->gridWidget2->setPalette(pe);
}

void Widget::silder3Event()
{

    QColor huecolor;

    hue[3] = ui->horizontalSlider3->value();
    huecolor.setHsl(hue[3],120,150);
    if(hue[3] == 0)
    {
        ui->nano3ButtonOff->setChecked(true);
        huecolor.setHsl(hue[3],0,80);
    }
    else if(hue[3] != 0)
    {
        ui->nano3ButtonOn->setChecked(true);
    }
    QPalette pe;
    setAutoFillBackground(true);
    pe.setColor(QPalette::Window,huecolor);
    ui->gridWidget3->setAutoFillBackground(true);
    ui->gridWidget3->setPalette(pe);
}

void Widget::silder4Event()
{

    QColor huecolor;
    hue[4] = ui->horizontalSlider4->value();
    huecolor.setHsl(hue[4],120,150);
    if(hue[4] == 0)
    {
        ui->nano4ButtonOff->setChecked(true);
        huecolor.setHsl(hue[4],0,80);
    }
    else if(hue[4] != 0)
    {
        ui->nano4ButtonOn->setChecked(true);
    }
    QPalette pe;
    setAutoFillBackground(true);
    pe.setColor(QPalette::Window,huecolor);
    ui->gridWidget4->setAutoFillBackground(true);
    ui->gridWidget4->setPalette(pe);
}

void Widget::silder5Event()
{
    QColor huecolor;

    hue[5] = ui->horizontalSlider5->value();
    huecolor.setHsl(hue[5],120,150);
    if(hue[5] == 0)
    {
        ui->nano5ButtonOff->setChecked(true);
        huecolor.setHsl(hue[5],0,80);
    }
    else if(hue[5] != 0)
    {
        ui->nano5ButtonOn->setChecked(true);
    }
    QPalette pe;
    setAutoFillBackground(true);
    pe.setColor(QPalette::Window,huecolor);
    ui->gridWidget5->setAutoFillBackground(true);
    ui->gridWidget5->setPalette(pe);
}

void Widget::silder6Event()
{

    QColor huecolor;

    hue[6] = ui->horizontalSlider6->value();
    huecolor.setHsl(hue[6],120,150);
    if(hue[6] == 0)
    {
        ui->nano6ButtonOff->setChecked(true);
        huecolor.setHsl(hue[6],0,80);
    }
    else if(hue[6] != 0)
    {
        ui->nano6ButtonOn->setChecked(true);
    }
    QPalette pe;
    setAutoFillBackground(true);
    pe.setColor(QPalette::Window,huecolor);
    ui->gridWidget6->setAutoFillBackground(true);
    ui->gridWidget6->setPalette(pe);
}

void Widget::silder7Event()
{
    QColor huecolor;
    hue[7] = ui->horizontalSlider7->value();
    huecolor.setHsl(hue[7],120,150);
    if(hue[7] == 0)
    {
        ui->nano7ButtonOff->setChecked(true);
        huecolor.setHsl(hue[7],0,80);
    }
    else if(hue[7] != 0)
    {
        ui->nano7ButtonOn->setChecked(true);
    }
    QPalette pe;
    setAutoFillBackground(true);
    pe.setColor(QPalette::Window,huecolor);
    ui->gridWidget7->setAutoFillBackground(true);
    ui->gridWidget7->setPalette(pe);
}

/*{
    QColor huecolor;
    huecolor.setHsl(hue[7],0,150);
    ui->horizontalSlider0->setTickPosition(0);
    QPalette pe;
    setAutoFillBackground(true);
    pe.setColor(QPalette::Window,huecolor);
    ui->gridWidget7->setAutoFillBackground(true);
    ui->gridWidget7->setPalette(pe);
}*/
