#include <sstream>
#include <bitset>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), reader(comin)
{
    ui->setupUi(this);   
    qRegisterMetaType<std::string>("std::string");

    QObject::connect(&reader, &ReaderThread::dataArrived, this, &MainWindow::appendData);
    reader.start();
    srand(time(NULL));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendData(string data)
{
    noisedata = data;
    ui->input_browser->clear();
    ui->output_browser->clear();

    QString bits = QString::fromStdString(GetBitsString(data));
    ui->output_browser->append(bits);
    ui->input_browser->append(bits);
}

string MainWindow::GetBitsString(string &data)
{
    stringstream bits;
    for(int i = 0; i < data.length(); ++i){
        bitset<8> x(data[i]);
        bits << x;
    }

    return bits.str();
}

void MainWindow::on_connect_in_clicked()
{
    string port = ui->com_in->text().toStdString();
    int badurate = 19200;

    if (comin.open(port, badurate)){
        ui->input_browser->setTextColor(Qt::green);
        ui->input_browser->append("Connection established");
        ui->input_browser->setTextColor(Qt::black);
    }
    else {
        ui->input_browser->setTextColor(Qt::red);
        ui->input_browser->append("Error occured!");
        ui->input_browser->setTextColor(Qt::black);
    }
}

void MainWindow::on_connect_out_clicked()
{
    string port = ui->com_out->text().toStdString();
    int badurate = 19200;

    if (comout.open(port, badurate)){
        ui->output_browser->setTextColor(Qt::green);
        ui->output_browser->append("Connection established");
        ui->output_browser->setTextColor(Qt::black);
    }
    else {
        ui->output_browser->setTextColor(Qt::red);
        ui->output_browser->append("Error occured!");
        ui->output_browser->setTextColor(Qt::black);
    }
}

void MainWindow::on_add_error_button_clicked()
{
    int byte = rand() % noisedata.length();
    int bit = rand() % 8;
    int mask = 1 << bit;

    noisedata[byte] = (noisedata[byte] | mask) != noisedata[byte] ? (noisedata[byte] | mask) : (noisedata[byte] & ~mask);
    ui->output_browser->clear();
    ui->output_browser->append(QString::fromStdString(GetBitsString(noisedata)));
}

void MainWindow::on_send_button_clicked()
{
    comout.write(noisedata);
    cout << "hh " << noisedata << endl;
}
