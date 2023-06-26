#pragma once
#include <qpixmap.h>
#include <qpainter.h>

enum ModifyType
{
	Move,
	Resize_X,
	Resize_Y,
	Resize_Width,
	Resize_Height,
	Resize_XAndY,
	Resize_HeigthAndWidth,
	Resize_XAndHeight,
	Resize_YAndWidth
};

class IFigure
{
protected:
	~IFigure() = default;

public:
	virtual void Delete() = 0;
	virtual void draw(QPixmap& background) const = 0;
	virtual bool contains(const QPoint& point) const = 0;
	virtual bool containsToModifyArea(const QPoint& point) const = 0;
	virtual float getArea() const = 0;

	virtual bool isUpperCenterBorder(const QPoint& point) const = 0;
	virtual bool isUpperLeftBorder(const QPoint& point) const = 0;
	virtual bool isUpperRightBorder(const QPoint& point) const = 0;
	virtual bool isBottomCenterBorder(const QPoint& point) const = 0;
	virtual bool isBottomLeftBorder(const QPoint& point) const = 0;
	virtual bool isBottomRightBorder(const QPoint& point) const = 0;
	virtual bool isRightCenterBorder(const QPoint& point) const = 0;
	virtual bool isLeftCenterBorder(const QPoint& point) const = 0;
	
	virtual void prepareForModify(const QPoint& point) = 0;
	virtual void resize(const QPoint& point) = 0;
	virtual void resize(const QPoint& point, const QSize& boundingSize) = 0;
	
	virtual void move(const QPoint& delta) = 0;
	virtual void move(const QPoint& delta, const QSize& boundingSize) = 0;
	
	IFigure& operator=(const IFigure& drop) = delete;

	
};

