/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho   <e.yllanescucho@ieee.org>   */
/*                                  <e.yllanescucho@gmail.com>  */
/****************************************************************/

#include "ProcessingThread.h"

ProcessingThread::ProcessingThread(ImageBuffer *imageBuffer, int inputSourceWidth, int inputSourceHeight): QThread(),  imageBuffer(imageBuffer), inputSourceWidth(inputSourceWidth), inputSourceHeight(inputSourceHeight)
{
    // Create images
    currentFrameCopy.create(cv::Size(inputSourceWidth,inputSourceHeight),CV_8UC3);//window 1
    currentFrameCopy2.create(cv::Size(inputSourceWidth,inputSourceHeight),CV_8UC3);//window 2
    currentFrameCopybin.create(cv::Size(inputSourceWidth,inputSourceHeight),CV_8UC1);
    // Initialize variables
    stopped=false;
    sampleNo=0;
    fpsSum=0;
    avgFPS=0;
    fps.clear();
    TeamColorType=0;
    Robot1ColorType=0;
    Robot2ColorType=0;
    BallColorType=0;
    playOn=false;

    SmoothType=0;
    SmoothSize=1;
    FlipType=NFLIP;
    ErodeType=cv::MORPH_RECT;
    ErodeSize=cv::MORPH_RECT;
    DilateType=1;
    DilateSize=1;

    BSNumberOfIterations=DEFAULT_BS_ITERATIONS;
    // Initialize currentROI variable
    currentROI=cv::Rect(0,0,inputSourceWidth,inputSourceHeight);
    campROI=currentROI;
    // Store original ROI
    originalROI=currentROI;
    isPos=false;
    initial.x=-10;
    initial.y=-10;
    final.x=-10;
    final.y=-10;
    setDilate(DilateType,DilateSize);
    setErode(ErodeType,ErodeSize);
} // ProcessingThread constructor

ProcessingThread::~ProcessingThread()
{
} // ProcessingThread destructor

void ProcessingThread::setDilate(int type, int size)
{
    elementDilate = cv::getStructuringElement(type,cv::Size( 2*size + 1, 2*size+1 ),cv::Point(size, size));
}

void ProcessingThread::setErode(int type, int size)
{
    elementErode = cv::getStructuringElement(type,cv::Size( 2*size + 1, 2*size+1 ),cv::Point(size, size));
}

