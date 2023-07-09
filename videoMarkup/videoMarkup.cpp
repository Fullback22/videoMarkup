#include "videoMarkup.h"

videoMarkup::videoMarkup(QWidget *parent)
    : QMainWindow{ parent },
    markupObjects_{ 0 }
{
    ui.setupUi(this);
    
    ui.widgetFrame->setActivFrame(Frame());

    connect(ui.openVideo, &QAction::triggered, this, &videoMarkup::slot_loadVideo);

    connect(ui.createActionClassifier, &QAction::triggered, this, &videoMarkup::slot_createActionsClassifier);
    connect(ui.editActionClassifier, &QAction::triggered, this, &videoMarkup::slot_editActionsClassifire);
    connect(ui.saveActionClassifier, &QAction::triggered, this, &videoMarkup::slot_saveActionsClassifire);
    connect(ui.openActionClassifier, &QAction::triggered, this, &videoMarkup::slot_loadActionsClassifire);

    connect(ui.createObjectClassifier, &QAction::triggered, this, &videoMarkup::slot_createObjectClassifier);
    connect(ui.editObjectClassifier, &QAction::triggered, this, &videoMarkup::slot_editObjectClassifier);
    connect(ui.saveObjectClassifier, &QAction::triggered, this, &videoMarkup::slot_saveObjectClassifier);
    connect(ui.openObjectClassifier, &QAction::triggered, this, &videoMarkup::slot_loadObjectClassifier);

    connect(ui.createMarkup, &QAction::triggered, this, &videoMarkup::slot_createMarkup);
    connect(ui.openMarkup, &QAction::triggered, this, &videoMarkup::slot_openMarkup);

    connect(ui.pb_nextFrame, &QPushButton::clicked, this, &videoMarkup::slot_nextFrame);

    connect(ui.pb_addObject, &QPushButton::clicked, this, &videoMarkup::slot_addMarkupObject);
    connect(ui.pb_deleteObject, &QPushButton::clicked, this, &videoMarkup::slot_removeActivMarkupObject);
    connect(ui.cb_objectNumber, qOverload<int>(&QComboBox::activated), this, &videoMarkup::slot_changeActivMarkupObject);
    connect(ui.widgetFrame, &QtGuiDisplay::newActivFigure, this, &videoMarkup::slot_setActivMarkupObject);

    ///del
    objectClassifier_.addValue("Объект1");
    objectClassifier_.addValue("Объект2");
    actionClassifier_.addValue("Действие1");
    actionClassifier_.addValue("Действие2");
    updateComboBoxAction();
    updateComboBoxObject();
    
    //setAttribute(Qt::WA_DeleteOnClose);
}

videoMarkup::~videoMarkup()
{
    if (videoFile_.isOpened())
        videoFile_.release();
    if (outMarkupFilename_.is_open())
        outMarkupFilename_.close();
    for (auto& object : markupObjects_)
        object.position->Delete();
}

void videoMarkup::setActivFrameNumberToForm()
{
    ui.label_activFrameNumber->setText(QString::number(activFrameNumber_ + 1));
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
    updatePb_AddObjectStatus();
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
    updatePb_AddObjectStatus();
}

void videoMarkup::updatePb_NextFrameStatus()
{
    if (outMarkupFilename_.is_open() && videoFile_.isOpened())
        ui.pb_nextFrame->setEnabled(true);
    else
        ui.pb_nextFrame->setDisabled(true);
}

void videoMarkup::writeCoordinateMarkupObjectToFile()
{
    for (const auto& object : markupObjects_)
    {
        outMarkupFilename_ << activFrameNumber_ << "\t";
        outMarkupFilename_ << object.objectClass << "\t";
        outMarkupFilename_ << object.activClass << "\t";
        outMarkupFilename_ << object.position->getX() << "\t" << object.position->getY() << "\t" << object.position->getWidth() << "\t" << object.position->getHeidth();
        outMarkupFilename_ << std::endl;
    }
}

void videoMarkup::readLastFrameFromMarkupFile(const std::string& filename)
{
    std::ifstream inputFile{ filename, std::ios::in };
    std::string bufer;
    int lastFrame{ -1 };
    while (!inputFile.eof()) //подсчёт количества строк
    {
        std::getline(inputFile, bufer);//чтение строки из файла
        if (bufer != "")
        {
            std::istringstream iss(bufer);
            std::string token;
            iss >> lastFrame;
        }
        else
            break;
    }
    inputFile.close();

    activFrameNumber_ = lastFrame;
    videoFile_.set(cv::CAP_PROP_POS_FRAMES, activFrameNumber_);
    slot_nextFrame();
    outMarkupFilename_.open(filename, std::ofstream::app);
    updatePb_NextFrameStatus();
}

