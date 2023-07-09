#pragma once

#include <QWidget>
#include <qfiledialog.h>

#include "ui_QtGuiDisplay.h"

#include "myLabel.h"
#include "../Frame.h"
#include "../Figure/FigureRectangle.h"

class QtGuiDisplay : public QWidget
{
	Q_OBJECT
protected:
	Frame frame_{};
	QCursor cursorOnDisplay_{ Qt::ArrowCursor };

	float scale_[10]{ 1, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 2, 3, 5 };
	size_t activScale_{ 0 };
	size_t scaleFollowingNormalScale_{ 10 };
	size_t scalePreviousToNormalScale_{ 0 };
	
	bool canMoveImage_{ false };
	bool canChangesFigure_{ true };
	bool figureIsChanging_{ false };

	std::vector<IFigure*> figuresForDrawing_;
	int activFigure_{ -1 };
	int prepareForModifyFigure_{ -1 };


public:
	QtGuiDisplay(QWidget *parent = Q_NULLPTR);
	~QtGuiDisplay();
	
	void setActivFrame(const Frame& activObj);
	void setEnableWidtsGrouBox(bool enable);
	
	void addRectangel(FigureRectangle* rectangel);

	QRect getLabelRect();
	void changeImgFormat(ImageFormat formatType);
	
	void updateFrame(const Frame& activObj);
	void resizeEvent(QResizeEvent* event);

	void setActivFigure(unsigned int const index);
	void deleteFigure(int const index);

	void updateImage();
private:
	Ui::QtGuiDisplay ui;

protected slots:
	void slot_mouvePixmap();
	void slot_mouseCurrentPos();
	void slot_mousePressed();
	void slot_mouseRelease();
	void slot_ZoomImg_In();
	void slot_ZoomImg_Out();
	void slot_ZoomImg_AllLabl();

protected:
	void setScalesBorderingWithNormalScale();
	void setSizeScrollBar();
	Qt::CursorShape determenateCursorViewOnFigure(const IFigure* figure, const QPoint& position);
	int getFigureIndexPrepareForModify(const QPoint& position);
	void modifyFigure();
	void preparationForModifyFigure();
	void moveImage();
	void drawAllFigure();

signals:
	void newActivFigure(int index);
};
