/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/

#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    // Setup user interface
    ui->setupUi(this);
    isPlay=false;
    ui->ledIndicator_1->setDisabled(true);
    //ui->on_off->setText("Play");
    playCtl = new QPushButton(this);
    playCtl->setIcon(QIcon(QPixmap("/home/edwin/Documents/MS/images/connect.png")));
    playCtl->setIconSize(QSize(25, 25));
    playCtl->setStyleSheet("QPushButton{border: none;outline: none;}");
    playCtl->move(1000,600);
    playCtl->setDisabled(true);
    //leds.append(new QLedIndicator(this));
    //QPalette Pal(palette());
    // Asignar el color de fondo como Negro
    //Pal.setColor(QPalette::Background, QColor::fromRgb(129, 0, 49 ));
    //Pal.setColor(QPalette::Foreground , QColor::fromRgb(0, 0, 0 ));
    //Pal.setColor(QPalette::Dark , QColor::fromRgb(100, 10, 90 ));
    //setAutoFillBackground(true);
    //setPalette(Pal);
    // Create controller
    controller = new Controller;
    // Save application version in QString variable
    appVersion=QUOTE(APP_VERSION);
    // Create processingSettingsDialog
    processingSettingsDialog = new ProcessingSettingsDialog();

    initializeProcessingFlagsStructure();

    initializeTaskDataStructure();

    setInitGUIState();

    initializeGUI();

    signalSlotsInit();

    rgbOn=false;
    hsvOn=false;
    ycrcbOn=false;

    teamOn=false;
    robot1On=false;
    robot2On=false;
    ballOn=false;

    isCameraConnected=false;
    isPortConnected=false;



    settingsDialog = new SettingsDialog(this);
    cameraConnectDialog = new CameraConnectDialog(this);
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
    if(isPortConnected)
    {
        controller->sendThread->disconnectSerial();
        //disconnect(controller->processingThread,SIGNAL(newData(QString)),0,0);
        //disconnect(controller->processingThread,SIGNAL(newFrame(QImage)),0,0);
        isPortConnected=false;
        ui->SerialAction->setDisabled(false);
    }

} // MainWindow destructor

void MainWindow::connectToSerial()
{
    if(settingsDialog->exec()==1)
    {
        settings=settingsDialog->settings();
        isSerial=true;
        if(isCamera)
            ui->StartAction->setDisabled(false);
    }
}

void MainWindow::connectToCamera()
{
    if(cameraConnectDialog->exec()==1)
    {
        cameraConnectDialog->setDeviceNumber();
        cameraConnectDialog->setImageBufferSize();
        // Store image buffer size in local variable
        imageBufferSize=cameraConnectDialog->getImageBufferSize();
        // Store device number in local variable
        deviceNumber=cameraConnectDialog->getDeviceNumber();
        isCamera=true;
        if(isSerial)
            ui->StartAction->setDisabled(false);
    }
} // connectToCamera()

void MainWindow::readData()
{
    //ui->datalabel_2->setText(controller->sendThread->serial->readAll());

}

