/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

namespace Ui {
class MainWindow;
}

#include "ui_MainWindow.h"

//My header
#include "CameraConnectDialog.h"
#include "ProcessingSettingsDialog.h"
#include "settingsdialog.h"
#include "Controller.h"
#include "Config.h"
#include "histogram.h"
// Qt header files
#include <QDebug>
#include <QPushButton>

#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)

class CameraConnectDialog;
class ProcessingSettingsDialog;
class Controller;
class SettingsDialog;
class Histogram;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initializeProcessingFlagsStructure();
    void initializeTaskDataStructure();
    void setInitGUIState();
    void initializeGUI();
    void signalSlotsInit();
private:
    Ui::MainWindow *ui;
    CameraConnectDialog *cameraConnectDialog;
    ProcessingSettingsDialog *processingSettingsDialog;
    SettingsDialog *settingsDialog;
    Controller *controller;
    Histogram *histogram;

    QMessageBox *qmessabox;
    int cont;

    ProcessingFlags processingFlags;
    TaskData taskData;
    PosData posData;
    QString appVersion;
    int sourceWidth;
    int sourceHeight;
    //QPushButton *playCtl;
    int deviceNumber;
    int imageBufferSize;
    Settings settings;
    bool isCameraConnected;
    bool isPortConnected;
    bool isPlay;
    bool isCamera;
    bool isSerial;

    bool campOn;
    bool teamOn;
    bool robot1On;
    bool robot2On;
    bool ballOn;

    void setColorModel(int);
    void setObject(int);

public slots:
    void play();
    void connectToStart();
    void connectToCamera();
    void connectToSerial();
    void about();
    void clearImageBuffer();
    void setProcessingSettings();
    void updateMouseCursorPosLabel();
    void newMouseData(struct MouseData);
    void setHistogram(const QImage &hist);
    void asinwrite();

private slots:
    void updateFrame(const QImage &frame);
    void updateFrame2(const QImage &frame);
    void updateData(const QString data);
    void updateCalibrar(const QString state);
    void readData();

    void setBS(bool);
    void setCamp();
    void setTeam();
    void setRobot1();
    void setRobot2();
    void setBall();

signals:
    void newProcessingFlags(struct ProcessingFlags p_flags);
    void newTaskData(struct TaskData taskData);
    void newPosData(struct PosData posData);
};

#endif // MAINWINDOW_H
