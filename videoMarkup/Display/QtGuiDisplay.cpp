#include "QtGuiDisplay.h"

QtGuiDisplay::QtGuiDisplay(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	this->updateGeometry();

	connect(ui.horSB_forTempImg, &QScrollBar::valueChanged, this, &QtGuiDisplay::slot_mouvePixmap);
	connect(ui.verSB_forTempImg, &QScrollBar::valueChanged, this, &QtGuiDisplay::slot_mouvePixmap);
	
	connect(ui.pb_zoomIncress, &QPushButton::clicked, this, &QtGuiDisplay::slot_ZoomImg_In);
	connect(ui.pb_zoomDeduce, &QPushButton::clicked, this, &QtGuiDisplay::slot_ZoomImg_Out);
	connect(ui.pb_allWindow, &QPushButton::clicked, this, &QtGuiDisplay::slot_ZoomImg_AllLabl);

	connect(ui.label_for_TempImg, &myLabel::mousePos, this, &QtGuiDisplay::slot_mouseCurrentPos);
	connect(ui.label_for_TempImg, &myLabel::mousePressed, this, &QtGuiDisplay::slot_mousePressed);
	connect(ui.label_for_TempImg, &myLabel::mouseRelease, this, &QtGuiDisplay::slot_mouseRelease);

	setActivFrame(frame_);
}

QtGuiDisplay::~QtGuiDisplay()
{
}

void QtGuiDisplay::setSizeScrollBar()
{
	QSize scaledImageSize{ ui.label_for_TempImg->getScaledImgSize() };
	if (scaledImageSize.width() > ui.label_for_TempImg->width())
	{
		ui.horSB_forTempImg->show();
		ui.horSB_forTempImg->setRange(0, std::abs(scaledImageSize.width() - ui.label_for_TempImg->width()));
	}
	else
	{
		ui.horSB_forTempImg->hide();
	}
	if (scaledImageSize.height() > ui.label_for_TempImg->height())
	{
		ui.verSB_forTempImg->show();
		ui.verSB_forTempImg->setRange(0, std::abs(scaledImageSize.height() - ui.label_for_TempImg->height()));
	}
	else
	{
		ui.verSB_forTempImg->hide();
	}
}

void QtGuiDisplay::slot_mouvePixmap()
{
	if (ui.pb_allWindow->isEnabled())
		ui.label_for_TempImg->showPartImage();
}

void QtGuiDisplay::slot_mouseCurrentPos()
{
	if (!frame_.frameIsNull())
	{
		if (moveImage)
		{
			int dr_x, dr_y;
			ui.label_for_TempImg->moveImg(dr_x, dr_y);
			ui.horSB_forTempImg->setSliderPosition(dr_x);
			ui.verSB_forTempImg->setSliderPosition(dr_y);
		}
	}
}

void QtGuiDisplay::slot_mousePressed()
{
	moveImage = true;
}

void QtGuiDisplay::slot_mouseRelease()
{
	moveImage = false;
}

void QtGuiDisplay::slot_ZoomImg_In()
{
	if (!isZoomNow_)
	{
		isZoomNow_ = true;
		int dr_x, dr_y;
		ui.label_for_TempImg->setImageScale(1);
		if (activScaled_ == 500)
		{
			ui.pb_zoomIncress->setEnabled(false);
		}
		else
		{
			ui.pb_zoomIncress->setEnabled(true);
		}
		ui.label_Scale->setText(QString::number(activScaled_) + "%");
		ui.pb_zoomDeduce->setEnabled(true);
		this->setSizeScrollBar();
		ui.label_for_TempImg->setImageScale(1);
		this->setSizeScrollBar();
		ui.horSB_forTempImg->setValue(dr_x);
		ui.verSB_forTempImg->setValue(dr_y);
		if (!ui.pb_allWindow->isEnabled())
			ui.pb_allWindow->setEnabled(true);
		ui.label_for_TempImg->showPartImage();
		isZoomNow_ = false;
	}
}

void QtGuiDisplay::slot_ZoomImg_Out()
{
	if (!isZoomNow_)
	{
		isZoomNow_ = true;
		ui.horSB_forTempImg->hide();
		ui.verSB_forTempImg->hide();
		int dr_x, dr_y;
		ui.label_for_TempImg->setImageScale(-1);
		if (activScaled_ == 25)
		{
			ui.pb_zoomDeduce->setEnabled(false);
		}
		else
		{
			ui.pb_zoomDeduce->setEnabled(true);
		}

		ui.label_Scale->setText(QString::number(activScaled_, 'f', 1) + "%");
		ui.pb_zoomIncress->setEnabled(true);
		this->setSizeScrollBar();
		ui.label_for_TempImg->setImageScale(1);
		this->setSizeScrollBar();
		ui.horSB_forTempImg->setValue(dr_x);
		ui.verSB_forTempImg->setValue(dr_y);
		if (!ui.pb_allWindow->isEnabled())
			ui.pb_allWindow->setEnabled(true);
		ui.label_for_TempImg->showPartImage();
		isZoomNow_ = false;
	}
}

