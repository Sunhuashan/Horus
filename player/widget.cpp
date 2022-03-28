#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Widget) {

    ui->setupUi(this);



    connect(ui->playwidget, SIGNAL(btnClicked_ffmpeg_signal(QString)), this, SLOT(btnClicked_widget_slot(QString)));

    QStringList urls;
    urls << "https://hls01open.ys7.com/openlive/6e0b2be040a943489ef0b9bb344b96b8.hd.m3u8";
    urls << "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";
    urls << "rtsp://admin:Admin123456@192.168.0.64:554/Streaming/Channels/102?transportmode=unicast&profile=Profile_2";
    urls << "rtsp://192.168.1.108:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif";
    urls << "rtsp://192.168.1.15:554/media/video1";
    urls << "rtsp://192.168.1.15:554/media/video2";
    urls << "rtsp://192.168.1.247:554/av0_0";
    urls << "rtsp://192.168.1.247:554/av0_1";
    urls << "rtmp://ivi.bupt.edu.cn:1935/livetv/cctv5phd";
    urls << "rtmp://hls.hsrtv.cn/hls/hstv1";
    urls << "rtmp://hls.hsrtv.cn/hls/hstv2";
//    ui->cboxUrl->addItems(urls);
//    ui->cboxUrl->setCurrentIndex(0);

//    ui->playWidget->setFlowEnable(true);
//    ui->playWidget->initFlowPanel();
//    ui->playWidget->initFlowStyle();
}

Widget::~Widget() {
    delete ui;
}

void Widget::btnClicked_widget_slot(const QString& str) {
    emit btnClicked_widget_signal(str);
}

void Widget::enterEvent(QEvent*) {
    ui->frame->setStyleSheet("border:1px solid blue");
}

void Widget::leaveEvent(QEvent*) {
    ui->frame->setStyleSheet("border:1px solid gray");
}

//void Widget::on_btnOpen_clicked() {
//    if (ui->btnOpen->text() == "打开") {
//        ui->btnOpen->setText("关闭");
//        QString url = ui->cboxUrl->currentText().trimmed();
//        ui->playWidget->setUrl(url);
//        ui->playWidget->open();
//    } else {
//        ui->btnOpen->setText("打开");
//        ui->playWidget->close();
//    }
//}
