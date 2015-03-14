#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <QObject>
#include "CommandLibrary.h"
#include "protocol.h"

#include "bluetoothdialog.h"

class Telegram : public QObject
{
    Q_OBJECT
public:
    explicit Telegram(QObject *parent = 0);
    bool Connect(QString port, int baud);
    void Disconnect(void);
    void showBluetoothDialog();
    //Protocol *serialProtocol;
    BluetoothDialog *serialProtocol;

private:

public slots:
    void Request(char command, char direction);
    void Process(quint8& command, quint8& direction, QByteArray& payload);

signals:
    void UpdateStructs();
    void ReceivedPid();

};


#endif // TELEGRAM_H
