﻿#ifndef AIDETECT_H
#define AIDETECT_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QMimeDatabase>
#include <iostream>
#include <yolov5.h>
#include <chrono>
#include "mainwindow.h"
#include <QDialog>
#include <QListWidget>

#pragma execution_character_set("utf-8")

//#ifdef QT_NO_DEBUG
//#pragma comment(lib, "D:/opencv/build/install/x64/vc15/lib/opencv_world455.lib")
//#endif



namespace Ui {
class AIDetect;
}

class AIDetect : public QWidget {
    Q_OBJECT

  public:
    explicit AIDetect(QWidget* parent = nullptr);
    ~AIDetect();
    void init();

  private slots:

    void readFrame(); //自定义信号处理函数




    void on_selectDevice_clicked();

    void on_openFile_clicked();

    void on_loadFile_clicked();

    void on_model_activated(const QString& arg1);


    void on_startDetect_clicked();

    void on_stopDetect_clicked();

    void on_reset_clicked();


  private:

    void initSelectDeviceDialog(QDialog* dialog);

  private:
    Ui::AIDetect* ui;

    QTimer* timer;
    cv::VideoCapture* capture;

    YOLOV5* yolov5;
    NetConfig conf;
    NetConfig* yolo_nets;
    std::vector<cv::Rect> bboxes;
    int IsDetect_ok = 0;

    QDialog* dialog = nullptr;  // crashed without nullptr

    QListWidget* listWidget;


  public:

    MainWindow* m_mainwindow;
};

#endif // AIDETECT_H
