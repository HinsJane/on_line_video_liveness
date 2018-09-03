#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(800,600);

    cameraIsOpened = false;
    FaceDetectionIsOK = false;
    FaceLivenessIsOK = false;
    InitFaceCascadeIsOK = false;
    InitFaceLivenessIsOK = false;
    firstFrameIsOK = false;
    needFaceDetectionOrNot = true;

//    diff = cv::Mat::zeros(640, 480, CV_8UC1);
    history = cv::Mat::zeros(480, 640, CV_32FC1);

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()),this, SLOT(readFrame()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::maxFace(std::vector<cv::Rect> &faces)
{
    std::vector<int> face_areas;
    for(size_t i = 0; i < faces.size(); ++i)
    {
        face_areas.push_back(faces.at(i).width * faces.at(i).height);
    }
    std::vector<int>::iterator biggest = std::max_element(std::begin(face_areas),
                                                          std::end(face_areas));
    return std::distance(std::begin(face_areas),biggest);
}


void MainWindow::readFrame()
{
    if(cameraIsOpened)
    {
        cap >> frame;
//        clock_t begin = clock();
        if(!frame.empty())
        {
            if(needFaceDetectionOrNot){
                if(FaceDetectionIsOK)
                {
                    std::vector<cv::Rect>faces;
                    int max_face_index;
                    cv::Mat image_gray;
                    cv::cvtColor(frame, image_gray, cv::COLOR_BGR2GRAY);
                    faceCascade.detectMultiScale(image_gray, faces, 1.1, 2, 0, cv::Size(30,30));

                    if(!firstFrameIsOK){
                        prev_frame = image_gray;
                        firstFrameIsOK = true;
                    }else{
                        curr_frame = image_gray;
                        cv::absdiff(curr_frame, prev_frame, diff);
                        frame.copyTo(frame_origi);
                        cv::calcOpticalFlowFarneback(prev_frame, curr_frame,optical_res,.4,1,12,2,8,1.2,0);
//                        Mat xy[2];
//                        cv::split(optical_res,xy);
//                        Mat magnitude, angle;
//                        cv::cartToPolar(xy[0], xy[1], magnitude, angle, true);
                        for (int y = 0; y < image_gray.rows; y += 5) {
                            for (int x = 0; x < image_gray.cols; x += 5)
                            {
                                // get the flow from y, x position * 3 for better visibility
                                const Point2f flowatxy = optical_res.at<Point2f>(y, x) * 1;
                                // draw line at flow direction
                                line(frame_origi, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)), Scalar(0, 0, 255));
                                // draw initial point
                                circle(frame_origi, Point(x, y), 1, Scalar(255, 0, 0), -1);
                            }
                        }
                        cv::threshold(diff, diff, DEFAULT_THRESHOLD, 1, CV_THRESH_BINARY);
//                        cout << "size:" << diff.rows << " ," << diff.cols << endl;
    //                    clock_t end = clock();
                        double timestamp = cv::getTickCount() / cv::getTickFrequency();
                        cv::updateMotionHistory(diff, history, timestamp, MHI_DURATION);
                        cout <<"[Info]: History shape: " << history.rows <<" " << history.cols << endl;
                        double MIN, MAX;
                        cv::minMaxLoc(history, &MIN, &MAX);
                        Mat history_rgb;
                        history.convertTo(history_gray, CV_8U, 255.0/(MAX - MIN), -255.0/(MAX - MIN));
                        Mat blank_ch = cv::Mat::zeros(480, 640, CV_8UC1);
                        std::vector<cv::Mat> channels_r;
                        channels_r.push_back(history_gray);
                        channels_r.push_back(blank_ch);
                        channels_r.push_back(blank_ch);
                        cv::merge(channels_r, history_rgb);
//                        cv::imshow("diff", diff);
                        cv::imshow("mhi",history_rgb);
                        cv::imshow("opt",frame_origi);
                        prev_frame = curr_frame;
//                        if (cv::waitKey(1) == 27)
//                            exit(EXIT_SUCCESS);
                    }

                    if(faces.size() !=0)
                    {
                        cout << "[Info]: Face detected ------------ \n";
                        max_face_index = maxFace(faces);
                        cv::Rect face_rect = faces.at(max_face_index);

                        int left = face_rect.x;
                        int top = face_rect.y;
                        int width = face_rect.width;
                        int height = face_rect.height;
                        int right = left + width;
                        int bottom = top + height;

                        int left_ext, top_ext, width_ext, height_ext;

                        if((left - width) <= 0)
                            left_ext = 0;
                        else
                            left_ext = left - width;

                        if((top - height) <= 0)
                            top_ext = 0;
                        else
                            top_ext = top - height;

                        if((right + width) >= history_gray.cols)
                            width_ext = history_gray.cols - left_ext;
                        else
                            width_ext = right + width - left_ext;

                        if((bottom  + height) >= history_gray.rows)
                            height_ext = history_gray.rows - top_ext;
                        else
                            height_ext = bottom + height - top_ext;

                        Rect face_rect_ext = Rect(left_ext, top_ext, width_ext, height_ext);

                        Mat face_motion_ext = history_gray(face_rect_ext);
                        int roi_size = 128;
//                        cv::resize(face_motion_ext, face_motion_ext,Size(roi_size,roi_size));
//                        float motion_info;
//                        motion_info = cv::sum(face_motion_ext)[0] / (roi_size * roi_size);
//                        ui->label_show_motion->setText(QString::number(motion_info));

//                        if (motion_info >= 1.0){
//                            Mat xy[2];
//                            cv::split(optical_res,xy);
//                            Mat magnitude, angle;
//                            cv::cartToPolar(xy[0], xy[1], magnitude, angle, true);

//                            int height_above, height_below, height_min;
//                            height_above = top - top_ext;
//                            height_below = height_ext + top_ext - (height + top);
//                            height_min = std::min(height_above, height_below);

//                            Rect above_face_rect = Rect(left, top - height_min, width, height_min);
//                            Rect below_face_rect = Rect(left, top + height, width, height_min);

////                            count << "[Info]: " << ""

//                            Mat above_face_mag_roi = magnitude(above_face_rect);
//                            Mat above_face_ang_roi = angle(above_face_rect);

//                            Mat below_face_mag_roi = magnitude(below_face_rect);
//                            Mat below_face_ang_roi = angle(below_face_rect);

//                            float above_mean_mag, above_mean_ang, below_mean_mag, below_mean_ang;

//                            above_mean_mag = cv::sum(above_face_mag_roi)[0] / (width * height_min);
//                            above_mean_ang = cv::sum(above_face_ang_roi)[0] / (width * height_min);

//                            below_mean_mag = cv::sum(below_face_mag_roi)[0] / (width * height_min);
//                            below_mean_ang = cv::sum(below_face_ang_roi)[0] / (width * height_min);
////                            float vof_above_data[] = {above_mean_mag, above_mean_ang};
////                            float vof_below_data[] = {below_mean_mag, below_mean_ang};
////                            Mat vof_above = Mat(2, 1, CV_32F, vof_above_data);
////                            Mat vof_below = Mat(2, 1, CV_32F, vof_below_data);

//                            Mat vof_above, vof_below;
//                            vof_above.push_back(above_mean_mag);
//                            vof_above.push_back(above_mean_ang);
//                            vof_below.push_back(below_mean_mag);
//                            vof_below.push_back(below_mean_ang);
//                            double ab = vof_above.dot(vof_below);
//                            double aa = vof_above.dot(vof_above);
//                            double bb = vof_below.dot(vof_below);
//                            double cos_sim_opt_flow = -ab / std::sqrt(aa * bb);
//                            ui->label_show_OF->setText(QString::number(cos_sim_opt_flow));
//                        }


                        //TODO:
                        // face roi based motion detection and left-right roi based histogram of
                        // optical flow matching for jilter detection

//                        needFaceDetectionOrNot = false;

//                        cv::rectangle(frame, face_rect, cv::Scalar(0,0,255));
//                        dlib::assign_image(img, dlib::cv_image<unsigned char>(image_gray));
//                        int face_center_x = face_rect.x + cvRound(face_rect.width / 2.0);
//                        int face_center_y = face_rect.y + cvRound(face_rect.height / 2.0);
//                        tracker.start_track(img,
//                                            centered_rect(dlib::point(face_center_x,face_center_y),
//                                                          int(face_rect.width), int(face_rect.height)));
                        Mat face_img = frame(face_rect);
//                        cv::rectangle(frame, face_rect, cv::Scalar(0,0,255));
                        cv::resize(face_img, face_img,Size(128,128));
                        Mat face_img_rgb;
                        cv::cvtColor(face_img, face_img_rgb, cv::COLOR_BGR2RGB);
                        QImage qimage_face = QImage((const unsigned char*)(face_img_rgb.data),
                                                    face_img_rgb.cols, face_img_rgb.rows,
                                                    face_img_rgb.cols*face_img_rgb.channels(),
                                                    QImage::Format_RGB888);
                        ui->label_face->setPixmap(QPixmap::fromImage(qimage_face));
                        ui->label_face->resize(face_img.cols, face_img.rows);
//                        cv::rectangle(frame, face_rect, cv::Scalar(0,0,255));
                    }else
                    {
                        ui->label_face->clear();
                    }

                }

            }else{
//                cv::Mat image_gray;
//                cv::cvtColor(frame, image_gray, cv::COLOR_BGR2GRAY);
//                dlib::assign_image(img, dlib::cv_image<unsigned char>(image_gray));
//                tracker.update(img);
//                dlib::drectangle tracking_rect = tracker.get_position();

//                int tracking_rect_x = tracking_rect.left() < 0 ? 0 :int(tracking_rect.left());
//                int tracking_rect_y = tracking_rect.top() < 0 ? 0 : int(tracking_rect.top());

//                int frame_width = frame.cols;
//                int frame_height = frame.rows;

//                int tracking_rect_w = tracking_rect.right() > frame_width ?
//                            frame_width - tracking_rect_x :
//                            int(tracking_rect.right() - tracking_rect_x);
//                int tracking_rect_h = tracking_rect.bottom() > frame_height ?
//                            frame_height - tracking_rect_y :
//                            int(tracking_rect.bottom() - tracking_rect_y);

//                Mat face_img = frame(Rect(tracking_rect_x,tracking_rect_y,tracking_rect_w,tracking_rect_h));
//                cv::resize(face_img, face_img,Size(128,128));
//                Mat face_img_rgb;
//                cv::cvtColor(face_img, face_img_rgb, cv::COLOR_BGR2RGB);
//                QImage qimage_face = QImage((const unsigned char*)(face_img_rgb.data),
//                                            face_img_rgb.cols, face_img_rgb.rows,
//                                            face_img_rgb.cols*face_img_rgb.channels(),
//                                            QImage::Format_RGB888);
//                ui->label_face->setPixmap(QPixmap::fromImage(qimage_face));
//                ui->label_face->resize(face_img.cols, face_img.rows);
//                cv::rectangle(frame,
//                              Rect(tracking_rect_x,
//                                   tracking_rect_y,
//                                   tracking_rect_w,
//                                   tracking_rect_h),
//                              cv::Scalar(255,0,0));
            }

            cv::cvtColor(frame, rgb_frame, cv::COLOR_BGR2RGB);
            QImage qimage((const unsigned char*)(rgb_frame.data),
                          rgb_frame.cols, rgb_frame.rows,
                          rgb_frame.cols*rgb_frame.channels(),
                          QImage::Format_RGB888);

            ui->label_image->setPixmap(QPixmap::fromImage(qimage));
            ui->label_image->resize(ui->label_image->pixmap()->size());
        }

    }

}