void videoMarkup::updatePb_AddObjectStatus()
{
    size_t quantityObjectsType{ objectClassifier_.size() };
    size_t quantityActionsType{ actionClassifier_.size() };
    if (videoFile_.isOpened() && quantityObjectsType > 0 && quantityActionsType > 0)
        ui.pb_addObject->setEnabled(true);
    else
        ui.pb_addObject->setDisabled(true);
}

void videoMarkup::slot_nextFrame()
{
    if (videoFile_.isOpened())
    {
        writeCoordinateMarkupObjectToFile();
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

void videoMarkup::slot_addMarkupObject()
{
    activMarkupObject_ = markupObjects_.size();
    markupObjects_.push_back(MarkupObject());
    markupObjects_[activMarkupObject_].activClass = ui.cb_newObjectAction->currentIndex();
    markupObjects_[activMarkupObject_].objectClass = ui.cb_newObjectClass->currentIndex();

    ui.cb_objectNumber->addItem(QString::number(activMarkupObject_));
    ui.widgetFrame->addRectangel(markupObjects_[activMarkupObject_].position);
    ui.widgetFrame->setActivFigure(activMarkupObject_);

    ui.cb_objectNumber->setCurrentIndex(activMarkupObject_);
    emit ui.cb_objectNumber->activated(activMarkupObject_);
    if (activMarkupObject_ == 0)
        ui.pb_deleteObject->setEnabled(true);
}

void videoMarkup::slot_removeActivMarkupObject()
{
    markupObjects_[activMarkupObject_].position->Delete();
    markupObjects_.erase(markupObjects_.begin() + activMarkupObject_);
    ui.widgetFrame->deleteFigure(activMarkupObject_);
    ui.cb_objectNumber->removeItem(ui.cb_objectNumber->count() - 1);

    if (activMarkupObject_ == markupObjects_.size() && activMarkupObject_ > 0)
    {
        --activMarkupObject_;
        ui.widgetFrame->setActivFigure(activMarkupObject_);
    }

    if (markupObjects_.size() == 0)
    {
        ui.cb_objectNumber->clear();
        ui.pb_deleteObject->setDisabled(true);
        activMarkupObject_ = -1;
    }
    ui.cb_objectNumber->setCurrentIndex(activMarkupObject_);
    emit ui.cb_objectNumber->activated(activMarkupObject_);
    ui.widgetFrame->updateImage();
}

void videoMarkup::slot_changeActivMarkupObject(int index)
{
    if (index < markupObjects_.size())
    {
        activMarkupObject_ = index;
        ui.widgetFrame->setActivFigure(index);
        ui.cb_objectClass->setCurrentIndex(markupObjects_[index].objectClass);
        ui.cb_odjectAction->setCurrentIndex(markupObjects_[index].activClass);
    }
}

void videoMarkup::slot_createActionsClassifier()
{
    classifierInterfase = new QtGuiClassifier{ QString::fromLocal8Bit("Действие") };
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
    classifierInterfase = new QtGuiClassifier{ QString::fromLocal8Bit("Действие") };
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
    classifierInterfase = new QtGuiClassifier{ QString::fromLocal8Bit("Объект") };
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
    classifierInterfase = new QtGuiClassifier{ QString::fromLocal8Bit("Объект") };
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

void videoMarkup::slot_createMarkup()
{
    QString fileName{ QFileDialog::getSaveFileName(this, tr("Create Markup File"), "", tr("Markup Files (*.txt)")) };
    if (!fileName.isEmpty())
    {
        outMarkupFilename_.open(fileName.toLocal8Bit().constData(), std::ofstream::out | std::ofstream::trunc);
        updatePb_NextFrameStatus();
    }
}

void videoMarkup::slot_openMarkup()
{
    QString fileName{ QFileDialog::getOpenFileName(this,tr("Open Markup File"), "", tr("Markup Files (*.txt)")) };
    if (!fileName.isEmpty())
    {
        readLastFrameFromMarkupFile(fileName.toLocal8Bit().constData());
    }
}

void videoMarkup::slot_setActivMarkupObject(int index)
{
    ui.cb_objectNumber->setCurrentIndex(index);
    emit ui.cb_objectNumber->activated(index);
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
            ui.markupMenu->setEnabled(true);
        }
        else
        {
            int bufer{ QMessageBox::warning(this, "Video warning", "Video not uploaded") };
            ui.widgetFrame->setActivFrame(Frame());
            ui.markupMenu->setDisabled(true);
        }
        setActivFrameNumberToForm();
    }
    updatePb_AddObjectStatus();
    updatePb_NextFrameStatus();
}