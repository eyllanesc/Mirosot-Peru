/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/

#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Setup user interface
    setupUi(this);
    // Create controller
    controller = new Controller;
    // Save application version in QString variable
    appVersion=QUOTE(APP_VERSION);
    // Create processingSettingsDialog
    processingSettingsDialog = new ProcessingSettingsDialog(this);
    // Initialize data structures
    initializeProcessingFlagsStructure();
    initializeTaskDataStructure();
    // Enable/disable appropriate GUI items
    setInitGUIState();
    // Initialize GUI
    initializeGUI();
    // Connect signals to slots
    signalSlotsInit();
    // Initialize flag
    isCameraConnected=false;
    //this->setStyleSheet("background-image: url(/home/edwin/Pictures/rasuni.png)");

} // MainWindow constructor

MainWindow::~MainWindow()
{
    // Check if camera is connected
    if(isCameraConnected)
    {
        // Disconnect connections (4)
        disconnect(controller->processingThread,SIGNAL(newFrame(QImage)),0,0);
        disconnect(this,SIGNAL(newProcessingFlags(struct ProcessingFlags)),controller->processingThread,SLOT(updateProcessingFlags(struct ProcessingFlags)));
        disconnect(this->processingSettingsDialog,SIGNAL(newProcessingSettings(struct ProcessingSettings)),controller->processingThread,SLOT(updateProcessingSettings(struct ProcessingSettings)));
        disconnect(this,SIGNAL(newTaskData(struct TaskData)),controller->processingThread,SLOT(updateTaskData(struct TaskData)));
        // Stop processing thread
        if(controller->processingThread->isRunning())
            controller->stopProcessingThread();
        // Stop capture thread
        if(controller->captureThread->isRunning())
            controller->stopCaptureThread();
        // Clear image buffer
        controller->clearImageBuffer();
        // Check if threads have stopped
        if((controller->captureThread->isFinished())&&(controller->processingThread->isFinished()))
        {
            // Disconnect camera
            controller->disconnectCamera();
            // Delete threads
            controller->deleteCaptureThread();
            controller->deleteProcessingThread();
            // Delete image buffer
            controller->deleteImageBuffer();
        }
    }
} // MainWindow destructor

