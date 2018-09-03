#-------------------------------------------------
#
# Project created by QtCreator 2018-08-03T13:10:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = online_video_liveness
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += D:\opencv-2.4.10\mingw_build\install\include\
            D:\opencv-2.4.10\mingw_build\install\include\opencv2\
            D:\opencv-2.4.10\mingw_build\install\include

#INCLUDEPATH += G:\dlib\dlib-19.7

LIBS += D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_core2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_highgui2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_imgproc2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_legacy2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_objdetect2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_video2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_photo2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_calib3d2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_contrib2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_features2d2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_flann2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_gpu2410.dll.a\
        D:\opencv-2.4.10\mingw_build\install\x64\mingw\lib\libopencv_ml2410.dll.a

#LIBS += G:\dlib\Dlib-build\dlib\libdlib.a

LIBS +=  -lwsock32  -lws2_32 -limm32  -luser32 -lgdi32 -lcomctl32 -lwinmm
