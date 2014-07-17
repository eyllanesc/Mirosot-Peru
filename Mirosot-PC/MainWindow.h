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

    int deviceNumber;
    int imageBufferSize;
    Settings settings;
    bool isCameraConnected;
    bool isPortConnected;

    bool isCamera;
    bool isSerial;
public slots:
    void connectToStart();
    void connectToCamera();
    void connectToSerial();
    void about();
    void clearImageBuffer();
    void setBS(bool);
    void setColor(bool);
    void setProcessingSettings();
    void updateMouseCursorPosLabel();
    void newMouseData(struct MouseData);
private slots:
    void updateFrame(const QImage &frame);
    void updateData(const QString data);
    void readData();
signals:
    void newProcessingFlags(struct ProcessingFlags p_flags);
    void newTaskData(struct TaskData taskData);
    void newPosData(struct PosData posData);
};

#endif // MAINWINDOW_H
