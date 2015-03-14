#include "bluetoothdialog.h"
#include "ui_bluetoothdialog.h"

BluetoothDialog::BluetoothDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BluetoothDialog)
{
    ui->setupUi(this);

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),this, SLOT(addDevice(QBluetoothDeviceInfo)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));
    connect(ui->listViewBluetoothDevices, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(stopScanning()));

    serialPort = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
     connect(serialPort,SIGNAL(readyRead()),this,SLOT(ReadFrame()));

      checksum = new Checksum;
}

BluetoothDialog::~BluetoothDialog()
{
    delete ui;
}






void BluetoothDialog::addDevice(const QBluetoothDeviceInfo &info)
{
    bluetoothDevices.insert(info.name() ,info.address().toString());


   // QString label = QString("%2 %1").arg(info.address().toString()).arg(info.name()); //Hvis vi bare ønsker navnet på modulen
    QString label = QString("%1").arg(info.name());
    QList<QListWidgetItem *> items = ui->listViewBluetoothDevices->findItems(label, Qt::MatchExactly);
    if (items.empty()) {
        QListWidgetItem *item = new QListWidgetItem(label);
         ui->listViewBluetoothDevices->addItem(item);

    }


}

void BluetoothDialog::scanFinished()
{
    discoveryAgent->stop();
}

void BluetoothDialog::stopScanning()
{
    discoveryAgent->stop();
    setBluetoothAddress(bluetoothDevices[ui->listViewBluetoothDevices->currentItem()->text()]);
}

void BluetoothDialog::on_pushButtonScan_clicked()
{
    discoveryAgent->start();
}

void BluetoothDialog::on_pushButtonOk_clicked()
{
    BluetoothAddress.remove(":", Qt::CaseInsensitive);


    if(serialPort->UnconnectedState)
    {

        QMessageBox::critical(qobject_cast<QWidget *>(parent()), "Error", "The Bluetooth are not responding!\nTry again");

    }
    else
    {
        try
        {

        serialPort->connectToService(QBluetoothAddress(BluetoothAddress),QBluetoothUuid(QBluetoothUuid::SerialPort));
        }
        catch(...)
        {
             QMessageBox::critical(qobject_cast<QWidget *>(parent()), "Error", "The Bluetooth are not responding!\nTry again");
             return;
        }


    }

    this->hide();

}

void BluetoothDialog::ReadFrame()
{
    static FrameState state;
    do
    {
        char data;
        serialPort->read(&data,1);
        switch(state)
        {
        case Start:
            state = CaseStart(data);
            break;
        case Direction:
            state = CaseDirection(data);
            break;
        case Command:
            state = CaseCommand(data);
            break;
        case Length:
            state = CaseLength(data);
            break;
        case Payload:
            state = CasePayload(data);
            if(state == Payload)
            { break; }
        case CRC:
            state = CaseCRC(data);
            if( state == CRC)
            { break; }
        case End:
            state = CheckCRC();
            break;
        }
    }
    while(serialPort->bytesAvailable());



}




void BluetoothDialog::CreateFrameAndSend(char &command, char &direction, QByteArray &payload)
{
    QByteArray sendArray;
    sendArray.append(StartByte);
    sendArray.append(direction);
    sendArray.append(command);
    sendArray.append(payload.size());
    sendArray.append(payload);

    checksum->ClearCRC();
    for(short i = 0; i< sendArray.size(); i++)
    {
        checksum->UpdateCRC(sendArray.at(i));
    }
    quint16 crc = checksum->GetCRC();

    sendArray.append(crc & 0xFF);
    sendArray.append(crc >> 8);

    serialPort->write(sendArray);
}
QString BluetoothDialog::getBluetoothAddress() const
{
    return BluetoothAddress;
}

void BluetoothDialog::setBluetoothAddress(const QString &value)
{
    BluetoothAddress = value;
}


BluetoothDialog::FrameState BluetoothDialog::CaseStart(quint8 data)
{
    if(data == StartByte)
    {
        incomingFrame.start = data;
        checksum->ClearCRC();
        checksum->UpdateCRC(data);
        return Direction;
    }
    return Start;

}

BluetoothDialog::FrameState BluetoothDialog::CaseDirection(quint8 data)
{
    incomingFrame.direction = data;
    checksum->UpdateCRC(data);
    return Command;
}

BluetoothDialog::FrameState BluetoothDialog::CaseCommand(quint8 data)
{
    incomingFrame.command = data;
    checksum->UpdateCRC(data);
    return Length;
}

BluetoothDialog::FrameState BluetoothDialog::CaseLength(quint8 data)
{
    incomingFrame.length = data;
    incomingFrame.payload.clear();
    checksum->UpdateCRC(data);
    return Payload;
}

BluetoothDialog::FrameState BluetoothDialog::CasePayload(quint8 data)
{
    if(incomingFrame.payload.size() == incomingFrame.length)
    {
        return CRC;
    }
    else
    {
        incomingFrame.payload.append(data);
        checksum->UpdateCRC(data);
    }
    return Payload;
}

BluetoothDialog::FrameState BluetoothDialog::CaseCRC(quint8 data)
{
    static int counter = 0;

    if(counter == 0)
    {
        incomingFrame.crc = (quint8)data;
        counter++;
        return CRC;
    }

    else if(counter == 1)
    {
        incomingFrame.crc = incomingFrame.crc | ( (quint8) data << 8);
        counter = 0;
    }
    return End;
}

BluetoothDialog::FrameState BluetoothDialog::CheckCRC()
{
    if(incomingFrame.crc == checksum->GetCRC())
    {
        emit FrameComplete(incomingFrame.command,incomingFrame.direction, incomingFrame.payload);
    }
    return Start;
}