void ProcessingThread::run()
{
    cv::BackgroundSubtractorMOG2 bg;
    bg.set("nmixtures",7);
    bg.set("detectShadows",true);

    cv::Mat fore;
    std::vector<cv::Mat> bgr_planes;
    int histSize = 256;
    float range[] = {0,256} ;
    const float* histRange = { range };
    cv::Mat b_hist, g_hist, r_hist;
    int bin_w;
    int hist_w = 256; int hist_h = 400;
    float angle=0;
    std::vector<std::vector<cv::Point> > contours;
    while(true)
    {
        /////////////////////////////////
        // Stop thread if stopped=TRUE //
        /////////////////////////////////
        stoppedMutex.lock();
        if (stopped)
        {
            stopped=false;
            stoppedMutex.unlock();
            break;
        }
        stoppedMutex.unlock();
        /////////////////////////////////
        /////////////////////////////////
        // Save processing time
        processingTime=t.elapsed();
        // Start timer (used to calculate processing rate)
        t.start();
        // Get frame from queue
        cv::Mat currentFrame=imageBuffer->getFrame();
        // Make copy of current frame (processing will be performed on this copy)
        currentFrame.copyTo(currentFrameCopy2);
        //cv::flip(currentFrameCopy2,currentFrameCopy2,-1);
        if(SmoothType==0)
        {
            cv::blur(currentFrameCopy2, currentFrameCopy, cv::Size( 2*SmoothSize+1, 2*SmoothSize+1), cv::Point(-1,-1));
        }
        else if(SmoothType==1)
        {
            cv::GaussianBlur( currentFrameCopy2, currentFrameCopy, cv::Size( 2*SmoothSize+1, 2*SmoothSize+1 ), 0, 0 );
        }
        else if(SmoothType==2)
        {
            cv::medianBlur(currentFrameCopy2, currentFrameCopy, 2*SmoothSize+1 );
        }
        else if(SmoothType==3)
        {
            cv::bilateralFilter (currentFrameCopy2, currentFrameCopy, 2*SmoothSize+1, (2*SmoothSize+1)*2, (2*SmoothSize+1)/2 );;
        }
        else
        {
            currentFrameCopy2.copyTo(currentFrameCopy);
        }

        if(FlipType!=NFLIP)
            cv::flip(currentFrameCopy,currentFrameCopy,FlipType);
        //line(currentFrameCopy, initial, final, CV_RGB(255, 255, 0), 2, CV_AA);
        //circle(currentFrameCopy , initial, 3, CV_RGB(255, 0, 255), 2, CV_AA);
        //circle(currentFrameCopy , final, 3, CV_RGB(0, 255, 255), 2, CV_AA);
        updateMembersMutex.lock();
        ///////////////////
        // PERFORM TASKS //
        ///////////////////
        if(resetROIFlag)
            resetROI();
        else if(setROIFlag)
        {
            setROI();
            if(!playOn)
            {
                currentFrameCopy.locateROI(frameSize,framePoint);
                currentFrameCopy.adjustROI(-currentROI.y,-(frameSize.height-currentROI.height-currentROI.y),                           -currentROI.x,-(frameSize.width-currentROI.width-currentROI.x));
                if(campOn)
                {
                    campROI=currentROI;
                }
                else{
                    if(teamOn)
                    {
                        if(TeamColorType==1)//HSV
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2HSV);
                        else if(TeamColorType==2)
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2YCrCb);
                        cv::meanStdDev(currentFrameCopy,meantmp,stdtmp);
                        Teammin=cv::Scalar(int(meantmp.val[0]-LAMBDA*stdtmp.val[0]),int(meantmp.val[1]-LAMBDA*stdtmp.val[1]),int(meantmp.val[2]-LAMBDA*stdtmp.val[2]));
                        Teammax=cv::Scalar(int(meantmp.val[0]+LAMBDA*stdtmp.val[0]),int(meantmp.val[1]+LAMBDA*stdtmp.val[1]),int(meantmp.val[2]+LAMBDA*stdtmp.val[2]));
                        psettings.TeamColorType=TeamColorType;
                        psettings.TeamChannel1min=Teammin.val[0];
                        psettings.TeamChannel1max=Teammax.val[0];
                        psettings.TeamChannel2min=Teammin.val[1];
                        psettings.TeamChannel2max=Teammax.val[1];
                        psettings.TeamChannel3min=Teammin.val[2];
                        psettings.TeamChannel3max=Teammax.val[2];
                        emit newProcessingSettings(psettings);
                    }
                    else if(robot1On)
                    {
                        if(Robot1ColorType==1)//HSV
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2HSV);
                        else if(Robot1ColorType ==2)
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2YCrCb);
                        cv::meanStdDev(currentFrameCopy,meantmp,stdtmp);
                        Robot1min=cv::Scalar(int(meantmp.val[0]-LAMBDA*stdtmp.val[0]),int(meantmp.val[1]-LAMBDA*stdtmp.val[1]),int(meantmp.val[2]-LAMBDA*stdtmp.val[2]));
                        Robot1max=cv::Scalar(int(meantmp.val[0]+LAMBDA*stdtmp.val[0]),int(meantmp.val[1]+LAMBDA*stdtmp.val[1]),int(meantmp.val[2]+LAMBDA*stdtmp.val[2]));
                        psettings.Robot1ColorType=Robot1ColorType;
                        psettings.Robot1Channel1min=Robot1min.val[0];
                        psettings.Robot1Channel1max=Robot1max.val[0];
                        psettings.Robot1Channel2min=Robot1min.val[1];
                        psettings.Robot1Channel2max=Robot1max.val[1];
                        psettings.Robot1Channel3min=Robot1min.val[2];
                        psettings.Robot1Channel3max=Robot1max.val[2];
                        emit newProcessingSettings(psettings);
                    }
                    else if(robot2On)
                    {
                        if(Robot2ColorType==1)//HSV
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2HSV);
                        else if(Robot2ColorType ==2)
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2YCrCb);
                        cv::meanStdDev(currentFrameCopy,meantmp,stdtmp);
                        psettings.Robot2ColorType=Robot2ColorType;
                        Robot2min=cv::Scalar(int(meantmp.val[0]-LAMBDA*stdtmp.val[0]),int(meantmp.val[1]-LAMBDA*stdtmp.val[1]),int(meantmp.val[2]-LAMBDA*stdtmp.val[2]));
                        Robot2max=cv::Scalar(int(meantmp.val[0]+LAMBDA*stdtmp.val[0]),int(meantmp.val[1]+LAMBDA*stdtmp.val[1]),int(meantmp.val[2]+LAMBDA*stdtmp.val[2]));
                        psettings.Robot2Channel1min=Robot2min.val[0];
                        psettings.Robot2Channel1max=Robot2max.val[0];
                        psettings.Robot2Channel2min=Robot2min.val[1];
                        psettings.Robot2Channel2max=Robot2max.val[1];
                        psettings.Robot2Channel3min=Robot2min.val[2];
                        psettings.Robot2Channel3max=Robot2max.val[2];
                        emit newProcessingSettings(psettings);
                    }
                    else if(ballOn)
                    {
                        if(BallColorType==1)//HSV
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2HSV);
                        else if(BallColorType ==2)
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2YCrCb);
                        cv::meanStdDev(currentFrameCopy,meantmp,stdtmp);
                        Ballmin=cv::Scalar(int(meantmp.val[0]-LAMBDA*stdtmp.val[0]),int(meantmp.val[1]-LAMBDA*stdtmp.val[1]),int(meantmp.val[2]-LAMBDA*stdtmp.val[2]));
                        Ballmax=cv::Scalar(int(meantmp.val[0]+LAMBDA*stdtmp.val[0]),int(meantmp.val[1]+LAMBDA*stdtmp.val[1]),int(meantmp.val[2]+LAMBDA*stdtmp.val[2]));
                        psettings.BallColorType=BallColorType;
                        psettings.BallChannel1min=Ballmin.val[0];
                        psettings.BallChannel1max=Ballmax.val[0];
                        psettings.BallChannel2min=Ballmin.val[1];
                        psettings.BallChannel2max=Ballmax.val[1];
                        psettings.BallChannel3min=Ballmin.val[2];
                        psettings.BallChannel3max=Ballmax.val[2];
                        emit newProcessingSettings(psettings);
                    }
                    if(teamOn||robot1On||robot2On||ballOn)
                    {
                        cv::split( currentFrameCopy, bgr_planes );
                        cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );
                        calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange);
                        calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange);
                        calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange);
                        bin_w = cvRound( (double) hist_w/histSize );
                        cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
                        cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
                        cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
                        for( int i = 1; i < histSize; i++ )
                        {
                            cv::line( histImage,cv::Point( bin_w*(i-1),hist_h-cvRound(b_hist.at<float>(i-1))) ,cv::Point( bin_w*(i), hist_h-cvRound(b_hist.at<float>(i)) ),cv::Scalar( 255, 0, 0), 2, 8, 0 );
                            cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),cv::Scalar( 0, 255, 0), 2, 8, 0 );
                            cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),cv::Scalar( 0, 0, 255), 2, 8, 0 );
                        }
                        hist=MatToQImage(histImage);
                        emit newHistogram(hist);
                    }
                }
            }
        }
        else
        {
            currentFrameCopy.locateROI(frameSize,framePoint);
            currentFrameCopy.adjustROI(-campROI.y,-(frameSize.height-campROI.height-campROI.y),-campROI.x,-(frameSize.width-campROI.width-campROI.x));
            if(playOn)//Play
            {
                //BS
                bg.operator()(currentFrameCopy,fore,0);
                fore=255*(fore==255);
                //Obtencion de Team
                if(TeamColorType==0)
                    currentFrameCopy.copyTo(team);
                else if(TeamColorType==1)
                    cv::cvtColor(currentFrameCopy,team,cv::COLOR_RGB2HSV);
                else if(TeamColorType==2)
                    cv::cvtColor(currentFrameCopy,team,cv::COLOR_RGB2YCrCb);

                cv::inRange(team,Teammin,Teammax,teambin);
                //cv::bitwise_and(fore,teambin,teambin);
                cv::erode(teambin,teambin,elementErode);
                cv::dilate(teambin,teambin,elementDilate);
                cv::findContours(teambin,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_TC89_L1);
                std::vector<cv::RotatedRect> minRectTeam( contours.size() );
                for(unsigned int i = 0; i < contours.size(); i++ )
                    minRectTeam[i] = cv::minAreaRect(cv::Mat(contours[i]));
                for(unsigned int i = 0; i< contours.size(); i++ )
                {
                    cv::Point2f rect_points[4];
                    minRectTeam[i].points(rect_points);
                    for( int j = 0; j < 4; j++ )
                        cv::line( currentFrameCopy, rect_points[j], rect_points[(j+1)%4], cv::Scalar(255,0,255), 1,CV_AA);
                }

                //Obtencion de Robot1
                if(Robot1ColorType==0)
                    currentFrameCopy.copyTo(Robot1);
                else if(Robot1ColorType==1)
                    cv::cvtColor(currentFrameCopy,Robot1,cv::COLOR_RGB2HSV);
                else if(Robot1ColorType==2)
                    cv::cvtColor(currentFrameCopy,Robot1,cv::COLOR_RGB2YCrCb);
                cv::inRange(Robot1,Robot1min,Robot1max,Robot1bin);
                //cv::bitwise_and(fore,Robot1bin,Robot1bin);
                cv::erode(Robot1bin,Robot1bin,elementErode);
                cv::dilate(Robot1bin,Robot1bin,elementDilate);
                cv::findContours(Robot1bin,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_TC89_L1);

                std::vector<cv::Moments> mu1(contours.size());
                std::vector<cv::Point2f> mc1( contours.size());

                for(unsigned int i = 0; i < contours.size(); i++ )
                    mu1[i] = moments( contours[i], false );

                for(unsigned int i = 0; i < contours.size(); i++ )
                    mc1[i]=cv::Point2f( mu1[i].m10/mu1[i].m00 , mu1[i].m01/mu1[i].m00 );

                std::vector<cv::RotatedRect> minRectRobot1( contours.size());
                for(unsigned int i = 0; i < contours.size(); i++ )
                    minRectRobot1[i] = cv::minAreaRect(cv::Mat(contours[i]));
                for(unsigned int i = 0; i< contours.size(); i++ )
                {
                    cv::Point2f rect_points[4];
                    minRectRobot1[i].points(rect_points);
                    for( int j = 0; j < 4; j++ )
                        cv::line( currentFrameCopy, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,0,255), 1,CV_AA);
                }
                if(!mc1.empty())
                    cv::putText(currentFrameCopy, "Robot1", mc1[0], CV_FONT_HERSHEY_PLAIN, 0.6,cv::Scalar(255, 0, 0), 1, 2 );

                //Obtencion de Robot2
                if(Robot2ColorType==0)
                    currentFrameCopy.copyTo(Robot2);
                else if(Robot2ColorType==1)
                    cv::cvtColor(currentFrameCopy,Robot2,cv::COLOR_RGB2HSV);
                else if(Robot1ColorType==2)
                    cv::cvtColor(currentFrameCopy,Robot2,cv::COLOR_RGB2YCrCb);
                cv::inRange(Robot2,Robot2min,Robot2max,Robot2bin);
                //cv::bitwise_and(fore,Robot1bin,Robot1bin);
                cv::erode(Robot2bin,Robot2bin,elementErode);
                cv::dilate(Robot2bin,Robot2bin,elementDilate);
                cv::findContours(Robot2bin,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_TC89_L1);
                std::vector<cv::RotatedRect> minRectRobot2( contours.size());
                for(unsigned int i = 0; i < contours.size(); i++ )
                    minRectRobot2[i] = cv::minAreaRect(cv::Mat(contours[i]));
                std::vector<cv::Moments> mu2(contours.size());
                std::vector<cv::Point2f> mc2( contours.size());

                for(unsigned int i = 0; i < contours.size(); i++ )
                    mu2[i] = moments( contours[i], false );

                for(unsigned int i = 0; i < contours.size(); i++ )
                    mc2[i]=cv::Point2f( mu2[i].m10/mu2[i].m00 , mu2[i].m01/mu2[i].m00 );

                for(unsigned int i = 0; i< contours.size(); i++ )
                {
                    cv::Point2f rect_points[4];
                    minRectRobot2[i].points(rect_points);
                    for( int j = 0; j < 4; j++ )
                        cv::line( currentFrameCopy, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,255,255), 1,CV_AA);
                }
                if(!mc2.empty())
                    cv::putText(currentFrameCopy, "Robot2", mc2[0], CV_FONT_HERSHEY_PLAIN, 0.6,cv::Scalar(255, 0, 0), 1, 2 );
                cv::bitwise_or(teambin,Robot1bin,currentFrameCopybin);
                cv::bitwise_or(Robot2bin,currentFrameCopybin,currentFrameCopybin);
                frame=MatToQImage(currentFrameCopy);
                frame1=MatToQImage(currentFrameCopybin);
            }
            else
            {//Background Subtraction
                if(bsOn)
                {
                    if(BSNumberOfIterations && !(teamOn||robot1On||robot2On||ballOn))
                    {
                        bg.operator ()(currentFrameCopy,fore);
                        BSNumberOfIterations--;
                        if(BSNumberOfIterations%12<3)
                            newCalibration("Calibrating.. : "+QString::number(BSNumberOfIterations));
                        else if(BSNumberOfIterations%12>7)
                            newCalibration("Calibrating...: "+QString::number(BSNumberOfIterations));
                        else
                            newCalibration("Calibrating.  : "+QString::number(BSNumberOfIterations));
                    }
                    else
                    {
                        bg.operator()(currentFrameCopy,fore,0);
                        //BSNumberOfIterations=0;
                        newCalibration("Calibrate!");
                        fore=255*(fore==255);
                    }
                    //foreout= cv::Mat::zeros(currentFrameCopy.size(), CV_8UC1 );
                    /*cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_TC89_L1);
                    for(unsigned int i=0;i<contours.size();i++){
                        //Ignore all small insignificant areas
                        if(cv::contourArea(contours[i])>=BSAreamin*BSAreamin && cv::contourArea(contours[i])<=BSAreamax*BSAreamax){
                            std::vector<std::vector<cv::Point> > tcontours;
                            tcontours.push_back(contours[i]);
                            cv::drawContours(foreout,tcontours,-1,255,CV_FILLED);
                        }
                    }*/
                    frame1=MatToQImage(fore);
                }
                else if(teamOn||robot1On||robot2On||ballOn)
                {
                    currentFrameCopybin2 = cv::Mat::zeros(currentFrameCopy.size(), CV_8UC1 );
                    if(teamOn)
                    {
                        if(TeamColorType==1)//HSV
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2HSV);
                        else if(TeamColorType==2)
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2YCrCb);
                        cv::inRange(currentFrameCopy,Teammin,Teammax,currentFrameCopybin);
                        cv::erode(currentFrameCopybin,currentFrameCopybin,elementErode);
                        cv::dilate(currentFrameCopybin,currentFrameCopybin,elementDilate);
                        /*cv::findContours(currentFrameCopybin,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_TC89_L1);
                        for(unsigned int i=0;i<contours.size();i++){
                            //Ignore all small insignificant areasEXTERNAL,CV_CHAIN_APPROX_TC89_L1);
                            if(cv::contourArea(contours[i])>=TeamAreamin*TeamAreamin && cv::contourArea(contours[i])<=TeamAreamax*TeamAreamax){
                                std::vector<std::vector<cv::Point> > tcontours;
                                tcontours.push_back(contours[i]);
                                cv::drawContours(currentFrameCopybin2,tcontours,-1,255,CV_FILLED);
                            }
                        }*/
                    }
                    else if(robot1On)
                    {
                        if(Robot1ColorType==1)//HSV
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2HSV);
                        else if(Robot1ColorType==2)
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2YCrCb);
                        cv::inRange(currentFrameCopy,Robot1min,Robot1max,currentFrameCopybin);
                        cv::erode(currentFrameCopybin,currentFrameCopybin,elementErode);
                        cv::dilate(currentFrameCopybin,currentFrameCopybin,elementDilate);
                        /*cv::findContours(currentFrameCopybin,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_TC89_L1);
                        for(unsigned int i=0;i<contours.size();i++){
                            //Ignore all small insignificant areasEXTERNAL,CV_CHAIN_APPROX_TC89_L1);
                            if(cv::contourArea(contours[i])>=Robot1Areamin*Robot1Areamin && cv::contourArea(contours[i])<=Robot1Areamax*Robot1Areamax){
                                std::vector<std::vector<cv::Point> > tcontours;
                                tcontours.push_back(contours[i]);
                                cv::drawContours(currentFrameCopybin2,tcontours,-1,255,CV_FILLED);
                            }
                        }*/
                    }
                    else if(robot2On)
                    {
                        if(Robot2ColorType==1)//HSV
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2HSV);
                        else if(Robot2ColorType==2)
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2YCrCb);
                        cv::inRange(currentFrameCopy,Robot2min,Robot2max,currentFrameCopybin);
                        cv::erode(currentFrameCopybin,currentFrameCopybin,elementErode);
                        cv::dilate(currentFrameCopybin,currentFrameCopybin,elementDilate);
                        /*cv::findContours(currentFrameCopybin,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_TC89_L1);
                        for(unsigned int i=0;i<contours.size();i++){
                            //Ignore all small insignificant areasEXTERNAL,CV_CHAIN_APPROX_TC89_L1);
                            if(cv::contourArea(contours[i])>=Robot2Areamin*Robot2Areamin && cv::contourArea(contours[i])<=Robot2Areamax*Robot2Areamax){
                                std::vector<std::vector<cv::Point> > tcontours;
                                tcontours.push_back(contours[i]);
                                cv::drawContours(currentFrameCopybin2,tcontours,-1,255,CV_FILLED);
                            }
                        }*/
                    }
                    else if(ballOn)
                    {
                        if(BallColorType==1)
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2HSV);
                        else if(BallColorType==2)
                            cv::cvtColor(currentFrameCopy,currentFrameCopy,cv::COLOR_RGB2YCrCb);
                        cv::inRange(currentFrameCopy,Ballmin,Ballmax,currentFrameCopybin);
                        cv::erode(currentFrameCopybin,currentFrameCopybin,elementErode);
                        cv::dilate(currentFrameCopybin,currentFrameCopybin,elementDilate);

                        /*cv::findContours(currentFrameCopybin,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_TC89_L1);
                        for(unsigned int i=0;i<contours.size();i++){
                            //Ignore all small insignificant areasEXTERNAL,CV_CHAIN_APPROX_TC89_L1);
                            if(cv::contourArea(contours[i])>=BallAreamin*BallAreamin && cv::contourArea(contours[i])<=BallAreamax*BallAreamax){
                                std::vector<std::vector<cv::Point> > tcontours;
                                tcontours.push_back(contours[i]);
                                cv::drawContours(currentFrameCopybin2,tcontours,-1,255,CV_FILLED);
                            }
                        }*/
                    }
                    frame1=MatToQImage(currentFrameCopybin);
                }
                else
                {
                    frame1=MatToQImage(currentFrameCopy);
                }
                frame=MatToQImage(currentFrameCopy);
            }
        }
        updateMembersMutex.unlock();
        updateFPS(processingTime);
        currentSizeOfBuffer=imageBuffer->getSizeOfImageBuffer();
        if(playOn)
            if(isPos)
            {
                angle=cv::fastAtan2(final.y-initial.y,final.x-initial.x);
                data=QString::number(angle-180);
                emit newData(data);
            }
        emit newFrame(frame);
        emit newFrame2(frame1);
    }
    qDebug() << "Stopping processing thread...";
}

