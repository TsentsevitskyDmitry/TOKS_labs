#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SerialAPI.h"
#include "readerthread.h"
#include "chatwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    SerialPort com;
    ReaderThread reader;
    ChatWindow* chat;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_send_button_clicked();
    void on_open_button_clicked();
    void appendData(std::string crc_data);
};

#endif // MAINWINDOW_H
