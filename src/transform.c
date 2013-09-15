#include <math.h>
#include <opencv2/imgproc/imgproc_c.h>
#include "params.h"
#include "transform.h"
#include "image.h"

CvPoint sph_to_sph_image(CvPoint2D64f spherical){
  CvPoint image = cvPoint(par_sph_width * spherical.x / 21600,
                          par_sph_height/2 - par_sph_height * spherical.y / 10800);
  return image;
}

CvPoint2D64f sph_image_to_sph(CvPoint image){
  CvPoint2D64f spherical = cvPoint2D64f(21600 * image.x / par_sph_width,
                                        (par_sph_height/2 - image.y) * 10800 / par_sph_height);
  return spherical;
}

void sph_to_image_precalculate_projection(const struct image_params *params, int width, int height, struct projection_params *proj_params) {
  proj_params->c_lon1 = cos(-params->longmin/10800*PI);
  proj_params->s_lon1 = sin(-params->longmin/10800*PI);
  proj_params->c_lat1 = cos(-params->latmin/10800*PI);
  proj_params->s_lat1 = sin(-params->latmin/10800*PI);
  proj_params->c_lat2 = cos(-params->directionlatmin/10800*PI);
  proj_params->s_lat2 = sin(-params->directionlatmin/10800*PI);
  proj_params->c_lon2 = cos(-params->directionlongmin/10800*PI);
  proj_params->s_lon2 = sin(-params->directionlongmin/10800*PI);
  proj_params->c_rot1 = cos(params->zrotationmin/10800*PI);
  proj_params->s_rot1 = sin(params->zrotationmin/10800*PI);
  proj_params->img_center_x = width/2 + params->xshiftpix;
  proj_params->img_center_y = height/2 + params->yshiftpix;
}

CvPoint sph_to_image(CvPoint2D64f spherical, const struct image_params *im_params, const struct projection_params *params) {
  double lon = spherical.x / 10800 * PI;
  double lat = spherical.y / 10800 * PI;

  double c_lat = cos(lat); //small optimization
  double x = c_lat*cos(lon);
  double y = c_lat*sin(lon);
  double z = sin(lat);


  double a = x*params->c_lon1 - y*params->s_lon1;
         y = x*params->s_lon1 + y*params->c_lon1;

         x = a*params->c_lat1 - z*params->s_lat1;
         z = a*params->s_lat1 + z*params->c_lat1;


  //move to camera coordinates (and mirror)
  x = -x + im_params->distance;

  if (x*x + y*y + z*z + 1 > im_params->distance*im_params->distance)
    return cvPoint(-1,-1);

  a = x*params->c_lat2 - z*params->s_lat2;
  z = x*params->s_lat2 + z*params->c_lat2;

  x = a*params->c_lon2 - y*params->s_lon2;
  y = a*params->s_lon2 + y*params->c_lon2;

  a = z*params->c_rot1 - y*params->s_rot1;
  y = z*params->s_rot1 + y*params->c_rot1;

  y *= im_params->focallength/x;
  z = a * im_params->focallength/x;

  return cvPoint(y + params->img_center_x, -z + params->img_center_y);
}

CvPoint sph_to_image_slow(CvPoint2D64f spherical, const struct image_params *im_params, int width, int height) {
  struct projection_params proj_params;
  sph_to_image_precalculate_projection(im_params, width, height, &proj_params);

  return sph_to_image(spherical, im_params, &proj_params);
}

CvPoint image_to_image_window(const CvPoint image, const IplImage *image_param, const IplImage *image_window_param) {
  return cvPoint(image.x * image_window_param->width / image_param->width, image.y * image_window_param->height / image_param->height);
}

CvPoint image_window_to_image(const CvPoint image_window, const IplImage *image_window_param, const IplImage *image_param) {
  return cvPoint(image_window.x * image_param->width / image_window_param->width, image_window.y * image_param->height / image_window_param->height);
}
