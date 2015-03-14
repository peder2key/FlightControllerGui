#include "setupserial.h"
#include "ui_setupserial.h"

setupSerial::setupSerial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setupSerial)
{
    ui->setupUi(this);

    ui->baudBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->baudBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudBox->addItem(QStringLiteral("Custom"));

    GetPortInfo();

}

setupSerial::~setupSerial()
{
    delete ui;
}

QString setupSerial::getPort()
{
    return myPort;
}

qint32 setupSerial::getBaud()
{
    return myBaud;
}

void setupSerial::setPort(QString savedPort)
{
    int search = ui->portBox->findText(savedPort);
    if(search != -1)
    {
        ui->portBox->setCurrentIndex(search);
        myPort = ui->portBox->currentText();
    }
}

void setupSerial::setBaud(qint32 savedBaud)
{
    QString temp = QString::number(savedBaud);
    int search = ui->baudBox->findText(temp);
    if(search != -1)
    {
        ui->baudBox->setCurrentIndex(search);
        myBaud = ui->baudBox->currentText().toInt();

    }
    else
    {
        myBaud = savedBaud;
    }
}





void setupSerial::GetPortInfo()
{
    ui->baudBox->setCurrentIndex(4);
    ui->portBox->clear();
    static const QString blankString = QObject::tr("N/A");
    QString description;
    QString manufacturer;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->portBox->addItem(list.first(), list);
    }

}

void setupSerial::UpdateList()
{
    myPort = ui->portBox->currentText();
    myBaud = ui->baudBox->currentText().toInt();

}

void setupSerial::on_pushButton_clicked()
{
    UpdateList();
    emit portBaudSetting(myPort, myBaud);
    this->hide();


}

void setupSerial::on_pushButton_2_clicked()
{
    this->hide();
}
