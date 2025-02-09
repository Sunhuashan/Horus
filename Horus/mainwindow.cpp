﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "monitor.h"
#include "settings.h"
#include "aidetect.h"
#include <QTabWidget>
#include <iostream>
#include <ui_monitor.h>

#include "iconhelper.h"


MainWindow::MainWindow(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    init();

}

MainWindow::~MainWindow() {
    delete ui;
}



void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    if (!this->property("movable").toBool() && !this->property("resizable").toBool()) {
        QWidget::mouseReleaseEvent(event);
        return ;
    }
    if (event->button() == Qt::LeftButton) {
        isLeftButtonPressed = false;
        if (dir != NONE) {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
    QWidget::mouseReleaseEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (!this->property("movable").toBool() && !this->property("resizable").toBool()) {
        QWidget::mouseMoveEvent(event);
        return ;
    }
    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    if(!isLeftButtonPressed) {
        this->region(gloPoint);
    } else {
        if(dir != NONE) {
            QRect rMove(tl, rb);

            switch(dir) {
            case LEFT:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                break;
            case RIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                break;
            case TOP:
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case BOTTOM:
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case TOPLEFT:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case TOPRIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setY(gloPoint.y());
                break;
            case BOTTOMLEFT:
                rMove.setX(gloPoint.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case BOTTOMRIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            default:

                break;
            }
            this->setGeometry(rMove);
        } else {
            move(event->globalPos() - mousePoint);
            event->accept();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (!this->property("movable").toBool() && !this->property("resizable").toBool()) {
        QWidget::mousePressEvent(event);
        return ;
    }
    if (event->button() == Qt::LeftButton) {
        isLeftButtonPressed = true;
        if (dir != NONE) {
            this->mouseGrabber();
        } else {
            mousePoint = event->globalPos() - this->frameGeometry().topLeft();
        }
    }
    QWidget::mousePressEvent(event);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->widgetTitle && event->type() == QEvent::MouseButtonDblClick) {
        on_btnMaximize_clicked();
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void MainWindow::region(const QPoint& cursorGlobalPoint) {
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
        // 左上角
        dir = TOPLEFT;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        // 右下角
        dir = BOTTOMRIGHT;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        //左下角
        dir = BOTTOMLEFT;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
        // 右上角
        dir = TOPRIGHT;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x()) {
        // 左边
        dir = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - PADDING) {
        // 右边
        dir = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if(y >= tl.y() && y <= tl.y() + PADDING) {
        // 上边
        dir = TOP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - PADDING) {
        // 下边
//        dir = BOTTOM;
//        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else {
        // 默认
        dir = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }


}

void MainWindow::init() {

    iconSet();



// ======================================================================


    ui->widgetTitle->installEventFilter(this);

    isLeftButtonPressed = false;
    this->dir = NONE;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMinimumWidth(1200 + 170);
    this->setMinimumHeight(400);
    this->setMouseTracking(true);

    this->setProperty("movable", true);
    this->setProperty("resizable", true);


//    QPixmap pic;
//    pic.load(":/res/images/camera.png");
//    pic = pic.scaled(67, 70);
//    ui->labIcon->setPixmap(pic);

    QFont font;
    font.setPixelSize(30);
    ui->labTitle->setFont(font);
    ui->labTitle->setText("AI智能视频监控平台");

    //    Monitor* w = new Monitor();
    //    ui->tabWidget->clear();
    //    ui->tabWidget->addTab(w, "视频监控界面");
    //    ui->tabWidget->setCurrentIndex(0);

    //    Playback* p = new Playback();
    //    ui->tabWidget->addTab(p, "视频回放界面");



    QList<QToolButton*> btns = ui->widget->findChildren<QToolButton*>();
    foreach (QToolButton* btn, btns) {
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(btnClicked()));
    }


    Monitor* monitor = new Monitor(this);
    treeWidget = monitor->ui->treeDevices;
    //qDebug() << (w == nullptr);
    ui->stackedWidget->addWidget(monitor);
    AIDetect* aidetect = new AIDetect(this);
    aidetect->m_mainwindow = this;
    ui->stackedWidget->addWidget(aidetect);
    Settings* s = new Settings(this);
    ui->stackedWidget->addWidget(s);
    ui->btnMonitor->setChecked(true);
    ui->btnMonitor->click();
}

void MainWindow::iconSet() {
    IconHelper();
    IconHelper::setIcon(ui->btnClose, QString(), 0xeb6a, 15);
    IconHelper::setIcon(ui->btnMaximize, QString(), 0xe60d, 15);
    IconHelper::setIcon(ui->btnMinimize, QString(), 0xe60e, 15);

    IconHelper::setIcon(ui->btnMonitor, QColor("white"), 0xe61f, 36);
    ui->btnMonitor->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    IconHelper::setIcon(ui->btnPlayback, QColor("white"), 0xe60a, 36);
    ui->btnPlayback->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    IconHelper::setIcon(ui->btnSettings, QColor("white"), 0xe65c, 36);
    ui->btnSettings->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    IconHelper::setIcon(ui->labIcon, QString(), 0xe7cc, 67);
}

void MainWindow::on_btnClose_clicked() {
    close();
}

void MainWindow::on_btnMaximize_clicked() {
    static bool max = false;
    if (max) {
        showNormal();
        this->setProperty("movable", true);
        this->setProperty("resizable", true);
    } else {
        showMaximized();
        this->setProperty("movable", false);
        this->setProperty("resizable", false);
    }
    max = !max;
}

void MainWindow::on_btnMinimize_clicked() {
    showMinimized();
}

void MainWindow::btnClicked() {
    QToolButton* b = static_cast<QToolButton*>(sender());
    QString name = b->text();
    QList<QToolButton*> btns = ui->widget->findChildren<QToolButton*>();
    foreach (QToolButton* btn, btns) {
        btn->setChecked(btn == b);
    }
    if (name == "视 频 监 控") {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "智 能 检 测") {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "系 统 设 置") {
        ui->stackedWidget->setCurrentIndex(2);
    }
}
