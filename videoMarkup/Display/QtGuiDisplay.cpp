#include "QtGuiDisplay.h"

QtGuiDisplay::QtGuiDisplay(QWidget *parent): 
	QWidget(parent),
	figuresForDrawing_(0)
{
	ui.setupUi(this);

	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	updateGeometry();

	connect(ui.horSB_forTempImg, &QScrollBar::sliderMoved, this, &QtGuiDisplay::slot_mouvePixmap);
	connect(ui.verSB_forTempImg, &QScrollBar::sliderMoved, this, &QtGuiDisplay::slot_mouvePixmap);
	
	connect(ui.pb_zoomIncress, &QPushButton::clicked, this, &QtGuiDisplay::slot_ZoomImg_In);
	connect(ui.pb_zoomDeduce, &QPushButton::clicked, this, &QtGuiDisplay::slot_ZoomImg_Out);
	connect(ui.pb_allWindow, &QPushButton::clicked, this, &QtGuiDisplay::slot_ZoomImg_AllLabl);

	connect(ui.label_for_TempImg, &myLabel::mousePos, this, &QtGuiDisplay::slot_mouseCurrentPos);
	connect(ui.label_for_TempImg, &myLabel::mousePressed, this, &QtGuiDisplay::slot_mousePressed);
	connect(ui.label_for_TempImg, &myLabel::mouseRelease, this, &QtGuiDisplay::slot_mouseRelease);

	setAttribute(Qt::WA_DeleteOnClose);
}

QtGuiDisplay::~QtGuiDisplay()
{
}

void QtGuiDisplay::slot_mouvePixmap()
{
	QPoint drawingPoint{ ui.horSB_forTempImg->value(), ui.verSB_forTempImg->value() };
	ui.label_for_TempImg->setDrawingPoint(drawingPoint);
	ui.label_for_TempImg->showPartImage();
}

void QtGuiDisplay::slot_mouseCurrentPos()
{
	if (!frame_.frameIsNull())
	{
		if (canChangesFigure_)
			preparationForModifyFigure();
		else if (figureIsChanging_)
			modifyFigure();
		else if (canMoveImage_)
			moveImage();
	}
}

void QtGuiDisplay::slot_mousePressed()
{
	canChangesFigure_ = false;
	QPoint cursorPosition{ ui.label_for_TempImg->getCursorPositionOnImage() };
	if (cursorOnDisplay_.shape() == Qt::ArrowCursor)
	{
		QSize imageSize(ui.label_for_TempImg->getOriginalImageSize());
		if (cursorPosition.x() > 0 && cursorPosition.y() > 0 && cursorPosition.x() < imageSize.width() && cursorPosition.y() < imageSize.height())
		{
			setCursor(Qt::ClosedHandCursor);
			canMoveImage_ = true;
		}
	}
	else if(cursorOnDisplay_.shape() == Qt::SizeVerCursor || cursorOnDisplay_.shape() == Qt::SizeHorCursor ||
			cursorOnDisplay_.shape() == Qt::SizeBDiagCursor || cursorOnDisplay_.shape() == Qt::SizeFDiagCursor || cursorOnDisplay_.shape() == Qt::SizeAllCursor)
	{
		setActivFigure(prepareForModifyFigure_);
		figuresForDrawing_[prepareForModifyFigure_]->prepareForModify(cursorPosition);
		emit newActivFigure(activFigure_);
		figureIsChanging_ = true;
	}
}

void QtGuiDisplay::slot_mouseRelease()
{
	setCursor(Qt::ArrowCursor);
	canMoveImage_ = false;
	canChangesFigure_ = true;
	figureIsChanging_ = false;
}

