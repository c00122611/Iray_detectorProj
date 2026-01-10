#include<QImage.h>
#include "ImageUtil.h"

QImage MatToQImage(const cv::Mat& mat) {
    if (mat.empty() || mat.type() != CV_16UC1) return QImage();

    double minVal, maxVal;
    cv::minMaxLoc(mat, &minVal, &maxVal);
    double range = maxVal - minVal;
    if (range == 0) range = 1;

    cv::Mat normMat;
    mat.convertTo(normMat, CV_8UC1, 255.0 / range, -minVal * 255.0 / range);
    return QImage(normMat.data, normMat.cols, normMat.rows, normMat.step, QImage::Format_Grayscale8).copy();
}