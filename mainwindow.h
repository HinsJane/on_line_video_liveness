#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QProgressDialog>
#include <QImage>
#include <QTimer>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/video/tracking.hpp>
//#include <opencv2/ml/ml.hpp>
#include <vector>
//#include <dlib/opencv/cv_image.h>
//#include <dlib/image_processing.h>
//#include <dlib/image_io.h>
//#include <dlib/dir_nav.h>
#include <ctime>

using namespace cv;
//using namespace dlib;
using namespace std;

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
    void on_pushButton_camera_clicked();
    void readFrame();

    void on_pushButton_face_live_clicked();

    void on_pushButton_quit_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
//    cv::Mat image;
    cv::Mat frame, frame_origi, rgb_frame, optical_res;
    cv::Mat prev_frame, curr_frame, history, history_gray,  diff;
//    dlib::array2d<unsigned char>img;
    cv::VideoCapture cap;
    cv::CascadeClassifier faceCascade;
    bool cameraIsOpened;
    bool FaceDetectionIsOK;
    bool InitFaceCascadeIsOK;
    bool FaceLivenessIsOK;
    bool InitFaceLivenessIsOK;
    bool needFaceDetectionOrNot;
    bool firstFrameIsOK;
//    dlib::correlation_tracker tracker;

private:
    int maxFace(std::vector<cv::Rect> &faces);
    double MHI_DURATION = 0.5;
    double DEFAULT_THRESHOLD = 32;
    double MAX_TIME_DELTA = 0.25;
    double MIN_TIME_DELTA = 0.05;
};

#endif // MAINWINDOW_H
