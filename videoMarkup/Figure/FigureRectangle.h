#pragma once
#include "IFigure.h"
class FigureRectangle : public IFigure
{
private:
    float x_{};
    float y_{};
    float width_{ 1.0 };
    float heidth_{ 1.0 };

    float deltaInsideBorder_{ 1.0 };
    float deltaOutsideBorder_{ 5.0 };

    QPen pen_{ QBrush{Qt::red}, 2 };
    FigureRectangle(float const x, float const y, float const width, float const heidth);
    ModifyType modifyType_{};
protected:
    ~FigureRectangle() {};
    
    void resizeByUpperCenter(const float y);
    void resizeByBottomCenter(const float y);
    void resizeByLeftCenter(const float x);
    void resizeByRightCenter(const float x);

    void move(const float dx, const float dy);

public:
    void Delete() override;
    void draw(QPixmap& background) const override;
    bool contains(const QPoint& point) const override;
    bool containsToModifyArea(const QPoint& point) const override;
    bool isUpperCenterBorder(const QPoint& point) const override;
    bool isUpperLeftBorder(const QPoint& point)const override;
    bool isUpperRightBorder(const QPoint& point) const override;
    bool isBottomCenterBorder(const QPoint& point) const override;
    bool isBottomLeftBorder(const QPoint& point) const override;
    bool isBottomRightBorder(const QPoint& point) const override;
    bool isRightCenterBorder(const QPoint& point) const override;
    bool isLeftCenterBorder(const QPoint& point) const override;
    float getArea() const override;

    void prepareForModify(const QPoint& point);
    void resize(const QPoint& point) override;
    void resize(const QPoint& point, const QSize& boundingSize) override;

    void move(const QPoint& delta) override;
    void move(const QPoint& delta, const QSize& boundingSize) override;

    void setRectangle(float const x, float const y, float const width, float const heidth);

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeidth() const;

    friend IFigure* CreateRectangel(float const x, float const y, float const width, float const heidth);
    friend FigureRectangle* Rectangel(float const x, float const y, float const width, float const heidth);
};