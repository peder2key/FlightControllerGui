#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = 0);
    ~loginDialog();

signals:
    void acceptedLogin(bool);

private slots:
       void on_buttonBox_accepted();

private:
    Ui::loginDialog *ui;
};

#endif // LOGINDIALOG_H
