#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QString>
#include <iostream>
#include <QTimer>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void socket_Read_Data();
    void on_pushButton_clicked();

    void on_list_friend_number_clicked(const QModelIndex &index);

    void on_add_friend_clicked();

    void on_add_group_clicked();
    void on_timeout();

    void on_list_group_number_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QTimer *tim;

};

#endif // MAINWINDOW_H
