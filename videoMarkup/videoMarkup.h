#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_videoMarkup.h"
#include <qmessagebox.h>

#include "Display/QtGuiDisplay.h"
#include "Classifier/QtGuiClassifier.h"

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
    QtGuiClassifier* classifierInterfase{};
    
    size_t activFrameNumber_{ };
    Classifier actionClassifier_{};
    Classifier objectClassifier_{};

    void setActivFrameNumberToForm();
    void updateComboBoxAction();
    void updateComboBoxObject();
protected slots:
    void slot_loadVideo();
    void slot_nextFrame();
    
    void slot_createActionsClassifier();
    void slot_updateActionsClassifier();
    void slot_editActionsClassifire();
    void slot_saveActionsClassifire();
    void slot_loadActionsClassifire();


    void slot_createObjectClassifier();
    void slot_updateObjectClassifier();
    void slot_editObjectClassifier();
    void slot_saveObjectClassifier();
    void slot_loadObjectClassifier();
};
