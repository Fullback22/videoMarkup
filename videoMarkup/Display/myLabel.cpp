#include "myLabel.h"

myLabel::myLabel(QWidget *parent) :QLabel(parent)
{
}

myLabel::~myLabel()
{
	delete imageBuffer_;
	imageBuffer_ = nullptr;
	delete imageMouvePart_;
	imageMouvePart_ = nullptr;
}

void myLabel::toImgCoordinate(int &inOutX, int &inOutY, bool isContains)
{
	if (scaledImageSize_.width() > this->width())
	{
		inOutX += drawingPoint_.x();
	}
	else
	{
		inOutX -= (this->width() - scaledImageSize_.width()) / 2;
	}
	if (inOutX >= scaledImageSize_.width() && isContains)
		inOutX = scaledImageSize_.width()-1;
	else if (inOutX <= 0 && isContains)
		inOutX = 0;

	if (scaledImageSize_.height() > this->height())
	{
		inOutY += drawingPoint_.y();
	}
	else
	{
		inOutY -= (this->height() - scaledImageSize_.height()) / 2;
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
	
	if (scaledImageSize_.width() > this->width())
	{
		inOutX = (drawingPoint_.x() + inOutX) / scal_W;
	}
	else
	{
		inOutX = (inOutX - (this->width() - scaledImageSize_.width()) / 2) / scal_W;
	}
	if (inOutX >= originalImageSize_.width() && isContains)
		inOutX = originalImageSize_.width() - 1;
	else if (inOutX <= 0 && isContains)
		inOutX = 0.0;

	if (scaledImageSize_.height() > this->height())
	{
		inOutY = (drawingPoint_.y() + inOutY) / scal_H;
	}
	else
	{
		inOutY = (inOutY - (this->height() - scaledImageSize_.height()) / 2) / scal_H;
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
	if (scaledImageSize_.width() > this->width())
	{
		x = (drawingPoint_.x() + xPoint_) / scal_W;
	}
	else
	{
		x = (xPoint_ - (this->width() - scaledImageSize_.width()) / 2) / scal_W;
	}
	if (x >= originalImageSize_.width() && isContains)
		x = originalImageSize_.width()-1;
	else if (x <= 0 && isContains)
		x = 0.0;

	if (scaledImageSize_.height() > this->height())
	{
		y = (drawingPoint_.y() + yPoint_) / scal_H;
	}
	else
	{
		y =(yPoint_ -(this->height() - scaledImageSize_.height())/2 ) / scal_H;
	}

	if (y > originalImageSize_.height()&& isContains)
		y = originalImageSize_.height()-1;
	else if (y < 0 && isContains)
		y = 0.0;
	return QPoint(x, y);
}

QPoint myLabel::getFirstImagePoint()
{
	return QPoint(first_xPointOnImage, first_yPointOnImage);
}

void myLabel::formatImage(ImageFormat formatType)
{
	if (imageFormat == formatType)
	{

	}
	else
	{
		if (formatType == ImageFormat::ORIGINAL)
		{
			activImage_ = originalActivImage_;
			//my_Pixmap = my_Pixmap.scaled(scaledSize);
			delete imageBuffer_;
			imageBuffer_ = nullptr;
			imageBuffer_ = new QPixmap(activImage_);
		}
		else if (formatType == ImageFormat::GRAY)
		{
			QImage buferImg(activImage_.toImage());
			buferImg = buferImg.convertToFormat(QImage::Format_Grayscale8);
			buferImg = buferImg.convertToFormat(QImage::Format_RGB32);
			delete imageBuffer_;
			imageBuffer_ = nullptr;
			imageBuffer_ = new QPixmap(QPixmap::fromImage(buferImg));
			activImage_ = *imageBuffer_;
		}
		imageFormat = formatType;
	}
}

void myLabel::update_myPixmap(const QPixmap& img)
{
	activImage_ = img;
	originalActivImage_ = img;
	originalImageSize_ = originalActivImage_.size();
	scaledImageSize_ = originalImageSize_;
	widthScalingCoefficient_ = static_cast<double>(this->width()) / originalImageSize_.width();
	heightScalingCoefficient_ = static_cast<double>(this->height()) / originalImageSize_.height();
	delete imageBuffer_;
	imageBuffer_ = nullptr;
	imageBuffer_ = new QPixmap(activImage_);
	setAllImgScaled();
	this->scaledPixmap();
}

double myLabel::get_activScaled()
{
	if (activScaled_ < 11 && activScaled_ > 0)
	{
		return scaleds_[activScaled_] * 100;
	}
	else
	{
		return scaleds_[10] * 100;
	}
}

void myLabel::setImage(const QPixmap& image)
{
	activImage_ = image;
	originalActivImage_ = image;
	originalImageSize_ = originalActivImage_.size();
	scaledImageSize_ = originalImageSize_;
	widthScalingCoefficient_ = static_cast<double>(this->width()) / originalImageSize_.width();
	heightScalingCoefficient_ = static_cast<double>(this->height()) / originalImageSize_.height();
	delete imageBuffer_;
	imageBuffer_ = nullptr;
	imageBuffer_ = new QPixmap(activImage_);
	this->setPixmap(*imageBuffer_);
	activScaled_ = 4;
	this->updateGeometry();
	setAllImgScaled();
}

const QPixmap& myLabel::getImage() const
{
	return activImage_;
}

void myLabel::mouseMoveEvent(QMouseEvent* event)
{
	if (event->x() < 0)
	{
		xPoint_ = 0;
		emit mouseLeft();
	}
	else if (event->x() > this->width())
	{
		xPoint_ = this->width() - 1;
		emit mouseLeft();
	}
	else
	{
		xPoint_ = event->x();
	}

	if (event->y() < 0)
	{
		yPoint_ = 0;
		emit mouseLeft();
	}
	else if (event->y() > this->height())
	{
		yPoint_ = this->height() - 1;
		emit mouseLeft();
	}
	else
	{
		yPoint_ = event->y();
	}
	emit mousePos();
}

void myLabel::mousePressEvent(QMouseEvent* event)
{
	first_xPointOnLabel = event->x();
	first_yPointOnLabel = event->y();
	first_xPointOnImage = first_xPointOnLabel;
	first_yPointOnImage = first_yPointOnLabel;
	toImgCoordinate_(first_xPointOnImage, first_yPointOnImage);

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
		delete imageBuffer_;
		imageBuffer_ = nullptr;
		imageBuffer_ = new QPixmap(activImage_);
		imageIsShown_ = false;
	}
	QImage drPic(drawPicture.data, drawPicture.cols, drawPicture.rows, drawPicture.step, QImage::Format::Format_BGR888);
	if (limitRect.width() != 0)
		drPic=drPic.scaled(limitRect.width(), limitRect.height());
	QPainter* painter = new QPainter(imageBuffer_);
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
	if (imageBuffer_ == nullptr)
	{
		imageMouvePart_ = new QPixmap(activImage_.copy(dx, dy, originalImageSize_.width()*bufer_scalCoef_W, originalImageSize_.height()*bufer_scalCoef_H));
	}
	else
	{
		imageMouvePart_ = new QPixmap(imageBuffer_->copy(dx, dy, originalImageSize_.width() * bufer_scalCoef_W, originalImageSize_.height() * bufer_scalCoef_H));
	}
	
	int scaledMouvePixmap{ 0 };
	if (scaledImageSize_.width() <= this->width())
		scaledMouvePixmap = 10;
	if (scaledImageSize_.height() <= this->height())
		++scaledMouvePixmap;
	QSize qwe(imageMouvePart_->size());
	if (scaledMouvePixmap == 0)
	{
		this->setPixmap(imageMouvePart_->scaled(this->size(), aspectRatiotMode_));
	}
	else if (scaledMouvePixmap == 10)
		this->setPixmap(imageMouvePart_->scaled(QSize(this->scaledImageSize_.width(), this->height()), aspectRatiotMode_));
	else if (scaledMouvePixmap == 1)
		this->setPixmap(imageMouvePart_->scaled(QSize(this->width(), this->scaledImageSize_.height()), aspectRatiotMode_));
	else if (scaledMouvePixmap == 11)
	{
		this->setPixmap(imageMouvePart_->scaled(this->scaledImageSize_, aspectRatiotMode_));
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
	if (imageBuffer_ == nullptr)
	{
		imageMouvePart_ = new QPixmap(activImage_.copy(dx, dy, originalImageSize_.width() * bufer_scalCoef_W, originalImageSize_.height() * bufer_scalCoef_H));
	}
	else
	{
		imageMouvePart_ = new QPixmap(imageBuffer_->copy(dx, dy, originalImageSize_.width() * bufer_scalCoef_W, originalImageSize_.height() * bufer_scalCoef_H));

	}
	int scaledMouvePixmap{ 0 };
	if (scaledImageSize_.width() <= this->width())
		scaledMouvePixmap = 10;
	if (scaledImageSize_.height() <= this->height())
		++scaledMouvePixmap;
	if (scaledMouvePixmap == 0)
	{
		this->setPixmap(imageMouvePart_->scaled(this->size(), aspectRatiotMode_));
	}
	else if (scaledMouvePixmap == 10)
		this->setPixmap(imageMouvePart_->scaled(QSize(this->scaledImageSize_.width(), this->height()), aspectRatiotMode_));
	else if (scaledMouvePixmap == 1)
		this->setPixmap(imageMouvePart_->scaled(QSize(this->width(), this->scaledImageSize_.height()), aspectRatiotMode_));
	else if (scaledMouvePixmap == 11)
	{

		this->setPixmap(imageMouvePart_->scaled(this->scaledImageSize_, aspectRatiotMode_));
		scaledImageSize_ = pixmap()->size();
	}
	imageIsShown_ = true;
}

double myLabel::scaledPixmap(int const scaled, int &dx, int &dy)
{
	bool ver_center{ false };
	bool hor_center{ false };
	if (scaledImageSize_.width() < this->width())
		hor_center = true;
	if (scaledImageSize_.height() < this->height())
		ver_center=true;

	if (scaled == 0 && activScaled_ == 0)
	{
		activScaled_ = 0;
		scaledImageSize_.setWidth(this->width());
		scaledImageSize_.setHeight(this->height());
		aspectRatiotMode_ = Qt::KeepAspectRatio;
	}
	else
	{
		if (activScaled_ != 0)
		{

			if (scaled != 0)
			{
				this->toCenterPoint(drawingPoint_);
				originalDrawingPoint_.setX(drawingPoint_.x() / scaleds_[activScaled_]);
				originalDrawingPoint_.setY(drawingPoint_.y() / scaleds_[activScaled_]);
				if (scaled > 0 && activScaled_ < 10)
				{
					activScaled_++;
				}
				else if (scaled < 0 && activScaled_>1)
				{
					activScaled_--;
				}
				scaledImageSize_.setWidth(originalImageSize_.width() * scaleds_[activScaled_]);
				scaledImageSize_.setHeight(originalImageSize_.height() * scaleds_[activScaled_]);
				aspectRatiotMode_ = Qt::IgnoreAspectRatio;
			}
			else
			{
				activScaled_ = 0;
				scaledImageSize_.setWidth(this->width());
				scaledImageSize_.setHeight(this->height());
				aspectRatiotMode_ = Qt::KeepAspectRatio;
			}
		}
		else
		{
			this->toCenterPoint(drawingPoint_);
			originalDrawingPoint_.setX(drawingPoint_.x() / (static_cast<double>(scaledImageSize_.width())/ originalImageSize_.width()));
			originalDrawingPoint_.setY(drawingPoint_.y() / (static_cast<double>(scaledImageSize_.height()) / originalImageSize_.height()));
			if (scaled > 0)
				activScaled_ = maxActivScaled_;
			else
				activScaled_ = minActivScaled_;
			scaledImageSize_.setWidth(originalImageSize_.width() * scaleds_[activScaled_]);
			scaledImageSize_.setHeight(originalImageSize_.height() * scaleds_[activScaled_]);
			aspectRatiotMode_ = Qt::IgnoreAspectRatio;
		}
	}
	if (imageBuffer_ != nullptr)
	{
		formatImage(imageFormat);
		imageFormat = ImageFormat::ORIGINAL;
		if (activScaled_ != 0)
		{
			if (!hor_center)
				dx = originalDrawingPoint_.x() * scaleds_[activScaled_];
			else
				dx = scaledImageSize_.width() / 2;
			if (!ver_center)
				dy = originalDrawingPoint_.y() * scaleds_[activScaled_];
			else
				dy = scaledImageSize_.height()/2;
			this->toUpLeftpoint(dx, dy);
		}
	}
	
	if (scaled != 0)
	{
		return scaleds_[activScaled_] * 100;
	}
	else
	{
		return scaleds_[10] * 100;
	}
}

void myLabel::scaledPixmap()
{
	this->toCenterPoint(drawingPoint_);
	originalDrawingPoint_.setX(drawingPoint_.x() / scaleds_[activScaled_]);
	originalDrawingPoint_.setY(drawingPoint_.y() / scaleds_[activScaled_]);

	this->toUpLeftpoint(drawingPoint_);

	if (activScaled_ == 0)
	{
		scaledImageSize_.setWidth(width());
		scaledImageSize_.setHeight(height());
	}
	else
	{
		scaledImageSize_.setWidth(originalImageSize_.width() * scaleds_[activScaled_]);
		scaledImageSize_.setHeight(originalImageSize_.height() * scaleds_[activScaled_]);
	}

	aspectRatiotMode_ = Qt::IgnoreAspectRatio;
	if (imageBuffer_ != nullptr)
	{	
		formatImage(imageFormat);
		imageFormat = ImageFormat::ORIGINAL;
	}
}

void myLabel::moveImg(int &out_dx, int &out_dy)
{
	int dx{ first_xPointOnLabel - xPoint_ };
	first_xPointOnLabel = xPoint_;
	int dy{ first_yPointOnLabel - yPoint_ };
	first_yPointOnLabel = yPoint_;
	out_dx = drawingPoint_.x() + dx;
	out_dy = drawingPoint_.y() + dy;
}

QPoint myLabel::getDrawingPoint() const
{
	return drawingPoint_;
}

int myLabel::getMaxScaled() const
{
	return maxActivScaled_;
}

int myLabel::getMinScaled() const
{
	return minActivScaled_;
}

void myLabel::toCenterPoint(QPoint& upLeftPoint)
{
	upLeftPoint.setX(upLeftPoint.x() + (this->width() - 1) / 2);
	upLeftPoint.setY(upLeftPoint.y() + (this->height() - 1) / 2);
}

void myLabel::toUpLeftpoint(QPoint &centerPoint)
{
	centerPoint.setY(centerPoint.y() - (this->height() - 1) / 2);
	centerPoint.setX(centerPoint.x() - (this->width() - 1) / 2);
}

void myLabel::toUpLeftpoint(int &x, int &y)
{
	y = y - (this->height() - 1) / 2;
	x = x - (this->width() - 1) / 2;
}

void myLabel::resizeEvent(QResizeEvent* event)
{
	previousImageSize_ = event->oldSize();
	widthScalingCoefficient_ = static_cast<double>(this->width()) / originalImageSize_.width();
	heightScalingCoefficient_ = static_cast<double>(this->height()) / originalImageSize_.height();
}

QPoint myLabel::getImagePoint() const
{
	return QPoint(xPoint_, yPoint_);
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

void myLabel::setAllImgScaled()
{
	double scal_NormImg{ (double)this->width() / (double)originalImageSize_.width() };
	scaleds_[10] = scal_NormImg;
	for (int i{ 0 }; i < 9; i++)
	{
		if (scaleds_[i] > scal_NormImg)
		{
			maxActivScaled_ = i;
			i = 10;
		}
		else
			maxActivScaled_ = 10;
	}
	for (int i{ 9 }; i >= 0; i--)
	{
		if (scaleds_[i] < scal_NormImg)
		{
			minActivScaled_ = i;
			i = -1;
		}
		else
			minActivScaled_ = 0;
	}
}

void myLabel::getPointInImg(int& x, int& y)
{
	x = xPoint_;
	y = yPoint_;
	toImgCoordinate(x, y);
}

void myLabel::setFirstPixmapPoint(QPoint newPoint)
{
	first_xPointOnImage = newPoint.x();
	first_yPointOnImage = newPoint.y();
}

double round(double InputNumber, int const accuracy)
{
	InputNumber *= pow(10, accuracy + 1);
	InputNumber = static_cast<int>(round(InputNumber));
	return static_cast<double>(InputNumber / pow(10, accuracy));
}