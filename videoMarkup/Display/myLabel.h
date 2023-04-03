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
	QPixmap curentImage_{};
	QPixmap originalImage_{};
	QPixmap imageBuffer_{};
	QPixmap* imageMouvePart_{ nullptr };

	QPoint drawingPoint_{ 0,0 };
	QPoint originalDrawingPoint_{ 0,0 };
	QSize originalImageSize_{};
	QSize scaledImageSize_{};
	QSize previousImageSize_{};

	double widthScalingCoefficient_{};
	double heightScalingCoefficient_{};
	//double scaleds_[11]{0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 2, 3, 5, 0};
	//size_t activScaled_{ 4 };
	double imageScaled_{ 1.0 };
	double normalImageScaled_{ 0.0 };

	bool imageIsShown_{ false };
	
	Qt::AspectRatioMode aspectRatiotMode_{ Qt::KeepAspectRatio };
	QPoint cursorPosition_{ 0,0 };
	QPoint firstCursorPosition_{ 0,0 };

	ImageFormat imageFormat_{ ImageFormat::ORIGINAL };

	Q_OBJECT
public:
	explicit myLabel(QWidget *parent = 0);
	~myLabel();
	
	void setImage(const QPixmap& image);
	void updateImage(const QPixmap& img);
	
	
	
	
	void draw_picture(const cv::Mat& drawPicture, const QRect& limitRect);

	void show_partImg(int dx, int dy, int const width, int const height);
	void show_partImg();
	
	QPoint getDrawingPoint() const;
	void moveImg(int& out_dx, int& out_dy);
	double scaledPixmap(int const scaled, int &dx, int &dy);
	void scaledPixmap();

	void toCenterPoint(QPoint &upLeftPoint);
	void toUpLeftpoint(QPoint &centerPoint);
	void toUpLeftpoint(int &x, int &y);
	
	QPoint getImagePoint() const;
	QSize getOldSize() const;
	QSize getScaledImgSize() const;
	QSize getOriginalImgSize() const;
	void setNormalImageScaled();
	void getPointInImg(int& x, int& y);
	void toImgCoordinate(int &inOutX, int &inOutY, bool isContains=true);
	void toImgCoordinate_(int &inOutX, int &inOutY, bool isContains=true);
	QPoint getImageCoordinate(bool isContains=true);
	void formatImage(ImageFormat formatType = ImageFormat::ORIGINAL);
	const QPixmap& getImage() const;
	
	double get_activScaled();
	
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