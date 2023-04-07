#include "myLabel.h"

myLabel::myLabel(QWidget *parent) :QLabel(parent)
{
}

myLabel::~myLabel()
{
}

void myLabel::setImage(const QPixmap& image)
{
	curentImage_ = image.copy();
	originalImage_ = image.copy();
	imageBuffer_ = image.copy();
	
	setNormalImageScale();
	imageScale_ = normalImageScale_;
	
	originalImageSize_ = originalImage_.size();
	scaledImageSize_ = originalImageSize_ * normalImageScale_;
	drawingSize_ = originalImageSize_;

	showPartImage();
}

void myLabel::updateImage(const QPixmap& image)
{
	curentImage_ = image.copy();
	originalImage_ = image.copy();
	originalImageSize_ = originalImage_.size();
	scaledImageSize_ = originalImageSize_;

	imageBuffer_ = curentImage_.copy();
	setNormalImageScale();
	setImageScale(imageScale_);
}

void myLabel::setImageScale(double const scale)
{
	imageScale_ = scale;
	scaledImageSize_.setWidth(originalImageSize_.width() * imageScale_);
	scaledImageSize_.setHeight(originalImageSize_.height() * imageScale_);
	
	if (imageScale_ > normalImageScale_)
	{
		QSize buferForDrawingSize{ drawingSize_ };
		drawingSize_.setWidth(width() / imageScale_);
		drawingSize_.setHeight(height() / imageScale_);

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
	if (imageBuffer_.isNull())
		partOfImageShown = curentImage_.copy(drawingPoint_.x(), drawingPoint_.y(), drawingSize_.width(), drawingSize_.height());
	else
		partOfImageShown = imageBuffer_.copy(drawingPoint_.x(), drawingPoint_.y(), drawingSize_.width(), drawingSize_.height());

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
	int dx{ (firstCursorPosition_.x() - cursorPosition_.x()) / imageScale_ };
	int dy{ (firstCursorPosition_.y() - cursorPosition_.y()) / imageScale_ };

	if (dx != 0)
		firstCursorPosition_.setX(cursorPosition_.x());

	if (dy != 0)
		firstCursorPosition_.setY(cursorPosition_.y());

	QPoint newDrawingPoint{ drawingPoint_.x() + dx, drawingPoint_.y() + dy };
	setDrawingPoint(newDrawingPoint);
}

void myLabel::setDrawingPoint(const QPoint& point)
{
	if (point.x() < 0)
		drawingPoint_.setX(0);
	else if (point.x() >= originalImageSize_.width() - drawingSize_.width())
		drawingPoint_.setX(originalImageSize_.width() - drawingSize_.width() - 1);
	else
		drawingPoint_.setX(point.x());

	if (point.y() < 0)
		drawingPoint_.setY(0);
	else if (point.y() >= originalImageSize_.height() - drawingSize_.height())
		drawingPoint_.setY(originalImageSize_.height() - drawingSize_.height() - 1);
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
			imageBuffer_ = curentImage_.copy();
		}
		else if (formatType == ImageFormat::GRAY)
		{
			QImage buferImg(curentImage_.toImage());
			buferImg = buferImg.convertToFormat(QImage::Format_Grayscale8);
			buferImg = buferImg.convertToFormat(QImage::Format_RGB32);
			imageBuffer_ = QPixmap(QPixmap::fromImage(buferImg));
			curentImage_ = imageBuffer_.copy();
		}
		imageFormat_ = formatType;
	}
}

void myLabel::drawPicture(const cv::Mat& drawPicture, const QRect& limitRect)
{
	if (imageIsShown_)
	{
		imageBuffer_ = curentImage_.copy();
		imageIsShown_ = false;
	}
	QImage drPic(drawPicture.data, drawPicture.cols, drawPicture.rows, drawPicture.step, QImage::Format::Format_BGR888);
	if (limitRect.width() != 0)
		drPic = drPic.scaled(limitRect.width(), limitRect.height());
	QPainter* painter = new QPainter(&imageBuffer_);
	painter->drawImage(limitRect.topLeft(), drPic);
	delete painter;
}

void myLabel::convertPointToImageCoordinate(QPoint& targetPoint) const
{
	int outputX{ targetPoint.x() / imageScale_ };
	if (imageScale_ > normalImageScale_)
	{
		outputX += drawingPoint_.x();
	}
	else
	{
		outputX -= (width() - scaledImageSize_.width()) / 2;
	}

	if (outputX >= scaledImageSize_.width())
		outputX = scaledImageSize_.width() - 1;
	else if (outputX <= 0)
		outputX = 0;
	targetPoint.setX(outputX);

	int outputY{ targetPoint.y() / imageScale_ };
	if (imageScale_ > normalImageScale_)
	{
		outputY += drawingPoint_.y();
	}
	else
	{
		outputY -= (height() - scaledImageSize_.height()) / 2;
	}

	if (outputY > scaledImageSize_.height())
		outputY = scaledImageSize_.height() - 1;
	else if (outputY < 0)
		outputY = 0;
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

double myLabel::getImageScale() const
{
	return imageScale_;
}

double myLabel::getNormalImageScale() const
{
	return normalImageScale_;
}

const QPixmap& myLabel::getCurentImage() const
{
	if (imageBuffer_.isNull())
		return curentImage_;
	else
		return imageBuffer_;
}

const QPixmap& myLabel::getOrignalImage() const
{
	return originalImage_;
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
}


void myLabel::setNormalImageScale()
{
	if (originalImageSize_.width() >= originalImageSize_.height())
		normalImageScale_ = static_cast<double>(width()) / originalImageSize_.width();
	else
		normalImageScale_ = static_cast<double>(height()) / originalImageSize_.height();
}

double round(double InputNumber, int const accuracy)
{
	InputNumber *= pow(10, accuracy + 1);
	InputNumber = static_cast<int>(round(InputNumber));
	return static_cast<double>(InputNumber / pow(10, accuracy));
}