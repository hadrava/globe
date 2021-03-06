#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <opencv2/imgproc/imgproc_c.h>
#include "image.h"
#include "stereographical.h"

#define PI 3.14159265358979323846  /* pi */

struct projection_params {
  double c_lon1;
  double s_lon1;
  double c_lat1;
  double s_lat1;
  double c_lon2;
  double s_lon2;
  double c_lat2;
  double s_lat2;
  double c_rot1;
  double s_rot1;
  int img_center_x;
  int img_center_y;
};

struct stereo_projection_params {
  int width;
  int height;
  int img_center_x;
  int img_center_y;
  double c_rot1;
  double s_rot1;
  double c_lat1;
  double s_lat1;
  double c_lon1;
  double s_lon1;
  double minus_x1;
  double minus_y1;
  double minus_z1;
};

CvPoint sph_to_sph_image(CvPoint2D64f spherical);
CvPoint2D64f sph_image_to_sph(CvPoint image);

CvPoint sph64_to_sph(CvPoint2D64f spherical);
CvPoint2D64f sph_to_sph64(CvPoint spherical);

void sph_to_image_precalculate_projection(const struct image_params *params, int width, int height, struct projection_params *proj_params);
CvPoint sph_to_image(CvPoint2D64f spherical, const struct image_params *im_params, const struct projection_params *params);
CvPoint sph_to_image_slow(CvPoint2D64f spherical, const struct image_params *im_params, int width, int height);

CvPoint image_to_image(const CvPoint image, const IplImage *src_param, const IplImage *dest_param);

void stereographical_to_sph_precalculate_projection(const struct stereographical_params *params, int width, int height, struct stereo_projection_params *proj_params);
CvPoint2D64f stereographical_to_sph(CvPoint stereographical, const struct stereographical_params *params, const struct stereo_projection_params *proj_params);
CvPoint2D64f stereographical_to_sph_slow(CvPoint stereographical, const struct stereographical_params *params, int width, int height);

CvPoint stereographical_to_image(CvPoint stereographical, const struct stereographical_params *s_params, const struct stereo_projection_params *proj_params, const struct image_params *im_params, const struct projection_params *params);

CvPoint sph_to_stereographical_slow(CvPoint2D64f spherical, const struct stereographical_params *params, int width, int height);

#endif
