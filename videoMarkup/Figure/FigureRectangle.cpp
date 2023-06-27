#include "FigureRectangle.h"

FigureRectangle::FigureRectangle(float const x, float const y, float const width, float const heidth) :
	x_{ x },
	y_{ y },
	width_{ width },
	heidth_{ heidth }
{
}

void FigureRectangle::Delete()
{
	delete this;
}

void FigureRectangle::draw(QPixmap& background) const
{
	QPainter painter{ &background };
	painter.setPen(pen_);
	painter.drawRect(x_, y_, width_, heidth_);
}

bool FigureRectangle::contains(const QPoint& point) const
{
	float rightX{ x_ + width_ };
	float bottomY{ y_ + heidth_ };
	if (point.x() > x_ && point.y() > y_ && point.x() < rightX && point.y() < bottomY)
		return true;
	else
		return false;
}

bool FigureRectangle::containsToModifyArea(const QPoint& point) const
{
	float leftX{ x_ - deltaOutsideBorder_ };
	float rightX{ x_ + width_ + deltaOutsideBorder_ };
	float upperY{ y_ - deltaOutsideBorder_ };
	float bottomY{ y_ + heidth_ + deltaOutsideBorder_ };
	if (point.x() > leftX && point.y() > upperY && point.x() < rightX && point.y() < bottomY)
		return true;
	else
		return false;
}

bool FigureRectangle::isUpperCenterBorder(const QPoint& point) const
{
	float leftX{ x_ };
	float rightX{ x_ + width_ };
	float upperY{ y_ - deltaOutsideBorder_ };
	float bottomY{ y_ + deltaInsideBorder_ };
	if (point.x() > leftX && point.y() > upperY && point.x() < rightX && point.y() < bottomY)
		return true;
	else
		return false;
}

bool FigureRectangle::isUpperLeftBorder(const QPoint& point) const
{
	float leftX{ x_ - deltaOutsideBorder_ };
	float rightX{ x_ };
	float upperY{ y_ - deltaOutsideBorder_ };
	float bottomY{ y_ };
	if (point.x() > leftX && point.y() > upperY && point.x() < rightX && point.y() < bottomY)
		return true;
	else
		return false;
}

bool FigureRectangle::isUpperRightBorder(const QPoint& point) const
{
	float leftX{ x_ + width_ };
	float rightX{ x_ + width_ + deltaOutsideBorder_ };
	float upperY{ y_ - deltaOutsideBorder_ };
	float bottomY{ y_ };
	if (point.x() > leftX && point.y() > upperY && point.x() < rightX && point.y() < bottomY)
		return true;
	else
		return false;
}

bool FigureRectangle::isBottomCenterBorder(const QPoint& point) const
{
	float leftX{ x_ };
	float rightX{ x_ + width_ };
	float upperY{ y_ + heidth_ - deltaInsideBorder_ };
	float bottomY{ y_ + heidth_ + deltaOutsideBorder_ };
	if (point.x() > leftX && point.y() > upperY && point.x() < rightX && point.y() < bottomY)
		return true;
	else
		return false;
}

bool FigureRectangle::isBottomLeftBorder(const QPoint& point) const
{
	float leftX{ x_ - deltaOutsideBorder_ };
	float rightX{ x_ };
	float upperY{ y_ + heidth_ };
	float bottomY{ y_ + heidth_ + deltaOutsideBorder_ };
	if (point.x() > leftX && point.y() > upperY && point.x() < rightX && point.y() < bottomY)
		return true;
	else
		return false;
}

bool FigureRectangle::isBottomRightBorder(const QPoint& point) const
{
	float leftX{ x_ + width_ };
	float rightX{ x_ + width_ + deltaOutsideBorder_ };
	float upperY{ y_ + heidth_ };
	float bottomY{ y_ + heidth_ + deltaOutsideBorder_ };
	if (point.x() > leftX && point.y() > upperY && point.x() < rightX && point.y() < bottomY)
		return true;
	else
		return false;
}

bool FigureRectangle::isRightCenterBorder(const QPoint& point) const
{
	float leftX{ x_ + width_ - deltaInsideBorder_ };
	float rightX{ x_ + width_ + deltaOutsideBorder_ };
	float upperY{ y_ };
	float bottomY{ y_ + heidth_ };
	if (point.x() > leftX && point.y() > upperY && point.x() < rightX && point.y() < bottomY)
		return true;
	else
		return false;
}

bool FigureRectangle::isLeftCenterBorder(const QPoint& point) const
{
	float leftX{ x_ - deltaOutsideBorder_ };
	float rightX{ x_ + deltaInsideBorder_ };
	float upperY{ y_ };
	float bottomY{ y_ + heidth_ };
	if (point.x() > leftX && point.y() > upperY && point.x() < rightX && point.y() < bottomY)
		return true;
	else
		return false;
}

float FigureRectangle::getArea() const
{
	return width_ * heidth_;
}

void FigureRectangle::prepareForModify(const QPoint& point)
{
	if (contains(point))
	{
		modifyType_ = ModifyType::Move;
	}

	if (isUpperCenterBorder(point))
	{
		modifyType_ = ModifyType::Resize_Y;
	}
	else if (isUpperRightBorder(point))
	{
		modifyType_ = ModifyType::Resize_YAndWidth;
	}
	else if (isRightCenterBorder(point))
	{
		modifyType_ = ModifyType::Resize_Width;
	}
	else if (isBottomRightBorder(point))
	{
		modifyType_ = ModifyType::Resize_HeigthAndWidth;
	}
	else if (isBottomCenterBorder(point))
	{
		modifyType_ = ModifyType::Resize_Height;
	}
	else if (isBottomLeftBorder(point))
	{
		modifyType_ = ModifyType::Resize_XAndHeight;
	}
	else if (isLeftCenterBorder(point))
	{
		modifyType_ = ModifyType::Resize_X;
	}
	else if (isUpperLeftBorder(point))
	{
		modifyType_ = ModifyType::Resize_XAndY;
	}
}

