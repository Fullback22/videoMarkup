#pragma once
#include <qpixmap.h>
#include <qpainter.h>

class IFigure
{
protected:
	~IFigure() = default;

public:
	virtual void Delete() = 0;
	virtual void draw(QPixmap& background) const = 0;
	virtual bool contains(const QPoint& point) const = 0;
	virtual bool isUpperCenterBorder(const QPoint& point) const = 0;
	virtual bool isUpperLeftBorder(const QPoint& point) const = 0;
	virtual bool isUpperRightBorder(const QPoint& point) const = 0;
	virtual bool isBottomCenterBorder(const QPoint& point) const = 0;
	virtual bool isBottomLeftBorder(const QPoint& point) const = 0;
	virtual bool isBottomRightBorder(const QPoint& point) const = 0;
	virtual bool isRightCenterBorder(const QPoint& point) const = 0;
	virtual bool isLeftCenterBorder(const QPoint& point) const = 0;
	virtual float getArea() const = 0;
	IFigure& operator=(const IFigure& drop) = delete;

	static IFigure* CreateInstance();
};

