#include "myLabel.h"

myLabel::myLabel(QWidget *parent) :QLabel(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
}

myLabel::~myLabel()
{
}

void myLabel::setImage(const QPixmap& image)
{
	curentImage_ = image.copy();
	originalImage_ = image.copy();
	imageWithStaticFigure_ = image.copy();
	
	originalImageSize_ = originalImage_.size();
	setNormalImageScale();
	scaledImageSize_ = originalImageSize_ * imageScale_;
	drawingSize_ = originalImageSize_;

	coefficientRecalculationByWidth_ = originalImage_.width() / static_cast<float>(width());
	coefficientRecalculationByHeigth_ = originalImage_.height() / static_cast<float>(height());
}

void myLabel::updateImage(const QPixmap& image)
{
	curentImage_ = image.copy();
	originalImage_ = image.copy();
	originalImageSize_ = originalImage_.size();
	scaledImageSize_ = originalImageSize_;

	imageWithStaticFigure_ = curentImage_.copy();
	setNormalImageScale();
	setImageScale(imageScale_);
}

void myLabel::setImageScale(float const scale)
{
	float previousScale{ imageScale_ };
	imageScale_ = scale;
	scaledImageSize_.setWidth(originalImageSize_.width() * imageScale_);
	scaledImageSize_.setHeight(originalImageSize_.height() * imageScale_);
	
	if (imageScale_ > normalImageScale_)
	{
		QSize buferForDrawingSize{ drawingSize_ };
		drawingSize_.setWidth(width() / imageScale_);
		if (drawingSize_.width() >= originalImageSize_.width())
			drawingSize_.setWidth(originalImageSize_.width());
		drawingSize_.setHeight(height() / imageScale_);
		if (drawingSize_.height() >= originalImageSize_.height())
			drawingSize_.setHeight(originalImageSize_.height());

		buferForDrawingSize -= drawingSize_;
		QPoint newDrawingPoint{ drawingPoint_.x() + buferForDrawingSize.width() / 2, drawingPoint_.y() + buferForDrawingSize.height() / 2 };
		setDrawingPoint(newDrawingPoint);
	}
	else
	{
		drawingSize_ = originalImageSize_;
		drawingPoint_.setX(0);
		drawingPoint_.setY(0);
	}
}

void myLabel::showPartImage()
{
	QPixmap partOfImageShown{};
	
	partOfImageShown = curentImage_.copy(drawingPoint_.x(), drawingPoint_.y(), drawingSize_.width(), drawingSize_.height());
	

	QSize scaledSize{ size() };

	if (scaledImageSize_.width() <= width())
		scaledSize.setWidth(scaledImageSize_.width());
	if (scaledImageSize_.height() <= height())
		scaledSize.setHeight(scaledImageSize_.height());

	setPixmap(partOfImageShown.scaled(scaledSize, aspectRatiotMode_));
	imageIsShown_ = true;
}

void myLabel::moveIamge()
{
	int dx{ static_cast<int>((firstCursorPosition_.x() - cursorPosition_.x()) / imageScale_) };
	int dy{ static_cast<int>((firstCursorPosition_.y() - cursorPosition_.y()) / imageScale_) };

	QPoint newDrawingPoint{ drawingPoint_.x(), drawingPoint_.y()};
	if (dx != 0 && scaledImageSize_.width() > width())
	{
		firstCursorPosition_.setX(cursorPosition_.x());
		newDrawingPoint.setX(newDrawingPoint.x() + dx);
	}

	if (dy != 0 && scaledImageSize_.height() > height())
	{
		firstCursorPosition_.setY(cursorPosition_.y());
		newDrawingPoint.setY(newDrawingPoint.y() + dy);
	}
	setDrawingPoint(newDrawingPoint);
}

