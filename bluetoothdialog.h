#ifndef BLUETOOTHDIALOG_H
#define BLUETOOTHDIALOG_H

#include <QDialog>

#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <QMenu>
#include <QDebug>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothaddress.h>
#include <QRect>
#include <QDesktopWidget>
#include <QMap>
#include <QSettings>
#include <QBluetoothSocket>
#include <QMessageBox>


#include "checksum.h"


QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceDiscoveryAgent)
QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceInfo)
QT_FORWARD_DECLARE_CLASS(QBluetoothServer)
QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

namespace Ui {
class BluetoothDialog;
}

class BluetoothDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BluetoothDialog(QWidget *parent = 0);
    ~BluetoothDialog();
    QBluetoothSocket *serialPort;

    QString getBluetoothAddress() const;
    void setBluetoothAddress(const QString &value);

private slots:
    void addDevice(const QBluetoothDeviceInfo&info);
    void scanFinished(void);
    void stopScanning(void);

    void on_pushButtonScan_clicked();

    void on_pushButtonOk_clicked();

private slots:
     void ReadFrame(void);

public slots:
     void CreateFrameAndSend(char &command, char &direction, QByteArray& payload);

signals:
     void FrameComplete(quint8& command, quint8& direction, QByteArray& payload);

private:
    Ui::BluetoothDialog *ui;

    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    void updateAddress(void);
    QString BluetoothAddress;
    QBluetoothLocalDevice localdevice;
    //BLUETOOTH_INFO BluetoothInfo;
    QMap<QString, QString> bluetoothDevices;
    enum FrameState { Start,Direction,Length,Command,Payload,CRC,End };
    enum Identifier { StartByte = '#', NoPayload = 0x00 };
    struct FrameData { quint8 start; quint8 direction; quint8 command; quint8 length;
                       QByteArray payload; quint16 crc; };
    FrameData incomingFrame;

    Checksum *checksum;
    quint8 frameCounter;

    FrameState CaseStart(quint8 data);
    FrameState CaseDirection(quint8 data);
    FrameState CaseCommand(quint8 data);
    FrameState CaseLength(quint8 data);
    FrameState CasePayload(quint8 data);
    FrameState CaseCRC(quint8 data);
    FrameState CheckCRC(void);




};

#endif // BLUETOOTHDIALOG_H