void QtGuiDisplay::slot_ZoomImg_AllLabl()
{	
	if (!isZoomNow_)
	{
		isZoomNow_ = true;
		ui.horSB_forTempImg->hide();
		ui.verSB_forTempImg->hide();
		this->updateGeometry();
		int maxScaled{};
		int minScaled{};
		ui.label_for_TempImg->setImageScale(0);
		ui.label_Scale->setText(QString::number(activScaled_, 'f', 1) + "%");
		//maxScaled = ui.label_for_TempImg->getMaxScaled();
		//minScaled = ui.label_for_TempImg->getMinScaled();
		
		if (maxScaled < 10)
			ui.pb_zoomIncress->setEnabled(true);
		else
			ui.pb_zoomIncress->setEnabled(false);
		if (minScaled > 0)
			ui.pb_zoomDeduce->setEnabled(true);
		else
			ui.pb_zoomDeduce->setEnabled(false);
		ui.pb_allWindow->setEnabled(false);
		ui.label_for_TempImg->showPartImage();
		isZoomNow_ = false;
	}
}

void QtGuiDisplay::setActivFrame(const Frame& activObj)
{
	frame_ = activObj;
 	ui.label_for_TempImg->setAlignment(Qt::AlignCenter);
	if (frame_.frameIsNull())
	{
		ui.label_for_TempImg->setImage(QPixmap("Display/NoImg.png"));
		ui.pb_zoomDeduce->hide();
		ui.pb_zoomIncress->hide();
		ui.pb_allWindow->hide();
		ui.label_Scale->hide();
	}
	else
	{
		ui.label_for_TempImg->setImage(activObj.getPixmap());
		ui.pb_zoomDeduce->show();
		ui.pb_zoomIncress->show();
		ui.pb_allWindow->show();
		ui.label_Scale->show();
	}
	slot_ZoomImg_AllLabl();
}

void QtGuiDisplay::setEnableWidtsGrouBox(bool enable)
{
	if (enable)
	{
		ui.pb_zoomDeduce->show();
		ui.pb_zoomIncress->show();
		ui.pb_allWindow->show();
		ui.label_Scale->show();
	}
	else
	{
		ui.pb_zoomDeduce->hide();
		ui.pb_zoomIncress->hide();
		ui.pb_allWindow->hide();
		ui.label_Scale->hide();
	}
}

QRect QtGuiDisplay::getLabelRect()
{
	QRect bufer;
	bufer.setX(this->x() + ui.label_for_TempImg->x());
	bufer.setY(this->y() + ui.label_for_TempImg->y());
	bufer.setWidth(ui.label_for_TempImg->rect().width());
	bufer.setHeight(ui.label_for_TempImg->rect().height());

	return bufer;
}

void QtGuiDisplay::changeImgFormat(ImageFormat formatType)
{
	ui.label_for_TempImg->formatImage(formatType);
}

void QtGuiDisplay::slot_updateImg()
{
	ui.label_for_TempImg->showPartImage();
}

void QtGuiDisplay::updateFrame(const Frame& activObj)
{
	frame_ = activObj;
	ui.label_for_TempImg->updateImage(activObj.getPixmap());

	this->setSizeScrollBar();
	ui.label_for_TempImg->showPartImage();
	activScaled_ = ui.label_for_TempImg->getImageScaled();
	ui.label_Scale->setText(QString::number(activScaled_, 'f', 1) + "%");
	if (!ui.pb_allWindow->isEnabled())
		slot_ZoomImg_AllLabl();
}

void QtGuiDisplay::resizeEvent(QResizeEvent* event)
{
	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	int dr_x{ui.label_for_TempImg->getOldSize().width() - ui.label_for_TempImg->size().width() };
	int dr_y{ ui.label_for_TempImg->getOldSize().height() - ui.label_for_TempImg->size().height() };
	ui.label_for_TempImg->showPartImage();
	ui.horSB_forTempImg->setSliderPosition(ui.horSB_forTempImg->value() + dr_x);
	ui.verSB_forTempImg->setSliderPosition(ui.verSB_forTempImg->value() + dr_y);
	ui.label_for_TempImg->getDrawingPoint();
	if (!ui.pb_allWindow->isEnabled())
		slot_ZoomImg_AllLabl();
	else
		this->setSizeScrollBar();
}