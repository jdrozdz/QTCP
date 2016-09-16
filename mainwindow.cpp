#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doConnect()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    // this is not blocking call
    socket->connectToHost(ui->lineEdit->text(), ui->lineEdit_2->text().toInt());

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

void MainWindow::connected()
{
    qDebug() << "connected...";

    // Hey server, tell me about you.
    socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
}

void MainWindow::disconnected()
{
    qDebug() << "disconnected...";
}

void MainWindow::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void MainWindow::readyRead()
{
    qDebug() << "reading...";

    // read the data from the socket
    QByteArray resp = socket->readAll();
    QString serv_resp = QString("<div style='color: blue;'>%1</div>").arg(resp.toStdString().c_str());
    ui->plainTextEdit->appendHtml(serv_resp);
}

void MainWindow::on_pushButton_released()
{
    doConnect();
}

void MainWindow::on_pushButton_2_released()
{
    socket->write(ui->lineEdit_3->text().toStdString().c_str());
}
