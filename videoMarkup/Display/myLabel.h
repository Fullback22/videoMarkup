#pragma once
#include <qlabel.h>
#include <qevent.h>
#include <qpainter.h>

#include"..\Figure\IFigure.h"

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
	QPixmap imageWithStaticFigure_{};
	
	QSize originalImageSize_{};
	QPoint drawingPoint_{ 0,0 };
	QSize drawingSize_{};
	QSize scaledImageSize_{};
	
	QSize previousImageSize_{};
	
	float imageScale_{ 1.0 };
	float normalImageScale_{ 1.0 };
	float coefficientRecalculationByWidth_{ 1.0 };
	float coefficientRecalculationByHeigth_{ 1.0 };
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
	void updateImage(const QPixmap& image);
	
	void setImageScale(float const scale);
	void showPartImage();
	void moveIamge();
	
	void setDrawingPoint(const QPoint& point);
	void setFormatImage(ImageFormat formatType = ImageFormat::ORIGINAL);

	void drawPicture(const QImage& drawPicture, const QRect& limitRect);
	void drawStaticFigure(const IFigure* figure);
	void drawStaticFigure(const std::vector<IFigure*>& figures);
	void drawDynamicFigure(const IFigure* figure);
	void clearImageFromFigure();
	
	void convertPointToImageCoordinate(QPoint& targetPoint) const;
	QPoint getDrawingPoint() const;
	QPoint getCursorPositionOnLabel() const;
	QPoint getCursorPositionOnImage() const;
	QPoint& getDeltaOnImageCoordinate();

	QSize getPreviousImageSize() const;
	QSize getScaledImageSize() const;
	QSize getOriginalImageSize() const;
	QSize getDrawingSize() const;

	float getImageScale() const;
	float getNormalImageScale() const;
	float getCoefficientRecalculationByWidth() const;
	float getCoefficientRecalculationByHeigth() const;

	const QPixmap& getCurentImage() const;
	const QPixmap& getOrignalImage() const;

signals:
	void mousePressed();
	void mousePos();
	void mouseLeft();
	void mouseRelease();

protected:
	void setNormalImageScale();

	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void leaveEvent(QMouseEvent* event);
	void resizeEvent(QResizeEvent* event);
};

float round(float InputNumber, int const accuracy);