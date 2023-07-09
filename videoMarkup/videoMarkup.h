#pragma once
#include <fstream>
#include <string>

#include <QtWidgets/QMainWindow>
#include "ui_videoMarkup.h"
#include <qmessagebox.h>

#include "Display/QtGuiDisplay.h"
#include "Classifier/QtGuiClassifier.h"
#include"MarkupObject.h"

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
    int activMarkupObject_{ -1 };

    Classifier actionClassifier_{};
    Classifier objectClassifier_{};

    std::vector<MarkupObject> markupObjects_;
    std::ofstream outMarkupFilename_{};

    void setActivFrameNumberToForm();
    void updateComboBoxAction();
    void updateComboBoxObject();
    void updatePb_AddObjectStatus();
    void updatePb_NextFrameStatus();

    void writeCoordinateMarkupObjectToFile();
    void readLastFrameFromMarkupFile(const std::string& filename);
protected slots:
    void slot_loadVideo();
    void slot_nextFrame();
    
    void slot_addMarkupObject();
    void slot_removeActivMarkupObject();
    void slot_changeActivMarkupObject(int index);
    void slot_setActivMarkupObject(int index);

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

    void slot_createMarkup();
    void slot_openMarkup();
};