void QtGuiDisplay::slot_ZoomImg_In()
{
	if (activScale_ == 0)
		activScale_ = scaleFollowingNormalScale_;
	else
		++activScale_;
	if (activScale_ > ui.label_for_TempImg->getCoefficientRecalculationByWidth())
		ui.horSB_forTempImg->show();
	if (activScale_ > ui.label_for_TempImg->getCoefficientRecalculationByHeigth())
		ui.verSB_forTempImg->show();

	ui.label_for_TempImg->setImageScale(scale_[activScale_]);
	setSizeScrollBar();

	QPoint drawingPoint{ ui.label_for_TempImg->getDrawingPoint() };
	ui.horSB_forTempImg->setValue(drawingPoint.x() );
	ui.verSB_forTempImg->setValue(drawingPoint.y() );

	ui.label_for_TempImg->showPartImage();

	if (activScale_ == 9)
		ui.pb_zoomIncress->setDisabled(true);
	ui.pb_zoomDeduce->setEnabled(true);
	ui.pb_allWindow->setEnabled(true);

	float scaleInPercent{ scale_[activScale_] * 100 };
	ui.label_Scale->setText(QString::number(scaleInPercent) + "%");
}

void QtGuiDisplay::slot_ZoomImg_Out()
{
	if (activScale_ == 0)
		activScale_ = scalePreviousToNormalScale_;
	else
		--activScale_;

	if (activScale_ <= ui.label_for_TempImg->getCoefficientRecalculationByWidth())
		ui.horSB_forTempImg->show();
	if (activScale_ <= ui.label_for_TempImg->getCoefficientRecalculationByHeigth())
		ui.verSB_forTempImg->show();

	ui.label_for_TempImg->setImageScale(scale_[activScale_]);
	setSizeScrollBar();

	QPoint drawingPoint{ ui.label_for_TempImg->getDrawingPoint() };
	ui.horSB_forTempImg->setValue(drawingPoint.x() );
	ui.verSB_forTempImg->setValue(drawingPoint.y() );
	ui.label_for_TempImg->showPartImage();

	if (activScale_ == 1)
		ui.pb_zoomDeduce->setDisabled(true);
	ui.pb_zoomIncress->setEnabled(true);
	ui.pb_allWindow->setEnabled(true);

	float scaleInPercent{ scale_[activScale_] * 100 };
	ui.label_Scale->setText(QString::number(scaleInPercent) + "%");
}

void QtGuiDisplay::slot_ZoomImg_AllLabl()
{
	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	updateGeometry();
	activScale_ = 0;
	ui.label_for_TempImg->setImageScale(scale_[activScale_]);
	float scaleInPercent{ scale_[activScale_] * 100 };
	ui.label_Scale->setText(QString::number(scaleInPercent, 'f', 1) + "%");

	if (scaleFollowingNormalScale_ < 10)
		ui.pb_zoomIncress->setEnabled(true);
	else
		ui.pb_zoomIncress->setDisabled(true);

	if (scalePreviousToNormalScale_ > 0)
		ui.pb_zoomDeduce->setEnabled(true);
	else
		ui.pb_zoomDeduce->setDisabled(true);

	ui.pb_allWindow->setEnabled(false);
	ui.label_for_TempImg->showPartImage();
}