void MainWindow::connectToStart()
{
    if(QString::compare(ui->StartAction->text(),"Connect",Qt::CaseSensitive)==0)
    {
        if(!isPortConnected)
            isPortConnected=controller->connectToSerial(settings);
        if(!isCameraConnected)
            isCameraConnected=controller->connectToCamera(deviceNumber,imageBufferSize);
        if(isPortConnected && isCameraConnected)
        {
            playCtl->setDisabled(false);
            connect(playCtl,SIGNAL(clicked()),this,SLOT(play()));
            connect(controller->sendThread->serial, SIGNAL(readyRead()), this,SLOT(readData()),Qt::UniqueConnection);
            connect(controller->processingThread,SIGNAL(newData(QString)),this,SLOT(updateData(QString)),Qt::UniqueConnection);
            connect(controller->processingThread,SIGNAL(newFrame(QImage)),this,SLOT(updateFrame(QImage)),Qt::UniqueConnection);
            qRegisterMetaType<struct ProcessingFlags>("ProcessingFlags");
            connect(this,SIGNAL(newProcessingFlags(struct ProcessingFlags)),controller->processingThread,SLOT(updateProcessingFlags(struct ProcessingFlags)),Qt::UniqueConnection);
            qRegisterMetaType<struct ProcessingSettings>("ProcessingSettings");
            connect(this->processingSettingsDialog,SIGNAL(newProcessingSettings(struct ProcessingSettings)),controller->processingThread,SLOT(updateProcessingSettings(struct ProcessingSettings)),Qt::UniqueConnection);
            qRegisterMetaType<struct TaskData>("TaskData");
            connect(this,SIGNAL(newTaskData(struct TaskData)),controller->processingThread,SLOT(updateTaskData(struct TaskData)),Qt::UniqueConnection);
            qRegisterMetaType<struct PosData>("PosData");
            connect(this,SIGNAL(newPosData(struct PosData)),controller->processingThread,SLOT(updatePosData(PosData)),Qt::UniqueConnection);
            // Set data to defaults in processingThread
            emit newProcessingFlags(processingFlags);
            emit newTaskData(taskData);
            processingSettingsDialog->resetAllDialogToDefaults();
            processingSettingsDialog->updateStoredSettingsFromDialog();
            // Setup imageBufferBar in main window with minimum and maximum values
            ui->imageBufferBar->setMinimum(0);
            ui->imageBufferBar->setMaximum(imageBufferSize);

            ui->menuCalibration->setDisabled(false);
            ui->processingMenu->setDisabled(false);

            // Enable "Clear Image Buffer" push button in main window
            ui->clearImageBufferButton->setDisabled(false);
            // Get input stream properties
            sourceWidth=controller->captureThread->getInputSourceWidth();
            sourceHeight=controller->captureThread->getInputSourceHeight();
            // Set text in labels in main window
            ui->deviceNumberLabel->setNum(deviceNumber);
            ui->cameraResolutionLabel->setText(QString::number(sourceWidth)+QString("x")+QString::number(sourceHeight));
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
            //controller->sendThread->start();
            ui->StartAction->setIcon(QIcon(":/images/disconnect.png"));
            ui->StartAction->setText("Disconnect");
            ui->SerialAction->setDisabled(true);
            ui->CameraAction->setDisabled(true);
            // StopAction->setDisabled(false);
            //StartAction->setDisabled(true);
        }

        else
        {
            if(!isCameraConnected && isPortConnected)
                QMessageBox::warning(this,"ERROR:","Could not connect to camera");
            else if(isCameraConnected && !isPortConnected)
                QMessageBox::warning(this,"ERROR:","Could not connect to serial port");
            else
                QMessageBox::warning(this,"ERROR:","Could not connect to camera and serial port");
        }
    }
    else
    {
        if(controller->captureThread->isCameraConnected() || controller->sendThread->isPortConnected())
        {
            if(controller->captureThread->isCameraConnected())
            {
                ui->StartAction->setIcon(QIcon(":/images/connect.png"));
                ui->StartAction->setText("Connect");
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
                    ui->processingMenu->setDisabled(true);
                    ui->menuCalibration->setDisabled(true);
                    ui->CameraAction->setDisabled(false);
                    // Set GUI in main window
                    ui->BSAction->setChecked(false);
                    //ui->ColorAction->setChecked(false);
                    ui->frameLabel->setText("No camera connected.");
                    ui->frameLabel_2->setText("No camera connected.");
                    ui->imageBufferBar->setValue(0);
                    ui->imageBufferLabel->setText("[000/000]");
                    ui->captureRateLabel->setText("");
                    ui->processingRateLabel->setText("");
                    ui->deviceNumberLabel->setText("");
                    ui->cameraResolutionLabel->setText("");
                    ui->roiLabel->setText("");
                    ui->mouseCursorPosLabel->setText("");
                    ui->clearImageBufferButton->setDisabled(true);
                }
                else
                    QMessageBox::warning(this,"ERROR:","Thread(s) could not be stopped. Camera was not disconnected.");
            }
            if(isPortConnected)
            {
                controller->sendThread->disconnectSerial();
                //disconnect(controller->processingThread,SIGNAL(newData(QString)),0,0);
                //disconnect(controller->processingThread,SIGNAL(newFrame(QImage)),0,0);
                isPortConnected=false;
                ui->SerialAction->setDisabled(false);
            }
            else
                QMessageBox::warning(this,"ERROR:","Serial port was not disconnected.");
            //StopAction->setDisabled(true);
            ui->StartAction->setDisabled(true);
        }
    }
}


