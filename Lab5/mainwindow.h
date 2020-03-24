#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "tockenring.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void appendData(std::string data);
    void appendMessage(std::string data);

    void on_reconnect_button_clicked();

    void on_send_button_clicked();

    void on_master_button_clicked();

private:
    Ui::MainWindow *ui;
    TockenRing ring;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



};

#endif // MAINWINDOW_H
