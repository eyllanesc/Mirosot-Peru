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

// Qt header files
#include <QDebug>
#include <QPushButton>

#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)

class CameraConnectDialog;
class ProcessingSettingsDialog;
class Controller;
class SettingsDialog;

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
    void connectToPlay();
private:
     Ui::MainWindow *ui;
    CameraConnectDialog *cameraConnectDialog;
    ProcessingSettingsDialog *processingSettingsDialog;
    SettingsDialog *settingsDialog;
    Controller *controller;
    ProcessingFlags processingFlags;
    TaskData taskData;
    PosData posData;
    QString appVersion;
    int sourceWidth;
    int sourceHeight;
    QPushButton *playCtl;
    int deviceNumber;
    int imageBufferSize;
    Settings settings;
    bool isCameraConnected;
    bool isPortConnected;
    bool isPlay;
    bool isCamera;
    bool isSerial;

    bool rgbOn;
    bool hsvOn;
    bool ycrcbOn;

    bool teamOn;
    bool robot1On;
    bool robot2On;
    bool ballOn;

    void setColorModel(int);
    void setObject(int);

public slots:
    void connectToStart();
    void connectToCamera();
    void connectToSerial();
    void about();
    void clearImageBuffer();
    void setBS(bool);
    void setProcessingSettings();
    void updateMouseCursorPosLabel();
    void newMouseData(struct MouseData);
private slots:
    void updateFrame(const QImage &frame);
    void updateData(const QString data);
    void readData();
    void play();

    void setTeam(bool);
    void setRobot1(bool);
    void setRobot2(bool);
    void setBall(bool);

    void setColorRGB(bool);
    void setColorHSV(bool);
    void setColorYCrCb(bool);
signals:
    void newProcessingFlags(struct ProcessingFlags p_flags);
    void newTaskData(struct TaskData taskData);
    void newPosData(struct PosData posData);
};

#endif // MAINWINDOW_H