void FigureRectangle::resize(const QPoint& point)
{
	if (modifyType_ == ModifyType::Resize_Y || modifyType_ == ModifyType::Resize_XAndY || modifyType_ == ModifyType::Resize_YAndWidth)
	{
		resizeByUpperCenter(point.y());
	}
	if (modifyType_ == ModifyType::Resize_X || modifyType_ == ModifyType::Resize_XAndY || modifyType_ == ModifyType::Resize_XAndHeight)
	{
		resizeByLeftCenter(point.x());
	}
	if (modifyType_ == ModifyType::Resize_Width || modifyType_ == ModifyType::Resize_HeigthAndWidth || modifyType_ == ModifyType::Resize_YAndWidth)
	{
		resizeByRightCenter(point.x());
	}
	if (modifyType_ == ModifyType::Resize_Height || modifyType_ == ModifyType::Resize_HeigthAndWidth || modifyType_ == ModifyType::Resize_XAndHeight)
	{
		resizeByBottomCenter(point.y());
	}
}

void FigureRectangle::resize(const QPoint& point, const QSize& boundingSize)
{
	QPoint corectedPoint{ point };
	if (corectedPoint.x() < 0.0)
		corectedPoint.setX(0.0);
	if (corectedPoint.y() < 0.0)
		corectedPoint.setY(0.0);
	if (corectedPoint.x() > boundingSize.width())
		corectedPoint.setX(boundingSize.width() - 1);
	if (corectedPoint.y() > boundingSize.height())
		corectedPoint.setY(boundingSize.height() - 1);

	if (modifyType_ == ModifyType::Resize_Y || modifyType_ == ModifyType::Resize_XAndY || modifyType_ == ModifyType::Resize_YAndWidth)
	{
		resizeByUpperCenter(corectedPoint.y());
	}
	if (modifyType_ == ModifyType::Resize_X || modifyType_ == ModifyType::Resize_XAndY || modifyType_ == ModifyType::Resize_XAndHeight)
	{
		resizeByLeftCenter(corectedPoint.x());
	}
	if (modifyType_ == ModifyType::Resize_Width || modifyType_ == ModifyType::Resize_HeigthAndWidth || modifyType_ == ModifyType::Resize_YAndWidth)
	{
		resizeByRightCenter(corectedPoint.x());
	}
	if (modifyType_ == ModifyType::Resize_Height || modifyType_ == ModifyType::Resize_HeigthAndWidth || modifyType_ == ModifyType::Resize_XAndHeight)
	{
		resizeByBottomCenter(corectedPoint.y());
	}
}

void FigureRectangle::move(const QPoint& delta)
{
	x_ += delta.x();
	y_ += delta.y();
}

void FigureRectangle::move(const QPoint& delta, const QSize& boundingSize)
{
	x_ += delta.x();
	if (x_ < 0)
		x_ = 0;
	else if (x_ + width_ > boundingSize.width())
		x_ = boundingSize.width() - width_ - 1;

	y_ += delta.y();
	if (y_ < 0)
		y_ = 0;
	else if (y_ + heidth_ > boundingSize.height())
		y_ = boundingSize.height() - heidth_ - 1;
}

void FigureRectangle::resizeByUpperCenter(const float y)
{
	float oldHeidth{ heidth_ };
	heidth_ += y_ - y;
	if (heidth_ < 1.0)
		heidth_ = 1.0;
	y_ += oldHeidth - heidth_;
}

void FigureRectangle::resizeByBottomCenter(const float y)
{
	heidth_ = y - y_;
	if (heidth_ < 1.0)
		heidth_ = 1.0;
}

void FigureRectangle::resizeByLeftCenter(const float x)
{
	float oldWidth{ width_ };
	width_ += x_ - x;
	if (width_ < 1.0)
		width_ = 1.0;
	x_ += oldWidth - width_;
}

void FigureRectangle::resizeByRightCenter(const float x)
{
	width_ = x - x_;
	if (width_ < 1.0)
		width_ = 1.0;
}

void FigureRectangle::move(const float dx, const float dy)
{
	x_ -= dx;
	y_ -= dy;
}

void FigureRectangle::setRectangle(float const x, float const y, float const width, float const heidth)
{
	if (width < 1.0 || heidth < 1.0)
		throw "incorect size";
	else
	{
		x_ = x;
		y_ = y;
		width_ = width;
		heidth_ = heidth;
	}
}

float FigureRectangle::getX() const
{
	return x_;
}

float FigureRectangle::getY() const
{
	return y_;
}

float FigureRectangle::getWidth() const
{
	return width_;
}

float FigureRectangle::getHeidth() const
{
	return heidth_;
}

IFigure* CreateRectangel(float const x, float const y, float const width, float const heidth)
{
	return static_cast<IFigure*>(new FigureRectangle(x, y, width, heidth));
}
FigureRectangle* Rectangel(float const x, float const y, float const width, float const heidth)
{
	return new FigureRectangle(x, y, width, heidth);
}