void QtGuiDisplay::setActivFrame(const Frame& activObj)
{
	frame_ = activObj;
 	ui.label_for_TempImg->setAlignment(Qt::AlignCenter);
	if (frame_.frameIsNull())
	{
		ui.label_for_TempImg->setImage(QPixmap("Display/NoImg.png"));
		setEnableWidtsGrouBox(false);
	}
	else
	{
		ui.label_for_TempImg->setImage(activObj.getPixmap());
		setEnableWidtsGrouBox(true);
	}
	scale_[0] = ui.label_for_TempImg->getNormalImageScale();
	setScalesBorderingWithNormalScale();
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

void QtGuiDisplay::addRectangel(FigureRectangle* rectangel)
{
	int w{ 0 };
	int h{ 0 };
	QSize scaledImageSize{ ui.label_for_TempImg->getScaledImageSize() };
	QSize labelSize{ ui.label_for_TempImg->size() };
	if (scaledImageSize.width() <= labelSize.width())
		w = scaledImageSize.width() / 4;
	else
		w = labelSize.width() / 4 / scale_[activScale_];

	if (scaledImageSize.height() <= labelSize.height())
		h = scaledImageSize.height() / 4;
	else
		h = labelSize.height() / 4 / scale_[activScale_];

	QPoint drawingRectangelPoint{ labelSize.width() / 2,  labelSize.height() / 2 };
	ui.label_for_TempImg->convertPointToImageCoordinate(drawingRectangelPoint);

	rectangel->setRectangle(drawingRectangelPoint.x() - w / 2, drawingRectangelPoint.y() - h / 2, w, h);
	figuresForDrawing_.push_back(static_cast<IFigure*>(rectangel));
}

QRect QtGuiDisplay::getLabelRect()
{
	QRect bufer;
	bufer.setX(x() + ui.label_for_TempImg->x());
	bufer.setY(y() + ui.label_for_TempImg->y());
	bufer.setWidth(ui.label_for_TempImg->rect().width());
	bufer.setHeight(ui.label_for_TempImg->rect().height());

	return bufer;
}

void QtGuiDisplay::changeImgFormat(ImageFormat formatType)
{
	ui.label_for_TempImg->setFormatImage(formatType);
}

void QtGuiDisplay::updateImage()
{
	drawAllFigure();
	ui.label_for_TempImg->showPartImage();
}

void QtGuiDisplay::updateFrame(const Frame& activObj)
{
	frame_ = activObj;
	ui.label_for_TempImg->updateImage(activObj.getPixmap());
	scale_[0] = ui.label_for_TempImg->getNormalImageScale();
	
	setSizeScrollBar();
	drawAllFigure();
	ui.label_for_TempImg->showPartImage();
	if (!ui.pb_allWindow->isEnabled())
		slot_ZoomImg_AllLabl();
	if (frame_.frameIsNull())
		canChangesFigure_ = false;
}

void QtGuiDisplay::resizeEvent(QResizeEvent* event)
{
	ui.horSB_forTempImg->hide();
	ui.verSB_forTempImg->hide();
	scale_[0] = ui.label_for_TempImg->getNormalImageScale();
	ui.label_for_TempImg->showPartImage();
	setScalesBorderingWithNormalScale();
	QPoint drawingPoint{ ui.label_for_TempImg->getDrawingPoint() };
	ui.horSB_forTempImg->setSliderPosition(drawingPoint.x());
	ui.verSB_forTempImg->setSliderPosition(drawingPoint.y());

	ui.label_for_TempImg->getDrawingPoint();
	if (!ui.pb_allWindow->isEnabled())
		slot_ZoomImg_AllLabl();
	else
		setSizeScrollBar();
}

void QtGuiDisplay::setActivFigure(unsigned int const index)
{
	if (index < figuresForDrawing_.size())
	{
		activFigure_ = index;
		drawAllFigure();
		ui.label_for_TempImg->showPartImage();
	}
}

void QtGuiDisplay::deleteFigure(int const index)
{
	if (index < figuresForDrawing_.size())
		figuresForDrawing_.erase(figuresForDrawing_.begin() + index);
	if (index == activFigure_)
		activFigure_ = 0;
	if (figuresForDrawing_.size() == 0)
		activFigure_ = -1;
}

void QtGuiDisplay::setScalesBorderingWithNormalScale()
{
	float normalImageScale{ ui.label_for_TempImg->getNormalImageScale() };
	scaleFollowingNormalScale_ = 10;
	for (size_t i{ 1 }; i < 10; i++)
	{
		if (scale_[i] > normalImageScale)
		{
			scaleFollowingNormalScale_ = i;
			i = 9;
		}
	}
	scalePreviousToNormalScale_ = 0;
	for (int i{ 9 }; i > 0; i--)
	{
		if (scale_[i] < normalImageScale)
		{
			scalePreviousToNormalScale_ = i;
			i = 0;
		}
	}
}

void QtGuiDisplay::setSizeScrollBar()
{
	QSize originImageSize{ ui.label_for_TempImg->getOriginalImageSize() };
	QSize drawingSize{ ui.label_for_TempImg->getDrawingSize() };
	if (originImageSize.width() > drawingSize.width())
	{
		ui.horSB_forTempImg->show();
		ui.horSB_forTempImg->setRange(0, originImageSize.width() - drawingSize.width());
	}
	else
	{
		ui.horSB_forTempImg->hide();
	}
	if (originImageSize.height() > drawingSize.height())
	{
		ui.verSB_forTempImg->show();
		ui.verSB_forTempImg->setRange(0, originImageSize.height() - drawingSize.height());
	}
	else
	{
		ui.verSB_forTempImg->hide();
	}
}

Qt::CursorShape QtGuiDisplay::determenateCursorViewOnFigure(const IFigure* figure, const QPoint& position)
{
	Qt::CursorShape cursorView{ Qt::ArrowCursor };
	
	if (figure->contains(position))
		cursorView = Qt::SizeAllCursor;

	if (figure->isUpperCenterBorder(position) || figure->isBottomCenterBorder(position))
		cursorView = Qt::SizeVerCursor;
	else if (figure->isRightCenterBorder(position) || figure->isLeftCenterBorder(position))
		cursorView = Qt::SizeHorCursor;
	else if (figure->isUpperRightBorder(position) || figure->isBottomLeftBorder(position))
		cursorView = Qt::SizeBDiagCursor;
	else if (figure->isBottomRightBorder(position) || figure->isUpperLeftBorder(position))
		cursorView = Qt::SizeFDiagCursor;
	
	return cursorView;
}

int QtGuiDisplay::getFigureIndexPrepareForModify(const QPoint& position)
{
	int smallestFigureIndex{ -1 };
	for (int i{}; i < figuresForDrawing_.size(); ++i)
	{
		Qt::CursorShape newShape{};
		if (smallestFigureIndex == -1)
		{
			if (figuresForDrawing_[i]->containsToModifyArea(position))
				smallestFigureIndex = i;
		}
		else if (figuresForDrawing_[i]->getArea() <= figuresForDrawing_[smallestFigureIndex]->getArea())
		{
			if (figuresForDrawing_[i]->containsToModifyArea(position))
				smallestFigureIndex = i;
		}
	}
	return smallestFigureIndex;
}

void QtGuiDisplay::modifyFigure()
{
	QSize imageSize(ui.label_for_TempImg->getOriginalImageSize());
	if (cursorOnDisplay_.shape() == Qt::SizeVerCursor || cursorOnDisplay_.shape() == Qt::SizeHorCursor ||
		cursorOnDisplay_.shape() == Qt::SizeBDiagCursor || cursorOnDisplay_.shape() == Qt::SizeFDiagCursor)
	{
		figuresForDrawing_[activFigure_]->resize(ui.label_for_TempImg->getCursorPositionOnImage(), imageSize);
	}
	else if (cursorOnDisplay_.shape() == Qt::SizeAllCursor)
	{
		figuresForDrawing_[activFigure_]->move(ui.label_for_TempImg->getDeltaOnImageCoordinate(), imageSize);
	}
	ui.label_for_TempImg->drawDynamicFigure(figuresForDrawing_[activFigure_]);
	ui.label_for_TempImg->showPartImage();
}

void QtGuiDisplay::preparationForModifyFigure()
{
	QPoint cursorPosition{ ui.label_for_TempImg->getCursorPositionOnImage() };
	QSize imageSize{ ui.label_for_TempImg->getOriginalImageSize() };
	prepareForModifyFigure_ = -1;
	cursorOnDisplay_.setShape(Qt::ArrowCursor);
	if (cursorPosition.x() > 0 && cursorPosition.y() > 0 && cursorPosition.x() < imageSize.width() && cursorPosition.y() < imageSize.height())
		prepareForModifyFigure_ = getFigureIndexPrepareForModify(cursorPosition);

	if (prepareForModifyFigure_ > -1)
		cursorOnDisplay_.setShape(determenateCursorViewOnFigure(figuresForDrawing_[prepareForModifyFigure_], cursorPosition));

	setCursor(cursorOnDisplay_);
}

void QtGuiDisplay::moveImage()
{
	ui.label_for_TempImg->moveIamge();
	QPoint drawingPoint{ ui.label_for_TempImg->getDrawingPoint() };
	ui.horSB_forTempImg->setValue(drawingPoint.x());
	ui.verSB_forTempImg->setValue(drawingPoint.y());
	ui.label_for_TempImg->showPartImage();
}

void QtGuiDisplay::drawAllFigure()
{
	ui.label_for_TempImg->clearImageFromFigure();
	if (figuresForDrawing_.size() > 0)
	{
		std::vector<IFigure*> staticFigure(figuresForDrawing_);
		staticFigure.erase(staticFigure.begin() + activFigure_);
		ui.label_for_TempImg->drawStaticFigure(staticFigure);
		ui.label_for_TempImg->drawDynamicFigure(figuresForDrawing_[activFigure_]);
	}
}
