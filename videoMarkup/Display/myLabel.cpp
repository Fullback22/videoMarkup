#include "myLabel.h"

myLabel::myLabel(QWidget *parent) :QLabel(parent)
{
}

myLabel::~myLabel()
{
	delete imageMouvePart_;
	imageMouvePart_ = nullptr;
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
	imageScaled_ = 1.0;
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
	scaledPixmap();
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



double myLabel::get_activScaled()
{
	/*if (activScaled_ < 10 && activScaled_ > 0)
	{
		return scaleds_[activScaled_] * 100;
	}
	else
	{
		return scaleds_[10] * 100;
	}*/
	return imageScaled_;
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

void myLabel::draw_picture(const cv::Mat& drawPicture, const QRect& limitRect)
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

void myLabel::show_partImg(int dx, int dy, int const width, int const height)
{
	if (dx < 0)
		dx = 0;
	else if (dx >= (scaledImageSize_.width() - width))
		dx = scaledImageSize_.width() - width;
	if (dy < 0)
		dy = 0;
	else if (dy >= (scaledImageSize_.height() - height))
		dy = scaledImageSize_.height() - height;

	drawingPoint_.setX(dx);
	drawingPoint_.setY(dy);
	bool notScaled{ false };
	double bufer_scalCoef_W{ static_cast<double>(originalImageSize_.width()) / scaledImageSize_.width() };
	
	dx = dx * bufer_scalCoef_W;
	bufer_scalCoef_W *= widthScalingCoefficient_;
	
	double bufer_scalCoef_H{ static_cast<double>(originalImageSize_.height()) / scaledImageSize_.height() };
	
	dy = dy * bufer_scalCoef_H;
	bufer_scalCoef_H *= heightScalingCoefficient_;
	

	delete imageMouvePart_;
	imageMouvePart_ = nullptr;
	if (imageBuffer_.isNull())
	{
		imageMouvePart_ = new QPixmap(curentImage_.copy(dx, dy, originalImageSize_.width()*bufer_scalCoef_W, originalImageSize_.height()*bufer_scalCoef_H));
	}
	else
	{
		imageMouvePart_ = new QPixmap(imageBuffer_.copy(dx, dy, originalImageSize_.width() * bufer_scalCoef_W, originalImageSize_.height() * bufer_scalCoef_H));
	}
	
	int scaledMouvePixmap{ 0 };
	if (scaledImageSize_.width() <= this->width())
		scaledMouvePixmap = 10;
	if (scaledImageSize_.height() <= this->height())
		++scaledMouvePixmap;
	QSize qwe(imageMouvePart_->size());
	if (scaledMouvePixmap == 0)
	{
		setPixmap(imageMouvePart_->scaled(size(), aspectRatiotMode_));
	}
	else if (scaledMouvePixmap == 10)
		setPixmap(imageMouvePart_->scaled(QSize(scaledImageSize_.width(), this->height()), aspectRatiotMode_));
	else if (scaledMouvePixmap == 1)
		setPixmap(imageMouvePart_->scaled(QSize(this->width(), scaledImageSize_.height()), aspectRatiotMode_));
	else if (scaledMouvePixmap == 11)
	{
		setPixmap(imageMouvePart_->scaled(scaledImageSize_, aspectRatiotMode_));
		scaledImageSize_ = pixmap()->size();
	}
	imageIsShown_ = true;
}

void myLabel::show_partImg()
{
	int dx{ drawingPoint_.x() };
	int dy{ drawingPoint_.y() };

	double bufer_scalCoef_W{ static_cast<double>(originalImageSize_.width()) / scaledImageSize_.width() };

	dx = dx * bufer_scalCoef_W;
	bufer_scalCoef_W *= widthScalingCoefficient_;

	double bufer_scalCoef_H{ static_cast<double>(originalImageSize_.height()) / scaledImageSize_.height() };

	dy = dy * bufer_scalCoef_H;
	bufer_scalCoef_H *= heightScalingCoefficient_;


	delete imageMouvePart_;
	imageMouvePart_ = nullptr;
	if (imageBuffer_.isNull())
	{
		imageMouvePart_ = new QPixmap(curentImage_.copy(dx, dy, originalImageSize_.width() * bufer_scalCoef_W, originalImageSize_.height() * bufer_scalCoef_H));
	}
	else
	{
		imageMouvePart_ = new QPixmap(imageBuffer_.copy(dx, dy, originalImageSize_.width() * bufer_scalCoef_W, originalImageSize_.height() * bufer_scalCoef_H));
	}
	int scaledMouvePixmap{ 0 };
	if (scaledImageSize_.width() <= width())
		scaledMouvePixmap = 10;
	if (scaledImageSize_.height() <= height())
		++scaledMouvePixmap;
	
	if (scaledMouvePixmap == 0)
		setPixmap(imageMouvePart_->scaled(size(), aspectRatiotMode_));
	else if (scaledMouvePixmap == 10)
		setPixmap(imageMouvePart_->scaled(QSize(scaledImageSize_.width(), height()), aspectRatiotMode_));
	else if (scaledMouvePixmap == 1)
		setPixmap(imageMouvePart_->scaled(QSize(width(), scaledImageSize_.height()), aspectRatiotMode_));
	else if (scaledMouvePixmap == 11)
	{
		setPixmap(imageMouvePart_->scaled(scaledImageSize_, aspectRatiotMode_));
		scaledImageSize_ = pixmap()->size();
	}
	imageIsShown_ = true;
}

double myLabel::scaledPixmap(int const scaled, int &dx, int &dy)
{
	bool ver_center{ false };
	bool hor_center{ false };
	if (scaledImageSize_.width() < width())
		hor_center = true;
	if (scaledImageSize_.height() < height())
		ver_center=true;

	if (scaled == 0 && activScaled_ == 0)
	{
		activScaled_ = 0;
		scaledImageSize_.setWidth(width());
		scaledImageSize_.setHeight(height());
		aspectRatiotMode_ = Qt::KeepAspectRatio;
	}
	else
	{
		if (activScaled_ != 0)
		{

			if (scaled != 0)
			{
				toCenterPoint(drawingPoint_);
				originalDrawingPoint_.setX(drawingPoint_.x() / imageScaled_);
				originalDrawingPoint_.setY(drawingPoint_.y() / imageScaled_);
				if (scaled > 0 && activScaled_ < 10)
				{
					activScaled_++;
				}
				else if (scaled < 0 && activScaled_>1)
				{
					activScaled_--;
				}
				scaledImageSize_.setWidth(originalImageSize_.width() * imageScaled_);
				scaledImageSize_.setHeight(originalImageSize_.height() * imageScaled_);
				aspectRatiotMode_ = Qt::IgnoreAspectRatio;
			}
			else
			{
				activScaled_ = 0;
				scaledImageSize_.setWidth(width());
				scaledImageSize_.setHeight(height());
				aspectRatiotMode_ = Qt::KeepAspectRatio;
			}
		}
		else
		{
			toCenterPoint(drawingPoint_);
			originalDrawingPoint_.setX(drawingPoint_.x() / (static_cast<double>(scaledImageSize_.width())/ originalImageSize_.width()));
			originalDrawingPoint_.setY(drawingPoint_.y() / (static_cast<double>(scaledImageSize_.height()) / originalImageSize_.height()));
			if (scaled > 0)
				activScaled_ = maxActivScaled_;
			else
				activScaled_ = minActivScaled_;
			scaledImageSize_.setWidth(originalImageSize_.width() * imageScaled_);
			scaledImageSize_.setHeight(originalImageSize_.height() * imageScaled_);
			aspectRatiotMode_ = Qt::IgnoreAspectRatio;
		}
	}
	if (!imageBuffer_.isNull())
	{
		formatImage(imageFormat_);
		imageFormat_ = ImageFormat::ORIGINAL;
		if (activScaled_ != 0)
		{
			if (!hor_center)
				dx = originalDrawingPoint_.x() * imageScaled_;
			else
				dx = scaledImageSize_.width() / 2;
			if (!ver_center)
				dy = originalDrawingPoint_.y() * imageScaled_;
			else
				dy = scaledImageSize_.height()/2;
			toUpLeftpoint(dx, dy);
		}
	}
	
	/*if (scaled != 0)
	{
		return scaleds_[activScaled_] * 100;
	}
	else
	{
		return scaleds_[10] * 100;
	}*/
	return imageScaled_ * 100;
}

void myLabel::scaledPixmap()
{
	toCenterPoint(drawingPoint_);
	originalDrawingPoint_.setX(drawingPoint_.x() / imageScaled_);
	originalDrawingPoint_.setY(drawingPoint_.y() / imageScaled_);

	toUpLeftpoint(drawingPoint_);

	if (activScaled_ == 0)
	{
		scaledImageSize_.setWidth(width());
		scaledImageSize_.setHeight(height());
	}
	else
	{
		scaledImageSize_.setWidth(originalImageSize_.width() * imageScaled_);
		scaledImageSize_.setHeight(originalImageSize_.height() * imageScaled_);
	}

	aspectRatiotMode_ = Qt::IgnoreAspectRatio;
	if (!imageBuffer_.isNull())
	{	
		formatImage(imageFormat_);
		imageFormat_ = ImageFormat::ORIGINAL;
	}
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

void myLabel::setNormalImageScaled()
{
	if (originalImageSize_.width() >= originalImageSize_.height())
		normalImageScaled_ = static_cast<double>(width()) / originalImageSize_.width();
	else
		normalImageScaled_ = static_cast<double>(height()) / originalImageSize_.height();
}

void myLabel::getPointInImg(int& x, int& y)
{
	x = cursorPosition_.x();
	y = cursorPosition_.y();
	toImgCoordinate(x, y);
}

double round(double InputNumber, int const accuracy)
{
	InputNumber *= pow(10, accuracy + 1);
	InputNumber = static_cast<int>(round(InputNumber));
	return static_cast<double>(InputNumber / pow(10, accuracy));
}