void MainWindow::updateData(const QString data)
{
    controller->sendThread->receive(data);
    //ui->datalabel->setText(data);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("App Mirosot Peru"),
                       tr("<BR>"
                          "<b>Author:</b> Edwin Christian Yllanes Cucho<br>"
                          "<b>Email:</b> <a href='mailto:e.yllanescucho@ieee.org'>e.yllanescucho@ieee.org</a><br><b>Version</b>: ")+appVersion+tr("<br>  <b>Organization:</b> RAS IEEE UNI"));
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

void MainWindow::updateFrame(const QImage &frame)
{
    // Show [number of images in buffer / image buffer size] in imageBufferLabel in main window
    ui->imageBufferLabel->setText(QString("[")+QString::number(controller->processingThread->getCurrentSizeOfBuffer())+
                                  QString("/")+QString::number(imageBufferSize)+QString("]"));
    // Show percentage of image bufffer full in imageBufferBar in main window
    ui->imageBufferBar->setValue(controller->processingThread->getCurrentSizeOfBuffer());
    // Show processing rate in captureRateLabel in main window
    ui->captureRateLabel->setNum(controller->captureThread->getAvgFPS());
    ui->captureRateLabel->setText(ui->captureRateLabel->text()+" fps");
    // Show processing rate in processingRateLabel in main window
    ui->processingRateLabel->setNum(controller->processingThread->getAvgFPS());
    ui->processingRateLabel->setText(ui->processingRateLabel->text()+" fps");
    // Show ROI information in roiLabel in main window
    ui->roiLabel->setText(QString("(")+QString::number(controller->processingThread->getCurrentROI().x)+QString(",")+
                          QString::number(controller->processingThread->getCurrentROI().y)+QString(") ")+
                          QString::number(controller->processingThread->getCurrentROI().width)+
                          QString("x")+QString::number(controller->processingThread->getCurrentROI().height));
    // Display frame in main window
    ui->frameLabel->setPixmap(QPixmap::fromImage(frame));
    ui->frameLabel_2->setPixmap(QPixmap::fromImage(frame));
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
    ui->mouseCursorPosLabel->setText(QString("(")+QString::number(ui->frameLabel->getMouseCursorPos().x())+
                                     QString(",")+QString::number(ui->frameLabel->getMouseCursorPos().y())+
                                     QString(")"));
} // updateMouseCursorPosLabel()