void ProcessingThread::updateFPS(int timeElapsed)
{
    // Add instantaneous FPS value to queue
    if(timeElapsed>0)
    {
        fps.enqueue((int)1000/timeElapsed);
        // Increment sample number
        sampleNo++;
    }
    // Maximum size of queue is 16
    if(fps.size() > 16)
        fps.dequeue();
    // Update FPS value every 16 samples
    if((fps.size()==16)&&(sampleNo==16))
    {
        // Empty queue and store sum
        while(!fps.empty())
            fpsSum+=fps.dequeue();
        avgFPS=fpsSum/16; // Calculate average FPS
        fpsSum=0; // Reset sum
        sampleNo=0; // Reset sample number
    }
} // updateFPS()

void ProcessingThread::stopProcessingThread()
{
    stoppedMutex.lock();
    stopped=true;
    stoppedMutex.unlock();
} // stopProcessingThread()

void ProcessingThread::setROI()
{
    // Save selection as new (current) ROI
    currentROI=selectionBox;
    qDebug() << "ROI successfully SET.";
    // Reset setROIOn flag to FALSE
    setROIFlag=false;
} // setROI()

void ProcessingThread::resetROI()
{
    // Reset ROI to original
    currentROI=originalROI;
    qDebug() << "ROI successfully RESET.";
    // Reset resetROIOn flag to FALSE
    resetROIFlag=false;
} // resetROI()

