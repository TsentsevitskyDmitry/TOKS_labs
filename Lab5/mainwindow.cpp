#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   qRegisterMetaType<std::string>("std::string");
   QObject::connect(&ring, &TockenRing::dataArrived, this, &MainWindow::appendData);
   QObject::connect(&ring, &TockenRing::messageArrived, this, &MainWindow::appendMessage);
   ring.start();


}

MainWindow::~MainWindow()
{
    ring.stop();
    delete ui;
}

void MainWindow::appendData(std::string data)
{
    ui->main_edit->clear();
    ui->main_edit->append(QString::fromStdString(data));
}

void MainWindow::appendMessage(std::string data)
{
    //ui->main_edit->clear();
    ui->massages_edit->append(QString::fromStdString(data));
}

void MainWindow::on_reconnect_button_clicked()
{
    std::string listen = ui->listen_com_edit->text().toStdString();
    std::string write = ui->write_com_edit->text().toStdString();
    std::string name = ui->name_edit->text().toStdString();
    std::cout << "listen: " << listen << ", write: " << write << std::endl;
    ring.reconnect(listen, write, name);
}

void MainWindow::on_send_button_clicked()
{
    ring.post_write(ui->dest_addr->text().toStdString(), ui->message_edit->text().toStdString());
    ui->message_edit->setText("");
}

void MainWindow::on_master_button_clicked()
{
    ring.set_master(true);
    ui->master_button->setVisible(false);
}
