#ifndef affineTransform_hpp
#define affineTransform_hpp

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define PI 3.141592654f

float randomNumber0to1();
float randomNumber0tox(float x);
float randomNumberatob(float a, float b);
float degToRad(float angleDegrees);

cv::Mat getXRotation(float angle);
cv::Mat getZRotation(float angle);
cv::Mat getYRotation(float angle);
cv::Mat getScale(float sx, float sy, float sz);
cv::Mat getShear(float sxy, float sxz, float syx, float syz, float szx, float szy);

#endif