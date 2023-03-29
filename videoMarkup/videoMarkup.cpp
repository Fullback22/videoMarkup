#include "videoMarkup.h"

videoMarkup::videoMarkup(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.openVideo, &QAction::triggered, this, &videoMarkup::slot_loadVideo);
}

videoMarkup::~videoMarkup()
{
    if (videoFile.isOpened())
    {
        videoFile.release();
    }
}

void videoMarkup::slot_loadVideo()
{
    if (videoFile.isOpened())
    {
        videoFile.release();
    }
    QString fileName{ QFileDialog::getOpenFileName(this,tr("Open Video"), "", tr("Video Files (*.avi *.mp4)")) };
    videoFile.open(fileName.toStdString());
    if (videoFile.isOpened())
    {
        cv::Mat firstFrame{};
        videoFile.read(firstFrame);
        ui.widgetFrame->setActivFrame(Frame(firstFrame));
    }
    else
    {
        int bufer{ QMessageBox::warning(this, "Video warning", "Video not uploaded") };
        ui.widgetFrame->setActivFrame(Frame());
    }
}