void myLabel::setDrawingPoint(const QPoint& point)
{
	if (point.x() < 0)
		drawingPoint_.setX(0);
	else if (point.x() >= abs(originalImageSize_.width() - drawingSize_.width()))
		drawingPoint_.setX(originalImageSize_.width() - drawingSize_.width() );
	else
		drawingPoint_.setX(point.x());

	if (point.y() < 0)
		drawingPoint_.setY(0);
	else if (point.y() >= abs(originalImageSize_.height() - drawingSize_.height()))
		drawingPoint_.setY(originalImageSize_.height() - drawingSize_.height() );
	else
		drawingPoint_.setY(point.y());
}

void myLabel::setFormatImage(ImageFormat formatType)
{
	if (imageFormat_ != formatType)
	{
		if (formatType == ImageFormat::ORIGINAL)
		{
			curentImage_ = originalImage_;
			imageWithStaticFigure_ = curentImage_.copy();
		}
		else if (formatType == ImageFormat::GRAY)
		{
			QImage buferImg(curentImage_.toImage());
			buferImg = buferImg.convertToFormat(QImage::Format_Grayscale8);
			buferImg = buferImg.convertToFormat(QImage::Format_RGB32);
			imageWithStaticFigure_ = QPixmap(QPixmap::fromImage(buferImg));
			curentImage_ = imageWithStaticFigure_.copy();
		}
		imageFormat_ = formatType;
	}
}

void myLabel::drawPicture(const QImage& drawPicture, const QRect& limitRect)
{
	if (imageIsShown_)
	{
		imageWithStaticFigure_ = curentImage_.copy();
		imageIsShown_ = false;
	}
	QImage drPic{ drawPicture.copy() };
	if (limitRect.width() != 0)
		drPic = drPic.scaled(limitRect.width(), limitRect.height());
	QPainter* painter = new QPainter(&imageWithStaticFigure_);
	painter->drawImage(limitRect.topLeft(), drPic);
	delete painter;
}

void myLabel::drawStaticFigure(const IFigure* figure)
{
	figure->draw(imageWithStaticFigure_);
	curentImage_ = imageWithStaticFigure_.copy();
}

void myLabel::drawStaticFigure(const std::vector<IFigure*>& figures)
{
	for(const IFigure* figure:figures)
		figure->draw(imageWithStaticFigure_);
	curentImage_ = imageWithStaticFigure_.copy();
}

void myLabel::drawDynamicFigure(const IFigure* figure)
{
	curentImage_ = imageWithStaticFigure_.copy();
	figure->draw(curentImage_);
}

void myLabel::clearImageFromFigure()
{
	curentImage_ = originalImage_.copy();
	imageWithStaticFigure_ = originalImage_.copy();
}

void myLabel::convertPointToImageCoordinate(QPoint& targetPoint) const
{
	int outputX{ static_cast<int>(targetPoint.x() / imageScale_) };
	if (width() < scaledImageSize_.width())
	{
		outputX += drawingPoint_.x();
	}
	else
	{
		outputX = (targetPoint.x() - (width() - scaledImageSize_.width()) / 2) / imageScale_;
	}

	targetPoint.setX(outputX);

	int outputY{ static_cast<int>(targetPoint.y() / imageScale_) };
	if (height() < scaledImageSize_.height())
	{
		outputY += drawingPoint_.y();
	}
	else
	{
		outputY = (targetPoint.y() - (height() - scaledImageSize_.height()) / 2) / imageScale_;
	}

	targetPoint.setY(outputY);
}

QPoint myLabel::getDrawingPoint() const
{
	return drawingPoint_;
}

QPoint myLabel::getCursorPositionOnLabel() const
{
	return cursorPosition_;
}

QPoint myLabel::getCursorPositionOnImage() const
{
	QPoint cursorPosition{ cursorPosition_ };
	convertPointToImageCoordinate(cursorPosition);
	return cursorPosition;
}

QPoint& myLabel::getDeltaOnImageCoordinate()
{
	int dx{ static_cast<int>(round((cursorPosition_.x() - firstCursorPosition_.x()) / imageScale_)) };
	int dy{ static_cast<int>(round((cursorPosition_.y() - firstCursorPosition_.y()) / imageScale_)) };
	QPoint delta{ dx, dy };

	if (dx != 0)
	{
		firstCursorPosition_.setX(cursorPosition_.x());
	}
	if (dy != 0 )
	{
		firstCursorPosition_.setY(cursorPosition_.y());
	}
	return delta;
}