void MainWindow::connectToSerial()
{
    settingsDialog = new SettingsDialog(this);
    //settingsDialog->setStyleSheet("background-image: url(/home/edwin/Pictures/myTeacher.jpg)");
    if(settingsDialog->exec()==1)
    {
        isPortConnected=controller->sendThread->connectToserial(settingsDialog->settings());
        if(isPortConnected)
        {
            connect(controller->processingThread,SIGNAL(newData(QString)),this,SLOT(updateData(QString)),Qt::UniqueConnection);
            //controller->sendThread->start(QThread::LowestPriority);
            connectToSerialAction->setDisabled(true);
            disconnectSerialAction->setDisabled(false);
        }
        else
        {
            QMessageBox::warning(this,"ERROR:","Could not connect to port.");
        }
    }
}
void MainWindow::updateData(QString data)
{
    controller->sendThread->receive(data);
    datalabel->setText(data);
}
void MainWindow::connectToCamera()
{
    // Create dialog
    cameraConnectDialog = new CameraConnectDialog(this);
    // PROMPT USER:
    // If user presses OK button on dialog, connect to camera; else do nothing
    if(cameraConnectDialog->exec()==1)
    {
        // Set private member variables in cameraConnectDialog to values in dialog
        cameraConnectDialog->setDeviceNumber();
        cameraConnectDialog->setImageBufferSize();
        // Store image buffer size in local variable
        imageBufferSize=cameraConnectDialog->getImageBufferSize();
        // Store device number in local variable
        deviceNumber=cameraConnectDialog->getDeviceNumber();
        // Connect to camera
        if((isCameraConnected=controller->connectToCamera(deviceNumber,imageBufferSize)))
        {
            // Create connection between processing thread (emitter) and GUI thread (receiver/listener)
            connect(controller->processingThread,SIGNAL(newFrame(QImage)),this,SLOT(updateFrame(QImage)),Qt::UniqueConnection);
            // Create connections (3) between GUI thread (emitter) and processing thread (receiver/listener)
            qRegisterMetaType<struct ProcessingFlags>("ProcessingFlags");
            connect(this,SIGNAL(newProcessingFlags(struct ProcessingFlags)),controller->processingThread,SLOT(updateProcessingFlags(struct ProcessingFlags)),Qt::UniqueConnection);
            qRegisterMetaType<struct ProcessingSettings>("ProcessingSettings");
            connect(this->processingSettingsDialog,SIGNAL(newProcessingSettings(struct ProcessingSettings)),controller->processingThread,SLOT(updateProcessingSettings(struct ProcessingSettings)),Qt::UniqueConnection);
            qRegisterMetaType<struct TaskData>("TaskData");
            connect(this,SIGNAL(newTaskData(struct TaskData)),controller->processingThread,SLOT(updateTaskData(struct TaskData)),Qt::UniqueConnection);
            // Set data to defaults in processingThread
            emit newProcessingFlags(processingFlags);
            emit newTaskData(taskData);
            processingSettingsDialog->resetAllDialogToDefaults();
            processingSettingsDialog->updateStoredSettingsFromDialog();
            // Setup imageBufferBar in main window with minimum and maximum values
            imageBufferBar->setMinimum(0);
            imageBufferBar->setMaximum(imageBufferSize);
            // Enable/disable appropriate menu items
            connectToCameraAction->setDisabled(true);
            disconnectCameraAction->setDisabled(false);
            processingMenu->setDisabled(false);
            menuSerial->setDisabled(false);
            // Enable "Clear Image Buffer" push button in main window
            clearImageBufferButton->setDisabled(false);
            // Get input stream properties
            sourceWidth=controller->captureThread->getInputSourceWidth();
            sourceHeight=controller->captureThread->getInputSourceHeight();
            // Set text in labels in main window
            deviceNumberLabel->setNum(deviceNumber);
            cameraResolutionLabel->setText(QString::number(sourceWidth)+QString("x")+QString::number(sourceHeight));
            /*
            QThread::IdlePriority               0	scheduled only when no other threads are running.
            QThread::LowestPriority             1	scheduled less often than LowPriority.
            QThread::LowPriority                2	scheduled less often than NormalPriority.
            QThread::NormalPriority             3	the default priority of the operating system.
            QThread::HighPriority               4	scheduled more often than NormalPriority.
            QThread::HighestPriority            5	scheduled more often than HighPriority.
            QThread::TimeCriticalPriority	6	scheduled as often as possible.
            QThread::InheritPriority            7	use the same priority as the creating thread. This is the default.
            */
            // Start capturing frames from camera
            controller->captureThread->start(QThread::IdlePriority);
            // Start processing captured frames
            controller->processingThread->start();
        }
        // Display error dialog if camera connection is unsuccessful
        else
            QMessageBox::warning(this,"ERROR:","Could not connect to camera.");
    }
} // connectToCamera()

void MainWindow::disconnectSerial()
{

    disconnect(controller->processingThread,SIGNAL(newData(QString)),0,0);
    //disconnect(controller->processingThread,SIGNAL(newData(QString)),0,0);
    controller->sendThread->disconnectSerial();
    connectToSerialAction->setEnabled(true);
    disconnectSerialAction->setEnabled(false);
    datalabel->setText(" ");
}
void MainWindow::disconnectCamera()
{
    if(isPortConnected)
        disconnectSerial();
    // Check if camera is connected
    if(controller->captureThread->isCameraConnected())
    {
        // Disconnect connections (4)
        disconnect(controller->processingThread,SIGNAL(newFrame(QImage)),0,0);
        disconnect(this,SIGNAL(newProcessingFlags(struct ProcessingFlags)),controller->processingThread,SLOT(updateProcessingFlags(struct ProcessingFlags)));
        disconnect(this->processingSettingsDialog,SIGNAL(newProcessingSettings(struct ProcessingSettings)),controller->processingThread,SLOT(updateProcessingSettings(struct ProcessingSettings)));
        disconnect(this,SIGNAL(newTaskData(struct TaskData)),controller->processingThread,SLOT(updateTaskData(struct TaskData)));
        // Stop processing thread
        if(controller->processingThread->isRunning())
            controller->stopProcessingThread();
        // Stop capture thread
        if(controller->captureThread->isRunning())
            controller->stopCaptureThread();
        // Clear image buffer
        controller->clearImageBuffer();
        // Check if threads have stopped
        if((controller->captureThread->isFinished())&&(controller->processingThread->isFinished()))
        {
            // Disconnect camera
            controller->disconnectCamera();
            // Reset flag
            isCameraConnected=false;
            // Delete threads
            controller->deleteCaptureThread();
            controller->deleteProcessingThread();
            // Delete image buffer
            controller->deleteImageBuffer();
            // Enable/Disable appropriate menu items
            connectToCameraAction->setDisabled(false);
            disconnectCameraAction->setDisabled(true);
            processingMenu->setDisabled(true);
            menuSerial->setDisabled(true);
            // Set GUI in main window
            BSAction->setChecked(false);
            ColorAction->setChecked(false);
            frameLabel->setText("No camera connected.");
            imageBufferBar->setValue(0);
            imageBufferLabel->setText("[000/000]");
            captureRateLabel->setText("");
            processingRateLabel->setText("");
            deviceNumberLabel->setText("");
            cameraResolutionLabel->setText("");
            roiLabel->setText("");
            mouseCursorPosLabel->setText("");
            clearImageBufferButton->setDisabled(true);
        }
        // Display error dialog
        else
            QMessageBox::warning(this,"ERROR:","Thread(s) could not be stopped. Camera was not disconnected.");
    }
    // Display error dialog
    else
        QMessageBox::warning(this,"ERROR:","Camera already disconnected.");
} // disconnectCamera()

