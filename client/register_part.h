#ifndef REGISTER_PART_H
#define REGISTER_PART_H

#include <QDialog>

namespace Ui {
class p_regis;
}

class p_regis : public QDialog
{
    Q_OBJECT

public:
    explicit p_regis(QWidget *parent = 0);
    ~p_regis();

private:
    Ui::p_regis *ui;
};

#endif // REGISTER_PART_H
