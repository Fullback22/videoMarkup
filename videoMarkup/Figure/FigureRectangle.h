#pragma once
#include "IFigure.h"
class FigureRectangle :
    protected IFigure
{
private:
    float x_{};
    float y_{};
    float width_{ 1.0 };
    float heidth_{ 1.0 };

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

    void resizeByUpperCenter(const float dy);
    void resizeByBottomCenter(const float dy);
    void resizeByLeftCenter(const float dx);
    void resizeByRightCenter(const float dx);
    void resizeByUpperLeft(const float dx, const float dy);
    void resizeByUpperRight(const float dx, const float dy);
    void resizeByBottomLeft(const float dx, const float dy);
    void resizeByBottomRight(const float dx, const float dy);
     
    void move(const float dx, const float dy);

    void setRectangle(float const x, float const y, float const width, float const heidth);

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeidth() const;

    friend IFigure* IFigure::CreateInstance();
};

