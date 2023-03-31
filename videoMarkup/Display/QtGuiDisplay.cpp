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
	connect(ui.label_for_TempImg, &myLabel::mouseLeft, this, &QtGuiDisplay::slot_mouseLeft);
	connect(ui.label_for_TempImg, &myLabel::mouseLeftMouveRoi, this, &QtGuiDisplay::slot_mouseLeft);

	setActivFrame(frame);
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
		ui.label_for_TempImg->show_partImg(ui.horSB_forTempImg->value(), ui.verSB_forTempImg->value(), ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
}

void QtGuiDisplay::slot_mouseCurrentPos()
{
	if (!frame.frameIsNull())
	{
		if (event_img)
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
	event_img = true;
}

void QtGuiDisplay::slot_mouseLeft()
{
	cursorType.setShape(Qt::ArrowCursor);
	this->setCursor(cursorType);
}

void QtGuiDisplay::slot_mouseRelease()
{
	event_img = false;
}

void QtGuiDisplay::slot_ZoomImg_In()
{
	if (!isZoomNow)
	{
		isZoomNow = true;
		int dr_x, dr_y;
		activ_scaled = ui.label_for_TempImg->scaledPixmap(1, dr_x, dr_y);
		if (activ_scaled == 500)
		{
			ui.pb_zoomIncress->setEnabled(false);
		}
		else
		{
			ui.pb_zoomIncress->setEnabled(true);
		}
		ui.label_Scale->setText(QString::number(activ_scaled) + "%");
		ui.pb_zoomDeduce->setEnabled(true);
		this->setSizeScrollBar();
		ui.label_for_TempImg->scaledPixmap();
		this->setSizeScrollBar();
		ui.horSB_forTempImg->setValue(dr_x);
		ui.verSB_forTempImg->setValue(dr_y);
		if (!ui.pb_allWindow->isEnabled())
			ui.pb_allWindow->setEnabled(true);
		ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
		isZoomNow = false;
	}
}

void QtGuiDisplay::slot_ZoomImg_Out()
{
	if (!isZoomNow)
	{
		isZoomNow = true;
		ui.horSB_forTempImg->hide();
		ui.verSB_forTempImg->hide();
		int dr_x, dr_y;
		activ_scaled = ui.label_for_TempImg->scaledPixmap(-1, dr_x, dr_y);
		if (activ_scaled == 25)
		{
			ui.pb_zoomDeduce->setEnabled(false);
		}
		else
		{
			ui.pb_zoomDeduce->setEnabled(true);
		}

		ui.label_Scale->setText(QString::number(activ_scaled, 'f', 1) + "%");
		ui.pb_zoomIncress->setEnabled(true);
		this->setSizeScrollBar();
		ui.label_for_TempImg->scaledPixmap();
		this->setSizeScrollBar();
		ui.horSB_forTempImg->setValue(dr_x);
		ui.verSB_forTempImg->setValue(dr_y);
		if (!ui.pb_allWindow->isEnabled())
			ui.pb_allWindow->setEnabled(true);
		ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
		isZoomNow = false;
	}
}

void QtGuiDisplay::slot_ZoomImg_AllLabl()
{	
	if (!isZoomNow)
	{
		isZoomNow = true;
		ui.horSB_forTempImg->hide();
		ui.verSB_forTempImg->hide();
		this->updateGeometry();
		int maxScaled{};
		int minScaled{};
		activ_scaled = ui.label_for_TempImg->scaledPixmap(0, maxScaled, minScaled);
		ui.label_Scale->setText(QString::number(activ_scaled, 'f', 1) + "%");
		maxScaled = ui.label_for_TempImg->getMaxScaled();
		minScaled = ui.label_for_TempImg->getMinScaled();
		
		if (maxScaled < 10)
			ui.pb_zoomIncress->setEnabled(true);
		else
			ui.pb_zoomIncress->setEnabled(false);
		if (minScaled > 0)
			ui.pb_zoomDeduce->setEnabled(true);
		else
			ui.pb_zoomDeduce->setEnabled(false);
		ui.pb_allWindow->setEnabled(false);
		ui.label_for_TempImg->show_partImg(0, 0, ui.label_for_TempImg->width(), ui.label_for_TempImg->height());
		isZoomNow = false;
	}
}

void QtGuiDisplay::setActivFrame(const Frame& activObj)
{
	frame = activObj;
 	ui.label_for_TempImg->setAlignment(Qt::AlignCenter);
	if (frame.frameIsNull())
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
	ui.label_for_TempImg->show_partImg();
}

void QtGuiDisplay::updateFrame(const Frame& activObj)
{
	frame = activObj;
	ui.label_for_TempImg->update_myPixmap(activObj.getPixmap());

	this->setSizeScrollBar();
	ui.label_for_TempImg->show_partImg();
	activ_scaled = ui.label_for_TempImg->get_activScaled();
	ui.label_Scale->setText(QString::number(activ_scaled, 'f', 1) + "%");
	if (!ui.pb_allWindow->isEnabled())
		slot_ZoomImg_AllLabl();
}

void QtGuiDisplay::resizeEvent(QResizeEvent* event)
{
	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	int dr_x{ui.label_for_TempImg->getOldSize().width() - ui.label_for_TempImg->size().width() };
	int dr_y{ ui.label_for_TempImg->getOldSize().height() - ui.label_for_TempImg->size().height() };
	ui.label_for_TempImg->show_partImg(dr_x, dr_y, ui.label_for_TempImg->size().width() - 1, ui.label_for_TempImg->size().height() - 1);
	ui.horSB_forTempImg->setSliderPosition(ui.horSB_forTempImg->value() + dr_x);
	ui.verSB_forTempImg->setSliderPosition(ui.verSB_forTempImg->value() + dr_y);
	ui.label_for_TempImg->getDrawingPoint();
	ui.label_for_TempImg->setAllImgScaled();
	if (!ui.pb_allWindow->isEnabled())
		slot_ZoomImg_AllLabl();
	else
		this->setSizeScrollBar();
}