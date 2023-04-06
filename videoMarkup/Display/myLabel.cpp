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
	originalImageSize_ = originalImage_.size();
	scaledImageSize_ = originalImageSize_;

	widthScalingCoefficient_ = static_cast<double>(width()) / originalImageSize_.width();
	heightScalingCoefficient_ = static_cast<double>(height()) / originalImageSize_.height();

	imageBuffer_ = image.copy();
	setPixmap(imageBuffer_);
	setNormalImageScaled();
	imageScale_ = normalImageScale_;
}

void myLabel::updateImage(const QPixmap& image)
{
	curentImage_ = image.copy();
	originalImage_ = image.copy();
	originalImageSize_ = originalImage_.size();
	scaledImageSize_ = originalImageSize_;
	widthScalingCoefficient_ = static_cast<double>(width()) / originalImageSize_.width();
	heightScalingCoefficient_ = static_cast<double>(height()) / originalImageSize_.height();
	imageBuffer_ = curentImage_.copy();
	setNormalImageScaled();
	setImageScale(imageScale_);
}

void myLabel::setImageScale(double const scale)
{
	double scaleRation{ scale / imageScale_ };
	drawingPoint_.setX(drawingPoint_.x() * scaleRation);
	drawingPoint_.setY(drawingPoint_.y() * scaleRation);

	imageScale_ = scale;
	scaledImageSize_.setWidth(originalImageSize_.width() * imageScale_);
	scaledImageSize_.setHeight(originalImageSize_.height() * imageScale_);
}

void myLabel::toImgCoordinate(int &inOutX, int &inOutY, bool isContains)
{
	if (scaledImageSize_.width() > width())
	{
		inOutX += drawingPoint_.x();
	}
	else
	{
		inOutX -= (width() - scaledImageSize_.width()) / 2;
	}
	if (inOutX >= scaledImageSize_.width() && isContains)
		inOutX = scaledImageSize_.width()-1;
	else if (inOutX <= 0 && isContains)
		inOutX = 0;

	if (scaledImageSize_.height() > height())
	{
		inOutY += drawingPoint_.y();
	}
	else
	{
		inOutY -= (height() - scaledImageSize_.height()) / 2;
	}

	if (inOutY > scaledImageSize_.height()&& isContains)
		inOutY = scaledImageSize_.height()-1;
	else if (inOutY < 0 && isContains)
		inOutY = 0;
}

void myLabel::toImgCoordinate_(int& inOutX, int& inOutY, bool isContains)
{
	double scal_H{ static_cast<double>(scaledImageSize_.height()) / originalImageSize_.height() };
	double scal_W{ static_cast<double>(scaledImageSize_.width()) / originalImageSize_.width() };
	
	if (scaledImageSize_.width() > width())
	{
		inOutX = (drawingPoint_.x() + inOutX) / scal_W;
	}
	else
	{
		inOutX = (inOutX - (width() - scaledImageSize_.width()) / 2) / scal_W;
	}
	if (inOutX >= originalImageSize_.width() && isContains)
		inOutX = originalImageSize_.width() - 1;
	else if (inOutX <= 0 && isContains)
		inOutX = 0.0;

	if (scaledImageSize_.height() > height())
	{
		inOutY = (drawingPoint_.y() + inOutY) / scal_H;
	}
	else
	{
		inOutY = (inOutY - (height() - scaledImageSize_.height()) / 2) / scal_H;
	}

	if (inOutY > originalImageSize_.height() && isContains)
		inOutY = originalImageSize_.height() - 1;
	else if (inOutY < 0 && isContains)
		inOutY = 0.0;
}

QPoint myLabel::getImageCoordinate(bool isContains)
{
	double scal_H{ static_cast<double>(scaledImageSize_.height()) / originalImageSize_.height() };
	double scal_W{ static_cast<double>(scaledImageSize_.width()) / originalImageSize_.width() };
	double x{ 0 }, y{ 0 };
	if (scaledImageSize_.width() > width())
	{
		x = (drawingPoint_.x() + cursorPosition_.x()) / scal_W;
	}
	else
	{
		x = (cursorPosition_.x() - (width() - scaledImageSize_.width()) / 2) / scal_W;
	}
	if (x >= originalImageSize_.width() && isContains)
		x = originalImageSize_.width()-1;
	else if (x <= 0 && isContains)
		x = 0.0;

	if (scaledImageSize_.height() > height())
	{
		y = (drawingPoint_.y() + cursorPosition_.y()) / scal_H;
	}
	else
	{
		y =(cursorPosition_.y() -(height() - scaledImageSize_.height())/2 ) / scal_H;
	}

	if (y > originalImageSize_.height()&& isContains)
		y = originalImageSize_.height()-1;
	else if (y < 0 && isContains)
		y = 0.0;
	return QPoint(x, y);
}

