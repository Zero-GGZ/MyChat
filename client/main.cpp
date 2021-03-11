#include "mainwindow.h"
#include <QApplication>
#include "login.h"
#include "common.h"
#include "dialog.h"
//#include <QTextCodec>

extern User global_user;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login w_login;
    w_login.setWindowTitle("要聊");

begin:
    if(w_login.exec()==QDialog::Accepted)
    {
        Dialog w_register;
        w_register.setWindowTitle("要聊");
        if(w_register.exec()==QDialog::Accepted)
            goto begin;
    }
    if(global_user.is_login_succeed)
    {
        MainWindow w;
        w.setWindowTitle("要聊");
        w.show();
        return a.exec();
    }
    else
        return 0;
}
