#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <opencv2/imgproc/imgproc_c.h>
#include "image.h"

#define PI 3.14159265358979323846  /* pi */


CvPoint sph_to_sph_image(CvPoint2D64f spherical);
CvPoint2D64f sph_image_to_sph(CvPoint image);
CvPoint sph_to_image(CvPoint2D64f spherical, struct image_params *params, int width, int height);

#endif
