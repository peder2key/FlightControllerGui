#include "mainwindow.h"
#include "ui_mainwindow.h"

imu_t imu;
att_t att;
p_t p;
rc_t rc;
motor_t motor;
settings_t settings;
status_t status;
loopTime_t loopTime;
debug debug123;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mTelegram = new Telegram(this);
    mLoginDialog = new loginDialog(this);
    mRequestTimer = new QTimer(this);

    //Info regarding which serial line is connected
  //  connect(mTelegram->serialProtocol->mSetupSerial, SIGNAL(portBaudSetting(QString,qint32)), this, SLOT(connectedTo(QString,qint32)));
    connect(mTelegram, SIGNAL(ReceivedPid()), this, SLOT(updatePid()));
    connect(mRequestTimer, SIGNAL(timeout()), this, SLOT(realTImeRequestData()));
    connect(mTelegram, SIGNAL(UpdateStructs()),this, SLOT(updateStructs()));

    plot = new PlotSetup(this);
    plot->Setup(ui->customPlot);
    plot->SetYAxis(-60,60);




    UpdateDataMap();
    QStringList graphNames = dataMap.keys(); // Get names from the map
    plot->SetGraphTextList(graphNames); // Copy names to plot class, used as text in grap boxes


    // Link GUI comboboxes to plot class
    plot->CreateGraph(ui->graphBox1,ui->colorBox1);
    plot->CreateGraph(ui->graphBox2,ui->colorBox2);
    plot->CreateGraph(ui->graphBox3,ui->colorBox3);
    plot->CreateGraph(ui->graphBox4,ui->colorBox4);
    plot->CreateGraph(ui->graphBox5,ui->colorBox5);
    plot->CreateGraph(ui->graphBox6,ui->colorBox6);
    plot->CreateGraph(ui->graphBox7,ui->colorBox7);
    plot->CreateGraph(ui->graphBox8,ui->colorBox8);
    plot->CreateGraph(ui->graphBox9,ui->colorBox9);



}



MainWindow::~MainWindow()
{
    delete ui;
}
// /////////////////////////////////////////
// MainWindow Buttons config
// ///////////////////////////////////////////
void MainWindow::on_pushButtonSetup_clicked()
{
    //mTelegram->serialProtocol->showSerialSetup();
}

void MainWindow::connectedTo(QString port, qint32 baud)
{
    ui->statusBar->setStyleSheet("QStatusBar{background:rgba(0, 255, 0, 255)}"); //Setter statusbaren grønn når den er connected
    ui->statusBar->showMessage(tr("Connected! To port: %1 and baudrate is: %2").arg(port).arg(baud));
}

void MainWindow::on_pushButtonGraphSensors_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    SerialPlot();
}

