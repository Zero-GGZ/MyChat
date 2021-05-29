#ifndef ADD_TARGET_ID_H
#define ADD_TARGET_ID_H

#include <QDialog>
#include <QAbstractButton>
namespace Ui {
class add_target_id;
}

class add_target_id : public QDialog
{
    Q_OBJECT

public:
    explicit add_target_id(QWidget *parent = 0);
    ~add_target_id();

private slots:

    void on_add_id_clicked(QAbstractButton *button);

private:
    Ui::add_target_id *ui;
};

#endif // add_target_id_H
