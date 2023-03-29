#pragma once
#include "qpixmap.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

class Frame
{
    cv::Mat originalMat_{};
    QPixmap originalPixmap_{};
    bool imageIsNull_{ true };
public:
    Frame();
    Frame(const cv::Mat& inputMat, const QPixmap& inputPixmap, const bool imageIsNull = false);
    Frame(const QPixmap& inputPixmap, const bool imageIsNull = false);
    Frame(const cv::Mat& inputMat, const bool imageIsNull = false);
    Frame(const Frame& dep);
    Frame& operator=(const Frame& drop);
    
    const cv::Mat& getMat() const;
    const QPixmap& getPixmap() const;
    bool frameIsNull() const;

    void updateFrame(const cv::Mat& newMat, const QPixmap& newPixmap);
    void updateFrame(const cv::Mat& newMat);
    void updateFrame(const QPixmap& newPixmap);
    void setStatusFrame(const bool isNull);
};

