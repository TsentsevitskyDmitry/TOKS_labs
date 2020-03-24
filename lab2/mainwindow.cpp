#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), reader(com)
{
    ui->setupUi(this);

    QObject::connect(&reader, SIGNAL(dataArrived(QString)), ui->textBrowser, SLOT(append(QString)));
    reader.start();
    QObject::connect(ui->lineEdit_2, SIGNAL(returnPressed()), ui->pushButton_2, SLOT(click()));
}

MainWindow::~MainWindow()
{
    com.close();
    reader.stop();
    reader.wait();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
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

void MainWindow::on_pushButton_2_clicked()
{
    string data = ui->lineEdit_2->text().toStdString();
    com.write(data);
    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->append("> " + ui->lineEdit_2->text());
    ui->textBrowser->setTextColor(Qt::black);
    ui->lineEdit_2->clear();
}

void MainWindow::on_lineEdit_2_returnPressed()
{

}