void MainWindow::about()
{
    QMessageBox::information(this,"About",QString("Created by Edwin Christian Yllanes cucho\n\nContact: <e.yllanescucho@ieee.org>\nWebsite: RAS-IEEE UNI\n\nVersion: ")+appVersion);
} // about()

void MainWindow::clearImageBuffer()
{
    controller->clearImageBuffer();
} // clearImageBuffer()


void MainWindow::setBS(bool input)
{
    // Not checked
    if(!input)
        processingFlags.bsOn=false;
    // Checked
    else if(input)
        processingFlags.bsOn=true;
    // Update processing flags in processingThread
    emit newProcessingFlags(processingFlags);
}// setBS()

void MainWindow::setColor(bool input)
{
    // Not checked
    if(!input)
        processingFlags.colorOn =false;
    // Checked
    else if(input)
        processingFlags.colorOn =true;
    // Update processing flags in processingThread
    emit newProcessingFlags(processingFlags);
}// setColor()

void MainWindow::updateFrame(const QImage &frame)
{
    // Show [number of images in buffer / image buffer size] in imageBufferLabel in main window
    imageBufferLabel->setText(QString("[")+QString::number(controller->processingThread->getCurrentSizeOfBuffer())+
                              QString("/")+QString::number(imageBufferSize)+QString("]"));
    // Show percentage of image bufffer full in imageBufferBar in main window
    imageBufferBar->setValue(controller->processingThread->getCurrentSizeOfBuffer());
    // Show processing rate in captureRateLabel in main window
    captureRateLabel->setNum(controller->captureThread->getAvgFPS());
    captureRateLabel->setText(captureRateLabel->text()+" fps");
    // Show processing rate in processingRateLabel in main window
    processingRateLabel->setNum(controller->processingThread->getAvgFPS());
    processingRateLabel->setText(processingRateLabel->text()+" fps");
    // Show ROI information in roiLabel in main window
    roiLabel->setText(QString("(")+QString::number(controller->processingThread->getCurrentROI().x)+QString(",")+
                      QString::number(controller->processingThread->getCurrentROI().y)+QString(") ")+
                      QString::number(controller->processingThread->getCurrentROI().width)+
                      QString("x")+QString::number(controller->processingThread->getCurrentROI().height));
    // Display frame in main window
    frameLabel->setPixmap(QPixmap::fromImage(frame));
} // updateFrame()

void MainWindow::setProcessingSettings()
{
    // Prompt user:
    // If user presses OK button on dialog, update processing settings
    if(processingSettingsDialog->exec()==1)
        processingSettingsDialog->updateStoredSettingsFromDialog();
    // Else, restore dialog state
    else
        processingSettingsDialog->updateDialogSettingsFromStored();
} // setProcessingSettings()

void MainWindow::updateMouseCursorPosLabel()
{
    // Update mouse cursor position in mouseCursorPosLabel in main window
    mouseCursorPosLabel->setText(QString("(")+QString::number(frameLabel->getMouseCursorPos().x())+
                                 QString(",")+QString::number(frameLabel->getMouseCursorPos().y())+
                                 QString(")"));
} // updateMouseCursorPosLabel()

