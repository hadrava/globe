#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <opencv2/imgproc/imgproc_c.h>

CvPoint sph_to_sph_image(CvPoint2D64f spherical);
CvPoint2D64f sph_image_to_sph(CvPoint image);

#endif
