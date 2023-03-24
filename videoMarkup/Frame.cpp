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
