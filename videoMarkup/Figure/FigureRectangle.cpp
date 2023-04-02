#include "FigureRectangle.h"

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
	float leftX{ x_ + width_ - deltaOutsideBorder_ };
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
	float rightX{ x_ + deltaOutsideBorder_ };
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

IFigure* IFigure::CreateInstance()
{
	return static_cast<IFigure*>(new FigureRectangle());
} 