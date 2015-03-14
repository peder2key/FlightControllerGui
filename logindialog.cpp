#include "logindialog.h"
#include "ui_logindialog.h"

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);

    ui->passwordlineEdit->setEchoMode(QLineEdit::Password);
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::on_buttonBox_accepted()
{

    QString strPassword = ui->passwordlineEdit->text();

    if(!strPassword.isEmpty() && ui->usercomboBox->currentIndex() == 1)
    {
        if(strPassword.toUpper() == "VIMANA")
        {

            qDebug() << "True";
            emit acceptedLogin(true);

        }
        else
        {
            emit acceptedLogin(false);
            qDebug() << "False";
            QMessageBox::critical(this, "Wrong password", "You have entered wrong password\nTry again");
        }

    }
    else
    {
        emit acceptedLogin(false);
        qDebug() << "false";
         QMessageBox::critical(this, "Wrong password", "You have entered wrong password\nTry again");
    }

}
