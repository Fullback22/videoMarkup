#include "videoMarkup.h"

videoMarkup::videoMarkup(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.openVideo, &QAction::triggered, this, &videoMarkup::slot_loadVideo);
}

videoMarkup::~videoMarkup()
{}

void videoMarkup::slot_loadVideo()
{
    QString fileName{ QFileDialog::getOpenFileName(this,tr("Open Video"), "", tr("Video Files (*.avi *.mp4)")) };
    videoFile.open(fileName.toStdString());
    if (videoFile.isOpened())
    {
        ui.widgetFrame->updateFrame(Frame())
    }
}