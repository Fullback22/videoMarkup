#include "videoMarkup.h"

videoMarkup::videoMarkup(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.openVideo, &QAction::triggered, this, &videoMarkup::slot_loadVideo);

    connect(ui.createActionClassifier, &QAction::triggered, this, &videoMarkup::slot_createActionsClassifier);
    connect(ui.editActionClassifier, &QAction::triggered, this, &videoMarkup::slot_editActionsClassifire);
    connect(ui.saveActionClassifier, &QAction::triggered, this, &videoMarkup::slot_saveActionsClassifire);
    connect(ui.openActionClassifier, &QAction::triggered, this, &videoMarkup::slot_loadActionsClassifire);

    connect(ui.createObjectClassifier, &QAction::triggered, this, &videoMarkup::slot_createObjectClassifier);
    connect(ui.editObjectClassifier, &QAction::triggered, this, &videoMarkup::slot_editObjectClassifier);
    connect(ui.saveObjectClassifier, &QAction::triggered, this, &videoMarkup::slot_saveObjectClassifier);
    connect(ui.openObjectClassifier, &QAction::triggered, this, &videoMarkup::slot_loadObjectClassifier);

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

void videoMarkup::updateComboBoxAction()
{
    ui.cb_newObjectAction->clear();
    ui.cb_odjectAction->clear();
    for (size_t i{}; i < actionClassifier_.size(); ++i)
    {
        ui.cb_newObjectAction->addItem(QString::fromLocal8Bit(actionClassifier_[i].c_str()));
        ui.cb_odjectAction->addItem(QString::fromLocal8Bit(actionClassifier_[i].c_str()));
    }
}

void videoMarkup::updateComboBoxObject()
{
    ui.cb_newObjectClass->clear();
    ui.cb_objectClass->clear();
    for (size_t i{}; i < objectClassifier_.size(); ++i)
    {
        ui.cb_newObjectClass->addItem(QString::fromLocal8Bit(objectClassifier_[i].c_str()));
        ui.cb_objectClass->addItem(QString::fromLocal8Bit(objectClassifier_[i].c_str()));
    }
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

void videoMarkup::slot_createActionsClassifier()
{
    classifierInterfase = new QtGuiClassifier{ QString::fromLocal8Bit("��������") };
    connect(classifierInterfase, &QtGuiClassifier::readyToUpdateClassifier, this, &videoMarkup::slot_updateActionsClassifier);
    classifierInterfase->exec();
    delete classifierInterfase;
    classifierInterfase = nullptr;
    updateComboBoxAction();
}

void videoMarkup::slot_updateActionsClassifier()
{
    classifierInterfase->updateClassifier(actionClassifier_);
}

void videoMarkup::slot_editActionsClassifire()
{
    classifierInterfase = new QtGuiClassifier{ QString::fromLocal8Bit("��������") };
    connect(classifierInterfase, &QtGuiClassifier::readyToUpdateClassifier, this, &videoMarkup::slot_updateActionsClassifier);
    classifierInterfase->setClassifier(actionClassifier_);
    classifierInterfase->exec();
    delete classifierInterfase;
    classifierInterfase = nullptr;
    updateComboBoxAction();
}

void videoMarkup::slot_saveActionsClassifire()
{
    QString fileName{ QFileDialog::getSaveFileName(this, tr("Save Action Classifier"), "", tr("Classifier Files (*.txt)")) };
    if (!fileName.isEmpty())
    {
        actionClassifier_.getClassifier(fileName.toLocal8Bit().constData());
    }
}

void videoMarkup::slot_loadActionsClassifire()
{
    QString fileName{ QFileDialog::getOpenFileName(this,tr("Open Action Classifier"), "", tr("Classifier Files (*.txt)")) };
    if (!fileName.isEmpty())
    {
        actionClassifier_.setClassifier(fileName.toLocal8Bit().constData());
        updateComboBoxAction();
    }
}

void videoMarkup::slot_createObjectClassifier()
{
    classifierInterfase = new QtGuiClassifier{ QString::fromLocal8Bit("������") };
    connect(classifierInterfase, &QtGuiClassifier::readyToUpdateClassifier, this, &videoMarkup::slot_updateObjectClassifier);
    classifierInterfase->exec();
    delete classifierInterfase;
    classifierInterfase = nullptr;
    updateComboBoxObject();
}

void videoMarkup::slot_updateObjectClassifier()
{
    classifierInterfase->updateClassifier(objectClassifier_);
}

void videoMarkup::slot_editObjectClassifier()
{
    classifierInterfase = new QtGuiClassifier{ QString::fromLocal8Bit("������") };
    connect(classifierInterfase, &QtGuiClassifier::readyToUpdateClassifier, this, &videoMarkup::slot_updateObjectClassifier);
    classifierInterfase->setClassifier(objectClassifier_);
    classifierInterfase->exec();
    delete classifierInterfase;
    classifierInterfase = nullptr;
    updateComboBoxObject();
}

void videoMarkup::slot_saveObjectClassifier()
{
    QString fileName{ QFileDialog::getSaveFileName(this, tr("Save Object Classifier"), "", tr("Classifier Files (*.txt)")) };
    if (!fileName.isEmpty())
    {
        objectClassifier_.getClassifier(fileName.toLocal8Bit().constData());
    }
}

void videoMarkup::slot_loadObjectClassifier()
{
    QString fileName{ QFileDialog::getOpenFileName(this,tr("Open Object Classifier"), "", tr("Classifier Files (*.txt)")) };
    if (!fileName.isEmpty())
    {
        objectClassifier_.setClassifier(fileName.toLocal8Bit().constData());
        updateComboBoxObject();
    }
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