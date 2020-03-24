#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), reader(com)
{
    ui->setupUi(this);
    chat = new ChatWindow(ui->textBrowser);
    qRegisterMetaType<std::string>("std::string");

    QObject::connect(&reader, &ReaderThread::dataArrived, this, &MainWindow::appendData);
    reader.start();
    QObject::connect(ui->lineEdit_2, SIGNAL(returnPressed()), ui->send_button, SLOT(click()));
}

MainWindow::~MainWindow()
{
    com.close();
    reader.stop();
    reader.wait();
    delete ui;
}

void MainWindow::appendData(std::string crc_data)
{
    bool isValid = com.checkCRC(crc_data);
    string data = com.cutCRC(crc_data);
    chat->appendRecievedCRC(isValid, data, crc_data);
}

void MainWindow::on_open_button_clicked()
{
    string port = ui->lineEdit->text().toStdString();
    int badurate = ui->comboBox->currentText().toInt();

    if (com.open(port, badurate)){
        ui->textBrowser->setTextColor(Qt::green);
        ui->textBrowser->append("Connection established");
        ui->textBrowser->setTextColor(Qt::black);
    }
    else {
        ui->textBrowser->setTextColor(Qt::red);
        ui->textBrowser->append("Error occured!");
        ui->textBrowser->setTextColor(Qt::black);
    }
}

void MainWindow::on_send_button_clicked()
{
    string data = ui->lineEdit_2->text().toStdString();
    string crcMessage = com.addCRC(data);
    com.write(crcMessage);
    chat->appendSendedCRC(data, crcMessage);
    ui->lineEdit_2->clear();
}