void ProcessingThread::updateProcessingFlags(struct ProcessingFlags processingFlags)
{
    QMutexLocker locker(&updateMembersMutex);
    playOn=processingFlags.playOn;
    bsOn=processingFlags.bsOn;
    campOn=processingFlags.campOn;
    teamOn=processingFlags.teamOn;
    robot1On=processingFlags.robot1On;
    robot2On=processingFlags.robot2On;
    ballOn=processingFlags.ballOn;
} // updateProcessingFlags()

void ProcessingThread::updateProcessingSettings(struct ProcessingSettings processingSettings)
{
    QMutexLocker locker(&updateMembersMutex);

    TeamColorType=processingSettings.TeamColorType;
    Teammin=cv::Scalar(processingSettings.TeamChannel1min,processingSettings.TeamChannel2min,processingSettings.TeamChannel3min);
    Teammax=cv::Scalar(processingSettings.TeamChannel1max,processingSettings.TeamChannel2max,processingSettings.TeamChannel3max);
    Robot1ColorType=processingSettings.Robot1ColorType;
    Robot1min=cv::Scalar(processingSettings.Robot1Channel1min,processingSettings.Robot1Channel2min,processingSettings.Robot1Channel3min);
    Robot1max=cv::Scalar(processingSettings.Robot1Channel1max,processingSettings.Robot1Channel2max,processingSettings.Robot1Channel3max);
    Robot2ColorType=processingSettings.Robot2ColorType;
    Robot2min=cv::Scalar(processingSettings.Robot2Channel1min,processingSettings.Robot2Channel2min,processingSettings.Robot2Channel3min);
    Robot2max=cv::Scalar(processingSettings.Robot2Channel1max,processingSettings.Robot2Channel2max,processingSettings.Robot2Channel3max);
    BallColorType=processingSettings.BallColorType;
    Ballmin=cv::Scalar(processingSettings.BallChannel1min,processingSettings.BallChannel2min,processingSettings.BallChannel3min);
    Ballmax=cv::Scalar(processingSettings.BallChannel1max,processingSettings.BallChannel2max,processingSettings.BallChannel3max);

    TeamAreamin=processingSettings.TeamAreamin;
    TeamAreamax=processingSettings.TeamAreamax;
    Robot1Areamin=processingSettings.Robot1Areamin;
    Robot1Areamax=processingSettings.Robot1Areamax;
    Robot2Areamin=processingSettings.Robot2Areamin;
    Robot2Areamax=processingSettings.Robot2Areamax;
    BallAreamin=processingSettings.BallAreamin;
    BallAreamax=processingSettings.BallAreamax;

    SmoothType=processingSettings.SmoothType;
    SmoothSize=processingSettings.SmoothSize;

    FlipType=processingSettings.FlipType;
    DilateType=processingSettings.DilateType;
    DilateSize=processingSettings.DilateSize;
    ErodeType=processingSettings.ErodeType;
    ErodeSize=processingSettings.ErodeSize;

    setDilate(DilateType,DilateSize);
    setErode(ErodeType,ErodeSize);

    psettings.TeamAreamin=processingSettings.TeamAreamin;
    psettings.TeamAreamax=processingSettings.TeamAreamax;
    psettings.Robot1Areamin=processingSettings.Robot1Areamin;
    psettings.Robot1Areamax=processingSettings.Robot1Areamax;
    psettings.Robot2Areamin=processingSettings.Robot2Areamin;
    psettings.Robot2Areamax=processingSettings.Robot2Areamax;
    psettings.BallAreamin=processingSettings.BallAreamin;
    psettings.BallAreamax=processingSettings.BallAreamax;

    psettings.SmoothType=processingSettings.SmoothType;
    psettings.SmoothSize=processingSettings.SmoothSize;
    psettings.TeamColorType=processingSettings.TeamColorType;
    psettings.TeamChannel1min=processingSettings.TeamChannel1min;
    psettings.TeamChannel1max=processingSettings.TeamChannel1max;
    psettings.TeamChannel2min=processingSettings.TeamChannel2min;
    psettings.TeamChannel2max=processingSettings.TeamChannel2max;
    psettings.TeamChannel3min=processingSettings.TeamChannel3min;
    psettings.TeamChannel3max=processingSettings.TeamChannel3max;

    psettings.Robot1ColorType=processingSettings.Robot1ColorType;
    psettings.Robot1Channel1min=processingSettings.Robot1Channel1min;
    psettings.Robot1Channel1max=processingSettings.Robot1Channel1max;
    psettings.Robot1Channel2min=processingSettings.Robot1Channel2min;
    psettings.Robot1Channel2max=processingSettings.Robot1Channel2max;
    psettings.Robot1Channel3min=processingSettings.Robot1Channel3min;
    psettings.Robot1Channel3max=processingSettings.Robot1Channel3max;

    psettings.Robot2ColorType=processingSettings.Robot2ColorType;
    psettings.Robot2Channel1min=processingSettings.Robot2Channel1min;
    psettings.Robot2Channel1max=processingSettings.Robot2Channel1max;
    psettings.Robot2Channel2min=processingSettings.Robot2Channel2min;
    psettings.Robot2Channel2max=processingSettings.Robot2Channel2max;
    psettings.Robot2Channel3min=processingSettings.Robot2Channel3min;
    psettings.Robot2Channel3max=processingSettings.Robot2Channel3max;

    psettings.BallColorType=processingSettings.BallColorType;
    psettings.BallChannel1min=processingSettings.BallChannel1min;
    psettings.BallChannel1max=processingSettings.BallChannel1max;
    psettings.BallChannel2min=processingSettings.BallChannel2min;
    psettings.BallChannel2max=processingSettings.BallChannel2max;
    psettings.BallChannel3min=processingSettings.BallChannel3min;
    psettings.BallChannel3max=processingSettings.BallChannel3max;
    psettings.SmoothType=SmoothType;
    psettings.SmoothSize=SmoothSize;
    psettings.FlipType=FlipType;
    psettings.DilateType=DilateType;
    psettings.DilateSize=DilateSize;
    psettings.ErodeType=ErodeType;
    psettings.ErodeSize=ErodeSize;
} // updateProcessingSettings()

