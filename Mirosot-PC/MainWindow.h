/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

class MainWindow : public QMainWindow, private Ui::MainWindow
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
    CameraConnectDialog *cameraConnectDialog;
    ProcessingSettingsDialog *processingSettingsDialog;
    SettingsDialog *settingsDialog;
    Controller *controller;
    ProcessingFlags processingFlags;
    TaskData taskData;
    QString appVersion;
    int sourceWidth;
    int sourceHeight;
    int deviceNumber;

    int imageBufferSize;
    bool isCameraConnected;
    bool isPortConnected;
public slots:
    void connectToCamera();
    void disconnectCamera();
    void connectToSerial();
    void disconnectSerial();
    void about();
    void clearImageBuffer();
    void setBS(bool);
    void setColor(bool);
    void setProcessingSettings();
    void updateMouseCursorPosLabel();
    void newMouseData(struct MouseData);
private slots:
    void updateFrame(const QImage &frame);
    void updateData(QString data);
signals:
    void newProcessingFlags(struct ProcessingFlags p_flags);
    void newTaskData(struct TaskData taskData);
};

#endif // MAINWINDOW_H
