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
	

	double widthScalingCoefficient_{};
	double heightScalingCoefficient_{};
	double imageScale_{ 1.0 };
	double normalImageScale_{ 0.0 };

	bool imageIsShown_{ false };
	
	Qt::AspectRatioMode aspectRatiotMode_{ Qt::KeepAspectRatio };
	QPoint cursorPosition_{ 0,0 };
	QPoint firstCursorPosition_{ 0,0 };

	ImageFormat imageFormat_{ ImageFormat::ORIGINAL };

	void setNormalImageScaled();
	Q_OBJECT
public:
	explicit myLabel(QWidget *parent = 0);
	~myLabel();
	
	void setImage(const QPixmap& image);
	void updateImage(const QPixmap& img);
	
	void setImageScale(double const scale);

	//void showPartImage(int xPart, int yPart, int const widthPart, int const heightPart);
	void showPartImage();
	
	QPoint getDrawingPoint() const;
	void moveImg(int& out_dx, int& out_dy);
	

	void toCenterPoint(QPoint &upLeftPoint);
	void toUpLeftpoint(QPoint &centerPoint);
	void toUpLeftpoint(int &x, int &y);
	
	QPoint getImagePoint() const;
	QSize getOldSize() const;
	QSize getScaledImgSize() const;
	QSize getOriginalImgSize() const;

	void getPointInImg(int& x, int& y);
	void toImgCoordinate(int &inOutX, int &inOutY, bool isContains=true);
	void toImgCoordinate_(int &inOutX, int &inOutY, bool isContains=true);
	QPoint getImageCoordinate(bool isContains=true);
	void formatImage(ImageFormat formatType = ImageFormat::ORIGINAL);
	const QPixmap& getImage() const;
	
	double getImageScaled() const;
	double getNormalImageScaled() const;
	
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void leaveEvent(QMouseEvent* event);
	void resizeEvent(QResizeEvent* event);
	
	
	void drawPicture(const cv::Mat& drawPicture, const QRect& limitRect);

signals:
	void mousePressed();
	void mousePos();
	void mouseLeft();
	void mouseRelease();
};

double round(double InputNumber, int const accuracy);