void MainWindow::on_pushButtonPID_clicked()
{
    mRequestTimer->stop();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButtonSpare_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


void MainWindow::on_getPidButton_clicked()
{
    mTelegram->Request(PID_CMD, ReadDirection);
}

void MainWindow::on_writePidButton_clicked()
{
    p.rollPID.kP = ui->rollPidkP->text().toFloat();
    p.rollPID.kI = ui->rollPidkI->text().toFloat();
    p.rollPID.kD = ui->rollPidkD->text().toFloat();

    p.pitchPID.kP = ui->pitchPidkP->text().toFloat();
    p.pitchPID.kI = ui->pitchPidkI->text().toFloat();
    p.pitchPID.kD = ui->pitchPidkD->text().toFloat();

    p.yawPID.kP = ui->yawPidkP->text().toFloat();
    p.yawPID.kI = ui->yawPidkI->text().toFloat();
    p.yawPID.kD = ui->yawPidkD->text().toFloat();
    p.iMax = ui->PidiMax->text().toUShort();

    mTelegram->Request(PID_CMD, WriteDirection);
}

void MainWindow::on_pushButtonRc_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_pushButtonMotor_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButtonSystemOverview_clicked()
{
     ui->stackedWidget->setCurrentIndex(4);
    /*if(mTelegram->serialProtocol->serialPort->isOpen())
    {
        mTelegram->Request(EEPROM_CMD, ReadDirection);
        mRequestTimer->start(100);
    }
    else
    {
        return;
    }
*/
}



// //////////////////////////////////////////////
//  Mainwindow Buttons END
// //////////////////////////////////////////////

// //////////////////////////////////////////////
//  Update Variabels and GUI
// //////////////////////////////////////////////

void MainWindow::updateStructs(void)
{
    ui->accRollLine->setText(QString::number(imu.accADC[ROLL]));
    ui->accPitchLine->setText(QString::number(imu.accADC[PITCH]));
    ui->accYawLine->setText(QString::number(imu.accADC[YAW]));

    ui->gyroRollLine->setText(QString::number(imu.gyroADC[ROLL]));
    ui->gyroPitchLine->setText(QString::number(imu.gyroADC[PITCH]));
    ui->gyroYawLine->setText(QString::number(imu.gyroADC[YAW]));

    ui->magRollLine->setText(QString::number(imu.magADC[ROLL]));
    ui->magPitchLine->setText(QString::number(imu.magADC[PITCH]));
    ui->magYawLine->setText(QString::number(imu.magADC[YAW]));

    //ATT
    float rollAngle = (att.angle[ROLL] / 10);
    float pitchAngle = (att.angle[PITCH] / 10);
    ui->attRollLine->setText(QString::number(rollAngle));
    ui->attPitchLine->setText(QString::number(pitchAngle));
    ui->attHeadingLine->setText(QString::number(att.heading));

    //Motor
    ui->frontMotor->setText(QString::number(motor.front));
    ui->backMotor->setText(QString::number(motor.back));
    ui->leftMotor->setText(QString::number(motor.left));
    ui->rightMotor->setText(QString::number(motor.right));

    //RC
    ui->rcRoll->setText(QString::number(rc.roll));
    ui->rcPitch->setText(QString::number(rc.pitch));
    ui->rcYaw->setText(QString::number(rc.yaw));
    ui->rcThrottle->setText(QString::number(rc.throttle));
    ui->rcAux1->setText(QString::number(rc.aux1));
    ui->rcAux2->setText(QString::number(rc.aux2));
    ui->rcAux3->setText(QString::number(rc.aux3));
    ui->rcAux4->setText(QString::number(rc.aux4));

    //EEPROM
    ui->accCaliRoll->setText(QString::number(settings.accOffset[ROLL]));
    ui->accCaliPitch->setText(QString::number(settings.accOffset[PITCH]));
    ui->accCaliYaw->setText(QString::number(settings.accOffset[YAW]));

    ui->magCaliRoll->setText(QString::number(settings.magOffset[ROLL]));
    ui->magCaliPitch->setText(QString::number(settings.magOffset[PITCH]));
    ui->magCaliYaw->setText(QString::number(settings.magOffset[YAW]));

    //STATUS
    ui->loopTime->setText(QString::number(loopTime.time));

    UpdateDataMap();

}

void MainWindow::updatePid()
{


    ui->rollPidkP->setText(QString::number(p.rollPID.kP));
    ui->rollPidkI->setText(QString::number(p.rollPID.kI));
    ui->rollPidkD->setText(QString::number(p.rollPID.kD));

    ui->pitchPidkP->setText(QString::number(p.pitchPID.kP));
    ui->pitchPidkI->setText(QString::number(p.pitchPID.kI));
    ui->pitchPidkD->setText(QString::number(p.pitchPID.kD));

    ui->yawPidkP->setText(QString::number(p.yawPID.kP));
    ui->yawPidkI->setText(QString::number(p.yawPID.kI));
    ui->yawPidkD->setText(QString::number(p.yawPID.kD));

    ui->PidiMax->setText(QString::number(p.iMax));

}



// //////////////////////////////////////////////
//  Update Variabels and GUI END
// //////////////////////////////////////////////


void MainWindow::serviceLogin(bool accepted)
{


}

void MainWindow::realTImeRequestData()
{

    static short state = 0;
    switch(state)
    {
    case 0:
        mTelegram->Request(IMU_CMD,ReadDirection);
        state++;
        break;
    case 1:
        mTelegram->Request(ATT_CMD,ReadDirection);
        state++;
        break;
    case 2:
        mTelegram->Request(MOTOR_CMD,ReadDirection);

        state++;
        break;
    case 3:
        mTelegram->Request(RC_CMD, ReadDirection);
        state++;
        break;
    case 4:
        mTelegram->Request(STATUS_CMD,ReadDirection);
        mTelegram->Request(LOOPT_CMD,ReadDirection);
        state = 5;
        break;
    case 5:
        mTelegram->Request(DEBUG_CMD, ReadDirection);
        state = 0;
        break;

    }

}

void MainWindow::SerialPlot(void)
{
    plot->clearGraph();
    plot->SetYAxis(40,40);
    plot->BeginPlot();
    mRequestTimer->start(1);

}

void MainWindow::UpdateDataMap(void)
{
    float scaledMotorFront = map(motor.front,1000,2000,0,100);
    float scaledMotorLeft = map(motor.left,1000,2000,0,100);
    float scaledMotorBack = map(motor.back,1000,2000,0,100);
    float scaledMotorRight = map(motor.right,1000,2000,0, 100);

    float scaledRcThrottle = map(rc.throttle, 900, 2100, 0, 100);
    float scaledRcPitch = map(rc.pitch, 900, 2100, 0, 100);
    float scaledRcRoll = map(rc.roll, 900, 2100, 0, 100);
    float scaledRcYaw = map(rc.yaw, 900, 2100, 0, 100);

    dataMap.insert("",0); // Blank for no plot selected
    dataMap.insert("Angle Roll",att.angle[ROLL]);
    dataMap.insert("Angle Pitch",att.angle[PITCH]);
    dataMap.insert("Acc Roll",imu.accADC[ROLL]);
    dataMap.insert("Acc Pitch",imu.accADC[PITCH]);
    dataMap.insert("Acc Yaw",imu.accADC[YAW]);
    dataMap.insert("Gyro Roll",imu.gyroADC[ROLL]);
    dataMap.insert("Gyro Pitch",imu.gyroADC[PITCH]);
    dataMap.insert("Gyro Yaw",imu.gyroADC[YAW]);
    dataMap.insert("Motor Front", scaledMotorFront);
    dataMap.insert("Motor Left", scaledMotorLeft);
    dataMap.insert("Motor Back", scaledMotorBack);
    dataMap.insert("Motor Right", scaledMotorRight);
    dataMap.insert("RC Throttle", scaledRcThrottle);
    dataMap.insert("RC Pitch", scaledRcPitch);
    dataMap.insert("RC Roll", scaledRcRoll);
    dataMap.insert("RC Yaw", scaledRcYaw);
    dataMap.insert("Debug 1", debug123.debug1);
    dataMap.insert("Debug 2", debug123.debug2);
    dataMap.insert("Debug 3", debug123.debug3);
    plot->UpdatePlot(dataMap);

}








void MainWindow::on_pushButtonAccCalibrate_clicked()
{
    mRequestTimer->stop();
    status.accDoCalibrate = 1;
    mTelegram->Request(STATUS_CMD,WriteDirection);
}

void MainWindow::on_pushButtonMagCalibrate_clicked()
{
    mRequestTimer->stop();
    status.magDoCalibrate = 1;
    mTelegram->Request(STATUS_CMD,WriteDirection);
}

long MainWindow::map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



void MainWindow::on_pushButtonGetDebug_clicked()
{
    mTelegram->Request(DEBUG_CMD, ReadDirection);
}

void MainWindow::on_actionBluetooth_triggered()
{
    mTelegram->showBluetoothDialog();
}