void MainWindow::newMouseData(struct MouseData mouseData)
{
    // Local variables
    int x_temp, y_temp, width_temp, height_temp;
    // Set ROI
    if(mouseData.leftButtonRelease)
    {
        // Copy box dimensions from mouseData to taskData
        taskData.selectionBox.setX(mouseData.selectionBox.x());
        taskData.selectionBox.setY(mouseData.selectionBox.y());
        taskData.selectionBox.setWidth(mouseData.selectionBox.width());
        taskData.selectionBox.setHeight(mouseData.selectionBox.height());
        // Check if selection box has NON-ZERO dimensions
        if((taskData.selectionBox.width()!=0) || ((taskData.selectionBox.height())!=0))
        {
            // Selection box can also be drawn from bottom-right to top-left corner
            if(taskData.selectionBox.width()<0)
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
            }
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

                posData.initial.setX(mouseData.initial.x());
                posData.initial.setY(mouseData.initial.y());
                posData.final.setX(mouseData.final.x());
                posData.final.setY(mouseData.final.y());

                emit newPosData(posData);
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
    processingFlags.playOn=false;
    processingFlags.rgbOn=false;
    processingFlags.hsvOn=false;
    processingFlags.ycrcbOn=false;
    processingFlags.teamOn=false;
    processingFlags.robot1On=false;
    processingFlags.robot2On=false;
    processingFlags.ballOn=false;
} // initializeProcessingFlagsStructure()

void MainWindow::initializeTaskDataStructure()
{
    taskData.setROIFlag=false;
    taskData.resetROIFlag=false;
} // initializeTaskDataStructure()

void MainWindow::initializeGUI()
{
    // Enable/disable appropriate menu items
    ui->CameraAction->setDisabled(false);
    ui->SerialAction->setDisabled(false);
    ui->exitAction->setDisabled(false);
    ui->StartAction->setDisabled(true);
    ui->processingMenu->setDisabled(true);
    ui->menuCalibration->setDisabled(true);
    //StopAction->setDisabled(true);
} // initializeGUI()

void MainWindow::setInitGUIState()
{
    // Set GUI in main window
    ui->BSAction->setChecked(false);
    //ui->ColorAction->setChecked(false);
    ui->frameLabel->setText("No camera connected.");
    ui->frameLabel_2->setText("No camera connected.");
    ui->imageBufferBar->setValue(0);
    ui->imageBufferLabel->setText("[000/000]");
    ui->captureRateLabel->setText("");
    ui->processingRateLabel->setText("");
    ui->deviceNumberLabel->setText("");
    ui->cameraResolutionLabel->setText("");
    ui->roiLabel->setText("");
    ui->mouseCursorPosLabel->setText("");
    ui->clearImageBufferButton->setDisabled(true);
} // setInitGUIState()

void MainWindow::signalSlotsInit()
{

    connect(ui->CameraAction,SIGNAL(triggered()),this, SLOT(connectToCamera()));
    connect(ui->SerialAction,SIGNAL(triggered()),this, SLOT(connectToSerial()));
    connect(ui->exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->StartAction, SIGNAL(triggered()),this, SLOT(connectToStart()));
    connect(ui->BSAction,SIGNAL(toggled(bool)), this, SLOT(setBS(bool)));
    connect(ui->settingsAction, SIGNAL(triggered()), this, SLOT(setProcessingSettings()));
    connect(ui->aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->clearImageBufferButton, SIGNAL(released()), this, SLOT(clearImageBuffer()));
    connect(ui->frameLabel, SIGNAL(onMouseMoveEvent()), this, SLOT(updateMouseCursorPosLabel()));

    connect(ui->actionRGB,SIGNAL(toggled(bool)),this,SLOT(setColorRGB(bool)));
    connect(ui->actionHSV,SIGNAL(toggled(bool)),this,SLOT(setColorHSV(bool)));
    connect(ui->actionYCrCb,SIGNAL(toggled(bool)),this,SLOT(setColorYCrCb(bool)));

    connect(ui->actionTeam,SIGNAL(toggled(bool)),this,SLOT(setTeam(bool)));
    connect(ui->actionRobot1,SIGNAL(toggled(bool)),this,SLOT(setRobot1(bool)));
    connect(ui->actionRobot2,SIGNAL(toggled(bool)),this,SLOT(setRobot2(bool)));
    connect(ui->actionBall,SIGNAL(toggled(bool)),this,SLOT(setBall(bool)));
    // Create connection between frameLabel (emitter) and GUI thread (receiver/listener)
    qRegisterMetaType<struct MouseData>("MouseData");
    connect(ui->frameLabel,SIGNAL(newMouseData(struct MouseData)),this,SLOT(newMouseData(struct MouseData)));


} // signalSlotsInit()

void MainWindow::setColorRGB(bool input)
{
    if(!input)
        rgbOn=false;
    else if(input)
    {
        rgbOn=true;
        setColorModel(1);
    }
}

void MainWindow::setColorHSV(bool input)
{
    if(!input)
        hsvOn=false;
    else if(input)
    {
        hsvOn=true;
        setColorModel(2);
    }
}

void MainWindow::setColorYCrCb(bool input)
{
    if(!input)
        ycrcbOn=false;
    else if(input)
    {
        ycrcbOn=true;
        setColorModel(3);
    }
}

void MainWindow::setColorModel(int type)
{
    if(type==1)
    {
        ui->actionHSV->setChecked(false);
        hsvOn=false;
        ui->actionYCrCb->setChecked(false);
        ycrcbOn=false;
    }
    else if(type==2)
    {
        ui->actionRGB->setChecked(false);
        rgbOn=false;
        ui->actionYCrCb->setChecked(false);
        ycrcbOn=false;
    }
    else if(type==3)
    {
        ui->actionRGB->setChecked(false);
        rgbOn=false;
        ui->actionHSV->setChecked(false);
    }
    processingFlags.rgbOn=rgbOn;
    processingFlags.hsvOn=hsvOn;
    processingFlags.ycrcbOn=ycrcbOn;
    emit newProcessingFlags(processingFlags);
}

void MainWindow::setTeam(bool input)
{
    if(!input)
        teamOn=false;
    else if(input)
    {
        teamOn=true;
        setObject(1);
    }
}

void MainWindow::setRobot1(bool input)
{
    if(!input)
        robot1On=false;
    else if(input)
    {
        robot1On=true;
        setObject(2);
    }
}

void MainWindow::setRobot2(bool input)
{
    if(!input)
        robot2On=false;
    else if(input)
    {
        robot2On =true;
        setObject(3);
    }
}

void MainWindow::setBall(bool input)
{
    if(!input)
        ballOn=false;
    else if(input)
    {
        ballOn =true;
        setObject(4);
    }
}

void MainWindow::setObject(int type)
{
    if(!isPlay)
    {
        if(type==1)
        {
            ui->actionRobot1->setChecked(false);
            robot1On=false;
            ui->actionRobot2->setChecked(false);
            robot2On=false;
            ui->actionBall->setChecked(false);
            ballOn=false;
        }
        else if(type==2)
        {
            ui->actionTeam->setChecked(false);
            teamOn=false;
            ui->actionRobot2->setChecked(false);
            robot2On=false;
            ui->actionBall->setChecked(false);
            ballOn=false;
        }
        else if(type==3)
        {
            ui->actionTeam->setChecked(false);
            teamOn=false;
            ui->actionRobot1->setChecked(false);
            robot1On=false;
            ui->actionBall->setChecked(false);
            ballOn=false;
        }
        else if(type==4)
        {
            ui->actionTeam->setChecked(false);
            teamOn=false;
            ui->actionRobot1->setChecked(false);
            robot1On=false;
            ui->actionRobot2->setChecked(false);
            robot2On=false;
        }
    }
    processingFlags.teamOn=teamOn;
    processingFlags.robot1On=robot1On;
    processingFlags.robot2On=robot2On;
    processingFlags.ballOn=ballOn;
    emit newProcessingFlags(processingFlags);
}
void MainWindow::play()
{
    ui->ledIndicator_1->toggle();
    if(isPlay)
    {
        isPlay=false;
        ui->actionTeam->setChecked(false);
        ui->actionRobot1->setChecked(false);
        ui->actionRobot2->setChecked(false);
        ui->actionBall->setChecked(false);
        processingFlags.teamOn=false;
        processingFlags.robot1On=false;
        processingFlags.robot2On=false;
        processingFlags.ballOn=false;
    }
    else if(!isPlay)
    {
        isPlay=true;
    }
    emit newProcessingFlags(processingFlags);
}