QSize myLabel::getPreviousImageSize() const
{
	return previousImageSize_;
}

QSize myLabel::getScaledImageSize() const
{
	return scaledImageSize_;
}

QSize myLabel::getOriginalImageSize() const
{
	return originalImageSize_;
}

QSize myLabel::getDrawingSize() const
{
	return drawingSize_;
}

float myLabel::getImageScale() const
{
	return imageScale_;
}

float myLabel::getNormalImageScale() const
{
	return normalImageScale_;
}

float myLabel::getCoefficientRecalculationByWidth() const
{
	return coefficientRecalculationByWidth_;
}

float myLabel::getCoefficientRecalculationByHeigth() const
{
	return coefficientRecalculationByHeigth_;
}

const QPixmap& myLabel::getCurentImage() const
{
	return curentImage_;
}

const QPixmap& myLabel::getOrignalImage() const
{
	return originalImage_;
}

void myLabel::setNormalImageScale()
{
	float previousNormalScale{ normalImageScale_ };
	if (originalImageSize_.width() >= originalImageSize_.height()) 
	{
		normalImageScale_ = static_cast<float>(width()) / originalImageSize_.width();
		if (static_cast<int>(originalImageSize_.height() * normalImageScale_) > height())
			normalImageScale_ = static_cast<float>(height()) / originalImageSize_.height();
	}
	else 
	{
		normalImageScale_ = static_cast<float>(height()) / originalImageSize_.height();
		if (static_cast<int>(originalImageSize_.width() * normalImageScale_) > width())
			normalImageScale_ = static_cast<float>(width()) / originalImageSize_.width();
	}
	if (imageScale_ == previousNormalScale)
	{
		imageScale_ = normalImageScale_;
		scaledImageSize_.setWidth(originalImageSize_.width() * imageScale_);
		scaledImageSize_.setHeight(originalImageSize_.height() * imageScale_);
	}
}

void myLabel::mouseMoveEvent(QMouseEvent* event)
{
	if (event->x() < 0)
	{
		cursorPosition_.setX(0);
		emit mouseLeft();
	}
	else if (event->x() > width())
	{
		cursorPosition_.setX(width() - 1);
		emit mouseLeft();
	}
	else
	{
		cursorPosition_.setX(event->x());
	}

	if (event->y() < 0)
	{
		cursorPosition_.setY(0);
		emit mouseLeft();
	}
	else if (event->y() > height())
	{
		cursorPosition_.setY(height() - 1);
		emit mouseLeft();
	}
	else
	{
		cursorPosition_.setY(event->y());
	}
	emit mousePos();
}

void myLabel::mousePressEvent(QMouseEvent* event)
{
	cursorPosition_.setX(event->x());
	cursorPosition_.setY(event->y());
	firstCursorPosition_ = cursorPosition_;
	emit mousePressed();
}

void myLabel::leaveEvent(QMouseEvent* event)
{
	emit mouseLeft();
}

void myLabel::mouseReleaseEvent(QMouseEvent* event)
{
	emit mouseRelease();
}

void myLabel::resizeEvent(QResizeEvent* event)
{
	previousImageSize_ = event->oldSize();
	setNormalImageScale();
	if (imageScale_ > normalImageScale_)
	{
		drawingSize_.setWidth(width() / imageScale_);
		drawingSize_.setHeight(height() / imageScale_);
	}
	else
	{
		drawingSize_ = originalImageSize_;
		drawingPoint_.setX(0);
		drawingPoint_.setY(0);
	}
	coefficientRecalculationByWidth_ = originalImage_.width() / static_cast<float>(width());
	coefficientRecalculationByHeigth_ = originalImage_.height() / static_cast<float>(height());
}

float round(float InputNumber, int const accuracy)
{
	InputNumber *= pow(10, accuracy + 1);
	InputNumber = static_cast<int>(round(InputNumber));
	return static_cast<float>(InputNumber / pow(10, accuracy));
}