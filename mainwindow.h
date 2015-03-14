#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CommandLibrary.h"
#include "telegram.h"
#include "setupserial.h"
#include "telegram.h"
#include "logindialog.h"
#include "plotsetup.h"


#include <QMainWindow>
#include <QTimer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_pushButtonSetup_clicked();
    void on_pushButtonGraphSensors_clicked();
    void on_pushButtonPID_clicked();
    void on_getPidButton_clicked();
    void on_writePidButton_clicked();
    void on_pushButtonRc_clicked();
    void on_pushButtonMotor_clicked();
    void on_pushButtonSystemOverview_clicked();
    void realTImeRequestData();
    void SerialPlot(void);


    void on_pushButtonAccCalibrate_clicked();

    void on_pushButtonMagCalibrate_clicked();

    void on_pushButtonSpare_clicked();

    void on_pushButtonGetDebug_clicked();

    void on_actionBluetooth_triggered();

public slots:
    void connectedTo(QString port, qint32 baud);
    void serviceLogin(bool);
    void updateStructs(void);
    void updatePid(void);

private:
    Ui::MainWindow *ui;
    Telegram *mTelegram;
    loginDialog *mLoginDialog;
    QTimer *mRequestTimer;
    PlotSetup *plot;
    QMap<QString,double> dataMap;
    void UpdateDataMap(void);
    long map(long x, long in_min, long in_max, long out_min, long out_max);

};

#endif // MAINWINDOW_H
