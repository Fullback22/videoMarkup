#pragma once
#include "IFigure.h"
class FigureRectangle :
    protected IFigure
{
private:
    float x_{};
    float y_{};
    float width_{};
    float heidth_{};

    float deltaInsideBorder_{ 0 };
    float deltaOutsideBorder_{ 5.0 };

    QPen pen_{ QBrush{Qt::red}, 1 };

    FigureRectangle() {};
protected:
    ~FigureRectangle() {};
    void Delete() override;
    void draw(QPixmap& background) const override;
    bool contains(const QPoint& point) const override;
    bool isUpperCenterBorder(const QPoint& point) const override;
    bool isUpperLeftBorder(const QPoint& point)const override;
    bool isUpperRightBorder(const QPoint& point) const override;
    bool isBottomCenterBorder(const QPoint& point) const override;
    bool isBottomLeftBorder(const QPoint& point) const override;
    bool isBottomRightBorder(const QPoint& point) const override;
    bool isRightCenterBorder(const QPoint& point) const override;
    bool isLeftCenterBorder(const QPoint& point) const override;
    float getArea() const override;

    friend IFigure* IFigure::CreateInstance();
};

