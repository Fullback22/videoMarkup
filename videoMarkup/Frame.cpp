#include "Frame.h"

Frame::Frame()
{
}

Frame::Frame(const cv::Mat& inputMat, const QPixmap& inputPixmap, const bool imageIsNull):
	originalMat_{ inputMat },
	originalPixmap_{ inputPixmap },
	imageIsNull_{ imageIsNull }
{
}

Frame::Frame(const QPixmap& inputPixmap, const bool imageIsNull):
	originalPixmap_{ inputPixmap },
	imageIsNull_{ imageIsNull }
{
	originalMat_ = cv::Mat(inputPixmap.height(), inputPixmap.width(), CV_8UC3, inputPixmap.toImage().bits(), inputPixmap.toImage().bytesPerLine());
}

Frame::Frame(const cv::Mat& inputMat, const bool imageIsNull):
	originalMat_{ inputMat },
	imageIsNull_{ imageIsNull }
{
	originalPixmap_ = QPixmap::fromImage(QImage(inputMat.data, inputMat.cols, inputMat.rows, inputMat.step, QImage::Format_RGB888));
}

Frame::Frame(const Frame& dep):
	originalMat_{ dep.originalMat_ },
	originalPixmap_{ dep.originalPixmap_ },
	imageIsNull_{ dep.imageIsNull_ }
{
}

const cv::Mat& Frame::getMat() const
{
	return originalMat_;
}

const QPixmap& Frame::getPixmap() const
{
	return originalPixmap_;
}

bool Frame::frameIsNull() const
{
	return imageIsNull_;
}

void Frame::updateFrame(const cv::Mat& newMat, const QPixmap& newPixmap)
{
	originalMat_ = newMat;
	originalPixmap_ = newPixmap;
}

void Frame::updateFrame(const cv::Mat& newMat)
{
	originalMat_ = newMat;
	originalPixmap_ = QPixmap::fromImage(QImage(newMat.data, newMat.cols, newMat.rows, newMat.step, QImage::Format_RGB888));
}

void Frame::updateFrame(const QPixmap& newPixmap)
{
	originalPixmap_ = newPixmap;
	originalMat_ = cv::Mat(newPixmap.height(), newPixmap.width(), CV_8UC3, newPixmap.toImage().bits(), newPixmap.toImage().bytesPerLine());
}

void Frame::setStatusFrame(const bool isNull)
{
	imageIsNull_ = isNull;
}

Frame& Frame::operator=(const Frame& drop)
{
	originalMat_ = drop.originalMat_;
	originalPixmap_ = drop.originalPixmap_;
	imageIsNull_ = drop.imageIsNull_;
	return *this;
}
