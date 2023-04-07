#pragma once
#include "qlabel.h"
#include <qdebug.h>
#include <qevent.h>
#include <qpainter.h>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

enum class ImageFormat
{
	ORIGINAL = 0,
	GRAY = 0,
	RGB = 0
};

class myLabel : public QLabel
{
protected:
	QPixmap originalImage_{};
	QPixmap curentImage_{};
	QPixmap imageBuffer_{};
	
	QSize originalImageSize_{};
	QPoint drawingPoint_{ 0,0 };
	QSize drawingSize_{};
	QSize scaledImageSize_{};
	
	QSize previousImageSize_{};
	
	double imageScale_{ 1.0 };
	double normalImageScale_{ 0.0 };

	bool imageIsShown_{ false };
	
	Qt::AspectRatioMode aspectRatiotMode_{ Qt::KeepAspectRatio };
	QPoint cursorPosition_{ 0,0 };
	QPoint firstCursorPosition_{ 0,0 };

	ImageFormat imageFormat_{ ImageFormat::ORIGINAL };

	void setNormalImageScale();
	Q_OBJECT
public:
	explicit myLabel(QWidget *parent = 0);
	~myLabel();
	
	void setImage(const QPixmap& image);
	void updateImage(const QPixmap& image);
	
	void setImageScale(double const scale);
	void showPartImage();
	void moveIamge();
	
	void setDrawingPoint(const QPoint& point);
	void setFormatImage(ImageFormat formatType = ImageFormat::ORIGINAL);

	void drawPicture(const cv::Mat& drawPicture, const QRect& limitRect);
	
	
	void convertPointToImageCoordinate(QPoint& targetPoint) const;
	QPoint getDrawingPoint() const;
	QPoint getCursorPositionOnLabel() const;
	QPoint getCursorPositionOnImage() const;

	QSize getPreviousImageSize() const;
	QSize getScaledImageSize() const;
	QSize getOriginalImageSize() const;

	double getImageScale() const;
	double getNormalImageScale() const;
	
	const QPixmap& getCurentImage() const;
	const QPixmap& getOrignalImage() const;
	
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void leaveEvent(QMouseEvent* event);
	void resizeEvent(QResizeEvent* event);

signals:
	void mousePressed();
	void mousePos();
	void mouseLeft();
	void mouseRelease();
};

double round(double InputNumber, int const accuracy);