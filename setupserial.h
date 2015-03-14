#ifndef SETUPSERIAL_H
#define SETUPSERIAL_H

#include <QDialog>
//#include <QSerialPort>
//#include <QSerialPortInfo>

#include <QIntValidator>
#include <QString>

namespace Ui {
class setupSerial;
}

class setupSerial : public QDialog
{
    Q_OBJECT

public:
    explicit setupSerial(QWidget *parent = 0);
    ~setupSerial();
    QString getPort(void);
    qint32 getBaud(void);
    void setPort(QString savedPort);
    void setBaud(qint32 savedBaud);
    void GetPortInfo(void);

private slots:



    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::setupSerial *ui;

    QString myPort;
    qint32 myBaud;
    void UpdateList(void);

signals:
    void portBaudSetting(QString port, qint32 baud);
};

#endif // SETUPSERIAL_H