void ProcessingThread::updatePosData(struct PosData posData)
{
    QMutexLocker locker(&updateMembersMutex);
    initial.x=posData.initial.x();
    initial.y=posData.initial.y();
    final.x=posData.final.x();
    final.y=posData.final.y();
    isPos=true;
}

void ProcessingThread::updateBSNumber(int number)
{
    QMutexLocker locker(&updateMembersMutex);
    BSNumberOfIterations=number;
}

void ProcessingThread::updateTaskData(struct TaskData taskData)
{
    QMutexLocker locker(&updateMembersMutex);
    setROIFlag=taskData.setROIFlag;
    resetROIFlag=taskData.resetROIFlag;
    selectionBox.x=taskData.selectionBox.left();
    //qDebug()<<taskData.selectionBox.left();
    //qDebug()<<taskData.selectionBox.right();
    //qDebug()<<taskData.selectionBox.bottom();
    //qDebug()<<taskData.selectionBox.top();
    selectionBox.y=taskData.selectionBox.top();
    selectionBox.width=taskData.selectionBox.width();
    selectionBox.height=taskData.selectionBox.height();
} // updateTaskData()

int ProcessingThread::getAvgFPS()
{
    return avgFPS;
} // getAvgFPS()

int ProcessingThread::getCurrentSizeOfBuffer()
{
    return currentSizeOfBuffer;
} // getCurrentSizeOfBuffer()

cv::Rect ProcessingThread::getCurrentROI()
{
    return currentROI;
} // getCurrentROI();
