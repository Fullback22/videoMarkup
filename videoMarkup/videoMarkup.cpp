#include "videoMarkup.h"

videoMarkup::videoMarkup(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.openVideo, &QAction::triggered, this, &videoMarkup::slot_loadVideo);
    connect(ui.pb_nextFrame, &QPushButton::clicked, this, &videoMarkup::slot_nextFrame);
    ui.widgetFrame->setActivFrame(Frame());
}

videoMarkup::~videoMarkup()
{
    if (videoFile_.isOpened())
        videoFile_.release();
}

void videoMarkup::setActivFrameNumberToForm()
{
    ui.le_activFrameNumber->setReadOnly(false);
    ui.le_activFrameNumber->setText(QString::number(activFrameNumber_ + 1));
    ui.le_activFrameNumber->setReadOnly(true);
}

void videoMarkup::slot_nextFrame()
{
    if (videoFile_.isOpened())
    {
        cv::Mat frame{};
        videoFile_.read(frame);
        if (frame.empty())
        {
            ui.pb_nextFrame->setDisabled(true);
        }
        else
        {
            ui.widgetFrame->updateFrame(Frame(frame));
            ++activFrameNumber_;
        }
    }
    else
    {
        int bufer{ QMessageBox::warning(this, "Video warning", "Video not uploaded") };
        ui.widgetFrame->setActivFrame(Frame());
        ui.pb_nextFrame->setDisabled(true);
        activFrameNumber_ = 0;
    }
    setActivFrameNumberToForm();
}

void videoMarkup::slot_loadVideo()
{
    QString fileName{ QFileDialog::getOpenFileName(this,tr("Open Video"), "", tr("Video Files (*.avi *.mp4)")) };
    if (!fileName.isEmpty())
    {
        if (videoFile_.isOpened())
        {
            videoFile_.release();
        }
        videoFile_.open(fileName.toStdString());
        activFrameNumber_ = 0;
        if (videoFile_.isOpened())
        {
            cv::Mat firstFrame{};
            videoFile_.read(firstFrame);
            ui.widgetFrame->setActivFrame(Frame(firstFrame));
            ui.pb_nextFrame->setEnabled(true);
        }
        else
        {
            int bufer{ QMessageBox::warning(this, "Video warning", "Video not uploaded") };
            ui.widgetFrame->setActivFrame(Frame());
            ui.pb_nextFrame->setDisabled(true);
        }
        setActivFrameNumberToForm();
    }
}