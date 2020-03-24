#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialapi.h"
#include "readerthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_in_clicked();
    void on_connect_out_clicked();
    void on_add_error_button_clicked();
    void on_send_button_clicked();
    void appendData(std::string data);


private:
    Ui::MainWindow *ui;
    SerialPort comin;
    SerialPort comout;
    ReaderThread reader;
    string noisedata;

    string GetBitsString(string& data);
};

#endif // MAINWINDOW_H