void MainWindow::newMouseData(struct MouseData mouseData)
{
    // Local variables
    //int x_temp, y_temp, width_temp, height_temp;
    // Set ROI
    if(mouseData.leftButtonRelease)
    {
        // Copy box dimensions from mouseData to taskData
        taskData.selectionBox.setX(mouseData.selectionBox.x());
        taskData.selectionBox.setY(mouseData.selectionBox.y());
        taskData.selectionBox.setWidth(mouseData.selectionBox.width());
        taskData.selectionBox.setHeight(mouseData.selectionBox.height());
        // Check if selection box has NON-ZERO dimensions
        if((taskData.selectionBox.width()!=0)&&((taskData.selectionBox.height())!=0))
        {
            // Selection box can also be drawn from bottom-right to top-left corner
            /*if(taskData.selectionBox.width()<0)
            {
                x_temp=taskData.selectionBox.x();
                width_temp=taskData.selectionBox.width();
                taskData.selectionBox.setX(x_temp+taskData.selectionBox.width());
                taskData.selectionBox.setWidth(width_temp*-1);
            }
            if(taskData.selectionBox.height()<0)
            {
                y_temp=taskData.selectionBox.y();
                height_temp=taskData.selectionBox.height();
                taskData.selectionBox.setY(y_temp+taskData.selectionBox.height());
                taskData.selectionBox.setHeight(height_temp*-1);
            }*/
            // Check if selection box is not outside window
            if((taskData.selectionBox.x()<0)||(taskData.selectionBox.y()<0)||
                    ((taskData.selectionBox.x()+taskData.selectionBox.width())>sourceWidth)||
                    ((taskData.selectionBox.y()+taskData.selectionBox.height())>sourceHeight))
            {
                // Display error message
                QMessageBox::warning(this,"ERROR:","Selection box outside range. Please try again.");
            }
            else
            {
                // Set setROIFlag to TRUE
                taskData.setROIFlag=true;
                // Update task data in processingThread
                emit newTaskData(taskData);
                // Set setROIFlag to FALSE
                taskData.setROIFlag=false;
            }
        }
    }
    // Reset ROI
    else if(mouseData.rightButtonRelease)
    {
        // Set resetROIFlag to TRUE
        taskData.resetROIFlag=true;
        // Update task data in processingThread
        emit newTaskData(taskData);
        // Set resetROIFlag to FALSE
        taskData.resetROIFlag=false;
    }
} // newMouseData()

void MainWindow::initializeProcessingFlagsStructure()
{
    processingFlags.bsOn=false;
    processingFlags.colorOn=false;
} // initializeProcessingFlagsStructure()

void MainWindow::initializeTaskDataStructure()
{
    taskData.setROIFlag=false;
    taskData.resetROIFlag=false;
} // initializeTaskDataStructure()

void MainWindow::initializeGUI()
{
    // Enable/disable appropriate menu items
    connectToCameraAction->setDisabled(false);
    disconnectCameraAction->setDisabled(true);
    connectToSerialAction->setDisabled(false);
    disconnectSerialAction->setDisabled(true);
    processingMenu->setDisabled(true);
    menuSerial->setDisabled(true);
} // initializeGUI()

void MainWindow::setInitGUIState()
{
    // Set GUI in main window
    BSAction->setChecked(false);
    ColorAction->setChecked(false);
    frameLabel->setText("No camera connected.");
    imageBufferBar->setValue(0);
    imageBufferLabel->setText("[000/000]");
    captureRateLabel->setText("");
    processingRateLabel->setText("");
    deviceNumberLabel->setText("");
    cameraResolutionLabel->setText("");
    roiLabel->setText("");
    mouseCursorPosLabel->setText("");
    clearImageBufferButton->setDisabled(true);
} // setInitGUIState()

void MainWindow::signalSlotsInit()
{
    connect(connectToSerialAction,SIGNAL(triggered()), this, SLOT(connectToSerial()));
    connect(disconnectSerialAction, SIGNAL(triggered()),this, SLOT(disconnectSerial()));
    connect(actionExitPort , SIGNAL(triggered()), this, SLOT(close()));

    connect(connectToCameraAction, SIGNAL(triggered()), this, SLOT(connectToCamera()));
    connect(disconnectCameraAction, SIGNAL(triggered()), this, SLOT(disconnectCamera()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    connect(BSAction,SIGNAL(toggled(bool)), this, SLOT(setBS(bool)));
    connect(ColorAction,SIGNAL(toggled(bool)), this, SLOT(setColor(bool)));

    connect(settingsAction, SIGNAL(triggered()), this, SLOT(setProcessingSettings()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(clearImageBufferButton, SIGNAL(released()), this, SLOT(clearImageBuffer()));
    connect(frameLabel, SIGNAL(onMouseMoveEvent()), this, SLOT(updateMouseCursorPosLabel()));
    // Create connection between frameLabel (emitter) and GUI thread (receiver/listener)
    qRegisterMetaType<struct MouseData>("MouseData");
    connect(this->frameLabel,SIGNAL(newMouseData(struct MouseData)),this,SLOT(newMouseData(struct MouseData)));
} // signalSlotsInit()
