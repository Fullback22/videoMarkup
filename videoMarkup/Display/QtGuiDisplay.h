#pragma once

#include <QWidget>
#include "ui_QtGuiDisplay.h"
#include <QTime>
#include "qfiledialog.h"
#include "myLabel.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>

#include "../Frame.h"

class QtGuiDisplay : public QWidget
{
	Q_OBJECT

	Frame frame_{};
	
	double activScaled_{};

	bool isZoomNow_{ false };
	bool moveImage{ false };

public:
	QtGuiDisplay(QWidget *parent = Q_NULLPTR);
	~QtGuiDisplay();
	void setSizeScrollBar();
	
	void setActivFrame(const Frame& activObj);
	void setEnableWidtsGrouBox(bool enable);
	
	QRect getLabelRect();
	void changeImgFormat(ImageFormat formatType);
	
	void updateFrame(const Frame& activObj);
	void resizeEvent(QResizeEvent* event);

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
	void slot_updateImg();
};