void MainWindow::on_pushButton_camera_clicked()
{
    if(!cameraIsOpened)
    {
        cap.open(0);
        if(cap.isOpened())
        {
            cameraIsOpened = true;
            ui->pushButton_camera->setText(tr("关闭摄像头"));
    //        QMessageBox::information(NULL, tr("OK:"), tr("open one device"));
            timer->start(33);

        }else
        {
            QMessageBox::information(NULL, tr("Error"), tr("找不到设备"));
        }

        if(!InitFaceCascadeIsOK)
        {
            QString path = QCoreApplication::applicationDirPath();
            path += "//haarcascade_frontalface_alt.xml";
//            QMessageBox::information(NULL, tr("人脸检测器"), path);
            if(!faceCascade.load(path.toStdString().data()))
            {
                QMessageBox::information(NULL, tr("Error"), tr("人脸检测初始化失败！"));
            }else{
//                QMessageBox::information(NULL, tr("OK"), tr("人脸检测初始化成功！"));
                InitFaceCascadeIsOK = true;
                FaceDetectionIsOK = true;
            }
        }
    }else
    {
        cap.release();
        cameraIsOpened = false;
        ui->pushButton_camera->setText(tr("打开摄像头"));
        if(!frame.empty())
        {

//            cv::resize(frame, frame, cv::Size(320, 240));
            cv::cvtColor(frame, rgb_frame, cv::COLOR_BGR2RGB);
            QImage qimage((const unsigned char*)(rgb_frame.data),
                          rgb_frame.cols, rgb_frame.rows,
                          rgb_frame.cols*rgb_frame.channels(),
                          QImage::Format_RGB888);

            ui->label_image->setPixmap(QPixmap::fromImage(qimage));
            ui->label_image->resize(ui->label_image->pixmap()->size());
        }else
        {
            QMessageBox::information(NULL, tr("Error"), tr("最后一次读取流数据失败"));
            ui->label_image->clear();
        }

    }
}

void MainWindow::on_pushButton_face_live_clicked()
{
    if(!FaceLivenessIsOK){
        FaceLivenessIsOK = true;
        ui->pushButton_face_live->setText(tr("结束检测"));
    }else{
        FaceLivenessIsOK = false;
        ui->pushButton_face_live->setText(tr("开启检测"));
    }
}

void MainWindow::on_pushButton_quit_clicked()
{
    cv::destroyAllWindows();
    QApplication::quit();
}