void myLabel::formatImage(ImageFormat formatType)
{
	if (imageFormat_ == formatType)
	{

	}
	else
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



double myLabel::getImageScaled() const
{
	return imageScale_;
}

double myLabel::getNormalImageScaled() const
{
	return normalImageScale_;
}



const QPixmap& myLabel::getImage() const
{
	return curentImage_;
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
	firstCursorPosition_.setX(event->x());
	firstCursorPosition_.setY(event->y());
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

void myLabel::drawPicture(const cv::Mat& drawPicture, const QRect& limitRect)
{
	if (imageIsShown_)
	{
		imageBuffer_ = curentImage_.copy();
		imageIsShown_ = false;
	}
	QImage drPic(drawPicture.data, drawPicture.cols, drawPicture.rows, drawPicture.step, QImage::Format::Format_BGR888);
	if (limitRect.width() != 0)
		drPic=drPic.scaled(limitRect.width(), limitRect.height());
	QPainter* painter = new QPainter(&imageBuffer_);
	painter->drawImage(limitRect.topLeft(), drPic);
	delete painter;
}

void myLabel::showPartImage(int xPart, int yPart, int const widthPart, int const heightPart)
{
	if (xPart < 0)
		xPart = 0;
	else if (xPart >= (scaledImageSize_.width() - widthPart))
		xPart = scaledImageSize_.width() - widthPart;
	if (yPart < 0)
		yPart = 0;
	else if (yPart >= (scaledImageSize_.height() - heightPart))
		yPart = scaledImageSize_.height() - heightPart;

	drawingPoint_.setX(xPart);
	drawingPoint_.setY(yPart);
	
	double buferForScaleByWidth{ static_cast<double>(originalImageSize_.width()) / scaledImageSize_.width() };
	
	xPart *= buferForScaleByWidth;
	buferForScaleByWidth *= widthScalingCoefficient_;
	
	double buferForScaleByHeigth{ static_cast<double>(originalImageSize_.height()) / scaledImageSize_.height() };
	
	yPart *= buferForScaleByHeigth;
	buferForScaleByHeigth *= heightScalingCoefficient_;
	QPixmap imageMouvePart{};
	if (imageBuffer_.isNull())
		imageMouvePart = curentImage_.copy(xPart, yPart, originalImageSize_.width() * buferForScaleByWidth, originalImageSize_.height() * buferForScaleByHeigth);
	else
		imageMouvePart = imageBuffer_.copy(xPart, yPart, originalImageSize_.width() * buferForScaleByWidth, originalImageSize_.height() * buferForScaleByHeigth);
	
	QSize scaledSize{ size() };

	if (scaledImageSize_.width() <= width())
		scaledSize.setWidth(scaledImageSize_.width());
	if (scaledImageSize_.height() <= height())
		scaledSize.setHeight(scaledImageSize_.height());
	
	setPixmap(imageMouvePart.scaled(scaledSize, aspectRatiotMode_));
	imageIsShown_ = true;
	
	//if (scaledImageSize_.width() <= width() && scaledImageSize_.height() <= height())
		//scaledImageSize_ = pixmap()->size();
}

void myLabel::showPartImage()
{
	int xPart{ drawingPoint_.x() };
	int yPart{ drawingPoint_.y() };

	double buferForScaleByWidth{ static_cast<double>(originalImageSize_.width()) / scaledImageSize_.width() };

	xPart *= buferForScaleByWidth;
	buferForScaleByWidth *= widthScalingCoefficient_;

	double buferForScaleByHeigth{ static_cast<double>(originalImageSize_.height()) / scaledImageSize_.height() };

	yPart *= buferForScaleByHeigth;
	buferForScaleByHeigth *= heightScalingCoefficient_;

	QPixmap imageMouvePart{};
	if (imageBuffer_.isNull())
	{
		imageMouvePart = curentImage_.copy(xPart, yPart, originalImageSize_.width() * buferForScaleByWidth, originalImageSize_.height() * buferForScaleByHeigth);
	}
	else
	{
		imageMouvePart = imageBuffer_.copy(xPart, yPart, originalImageSize_.width() * buferForScaleByWidth, originalImageSize_.height() * buferForScaleByHeigth);
	}

	QSize scaledSize{ size() };
	if (scaledImageSize_.width() <= width())
		scaledSize.setWidth(scaledImageSize_.width());
	if (scaledImageSize_.height() <= height())
		scaledSize.setHeight(scaledImageSize_.height());

	setPixmap(imageMouvePart.scaled(scaledSize, aspectRatiotMode_));
	imageIsShown_ = true;
}



void myLabel::moveImg(int &out_dx, int &out_dy)
{
	int dx{ firstCursorPosition_.x() - cursorPosition_.x()};
	int dy{ firstCursorPosition_.y() - cursorPosition_.y() };
	firstCursorPosition_ = cursorPosition_;
	
	out_dx = drawingPoint_.x() + dx;
	out_dy = drawingPoint_.y() + dy;
}

QPoint myLabel::getDrawingPoint() const
{
	return drawingPoint_;
}

void myLabel::toCenterPoint(QPoint& upLeftPoint)
{
	upLeftPoint.setX(upLeftPoint.x() + (width() - 1) / 2);
	upLeftPoint.setY(upLeftPoint.y() + (height() - 1) / 2);
}

void myLabel::toUpLeftpoint(QPoint &centerPoint)
{
	centerPoint.setY(centerPoint.y() - (height() - 1) / 2);
	centerPoint.setX(centerPoint.x() - (width() - 1) / 2);
}

void myLabel::toUpLeftpoint(int &x, int &y)
{
	y = y - (height() - 1) / 2;
	x = x - (width() - 1) / 2;
}

void myLabel::resizeEvent(QResizeEvent* event)
{
	previousImageSize_ = event->oldSize();
	widthScalingCoefficient_ = static_cast<double>(width()) / originalImageSize_.width();
	heightScalingCoefficient_ = static_cast<double>(height()) / originalImageSize_.height();
}

QPoint myLabel::getImagePoint() const
{
	return cursorPosition_;
}

QSize myLabel::getOldSize() const
{
	return previousImageSize_;
}

QSize myLabel::getScaledImgSize() const
{
	return scaledImageSize_;
}

QSize myLabel::getOriginalImgSize() const
{
	return originalImageSize_;
}



void myLabel::getPointInImg(int& x, int& y)
{
	x = cursorPosition_.x();
	y = cursorPosition_.y();
	toImgCoordinate(x, y);
}

void myLabel::setNormalImageScaled()
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