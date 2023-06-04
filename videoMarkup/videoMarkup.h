#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_videoMarkup.h"
#include <qmessagebox.h>

#include "Display/QtGuiDisplay.h"
#include "Classifire/QtGuiClassifire.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

class videoMarkup : public QMainWindow
{
    Q_OBJECT

public:
    videoMarkup(QWidget *parent = nullptr);
    ~videoMarkup();

private:
    Ui::videoMarkupClass ui;
    cv::VideoCapture videoFile_{};
    size_t activFrameNumber_{ };


    void setActivFrameNumberToForm();
protected slots:
    void slot_loadVideo();
    void slot_nextFrame();
    void slot_createClassifir();
};
