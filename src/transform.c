#include <math.h>
#include <opencv2/imgproc/imgproc_c.h>
#include "params.h"
#include "transform.h"
#include "image.h"
#include <stdio.h>
//TODO remove stdio

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

CvPoint sph64_to_sph(CvPoint2D64f spherical) {
  return cvPoint(spherical.x, spherical.y);
}

CvPoint2D64f sph_to_sph64(CvPoint spherical) {
  return cvPoint2D64f(spherical.x, spherical.y);
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

CvPoint image_to_image(const CvPoint image, const IplImage *src_param, const IplImage *dest_param) {
  return cvPoint(image.x * dest_param->width / src_param->width, image.y * dest_param->height / src_param->height);
}

void stereographical_to_sph_precalculate_projection (const struct stereographical_params *params, int width, int height, struct stereo_projection_params *proj_params) {
  proj_params->width = width;
  proj_params->height = height;
  proj_params->img_center_x = width/2;
  proj_params->img_center_y = height/2;

  proj_params->c_rot1 = cos(params->zrotationmin);
  proj_params->s_rot1 = sin(params->zrotationmin);
  proj_params->c_lat1 = cos(params->latmin);
  proj_params->s_lat1 = sin(params->latmin);
  proj_params->c_lon1 = cos(params->longmin);
  proj_params->s_lon1 = sin(params->longmin);

  proj_params->minus_x1 = proj_params->c_lat1 * proj_params->s_lon1;
  proj_params->minus_y1 = proj_params->s_lat1;
  proj_params->minus_z1 = proj_params->c_lat1 * proj_params->c_lon1;
}

CvPoint2D64f stereographical_to_sph(CvPoint stereographical, const struct stereographical_params *params, const struct stereo_projection_params *proj_params) {
  double x1,y1,z1,x2,y2,z2;

  //adapt image size
  x1 =   (stereographical.x - proj_params->img_center_x ) / params->image_size_factor;
  y1 = - (stereographical.y - proj_params->img_center_y) / params->image_size_factor;

  //image rotation
  x2 = x1*proj_params->c_rot1 - y1*proj_params->s_rot1;
  y2 = x1*proj_params->s_rot1 + y1*proj_params->c_rot1;

  //move to desired latitude
  z1 = proj_params->c_lat1 - y2*proj_params->s_lat1;
  y2 = proj_params->s_lat1 + y2*proj_params->c_lat1;

  //move to desired longitude
  z2 = z1*proj_params->c_lon1 - x2*proj_params->s_lon1;
  x2 = z1*proj_params->s_lon1 + x2*proj_params->c_lon1;

  z1 = proj_params->minus_z1;
  x1 = proj_params->minus_x1;
  y1 = proj_params->minus_y1;

  //calculate vector projectionpoint -> imagepoint
  x2 += x1;
  y2 += y1;
  z2 += z1;

  //project vector -(1) to (2)
  double scal = (x1*x2) + (y1*y2) + (z1*z2);
  //divide by norm (2)
  scal /= x2*x2 + y2*y2 + z2*z2;

  //calculate position on sphere
  x1 = x2*2*scal - x1;
  y1 = y2*2*scal - y1;
  z1 = z2*2*scal - z1;

  //calculate angles
  double lat=asin(y1);
  double lon;
  if (z1>=0 && x1>=0)
    lon = atan(x1/z1);
  else if(z1<0 && x1>=0)
    lon = atan(-z1/x1) + PI/2;
  else if(z1<0 && x1<0)
    lon = atan(x1/z1) + PI;
  else //if(z1>=0 && x1<0)
    lon = atan(-z1/x1) + 3*PI/2;

  return cvPoint2D64f(lon / PI * 10800, lat / PI * 10800);
}

CvPoint2D64f stereographical_to_sph_slow(CvPoint stereographical, const struct stereographical_params *params, int width, int height) {
  double x1,y1,z1,x2,y2,z2,x3,y3,z3;

  //adapt image size
  x1 =   (stereographical.x - width/2 ) / params->image_size_factor;
  y1 = - (stereographical.y - height/2) / params->image_size_factor;

  //image rotation
  x2 = x1*cos(params->zrotationmin) - y1*sin(params->zrotationmin);
  y2 = x1*sin(params->zrotationmin) + y1*cos(params->zrotationmin);
  z2 = 1;

  //move to desired latitude
  x1 = x2;
  z1 = z2*cos(params->latmin) - y2*sin(params->latmin);
  y1 = z2*sin(params->latmin) + y2*cos(params->latmin);

  //move to desired longitude
  z2 = z1*cos(params->longmin) - x1*sin(params->longmin);
  x2 = z1*sin(params->longmin) + x1*cos(params->longmin);
  y2 = y1;


  //calculate projection point
  x1 = 0;
  y1 = 0;
  z1 = -1;

  //move to desired latitude //WARN: duplicate code
  x3 = x1;
  z3 = z1*cos(params->latmin) - y1*sin(params->latmin);
  y3 = z1*sin(params->latmin) + y1*cos(params->latmin);

  //move to desired longitude //WARN: duplicate code
  z1 = z3*cos(params->longmin) - x3*sin(params->longmin);
  x1 = z3*sin(params->longmin) + x3*cos(params->longmin);
  y1 = y3;


  //calculate vector projectionpoint -> imagepoint
  x2 -= x1;
  y2 -= y1;
  z2 -= z1;

  //project vector -(1) to (2)
  double scal = (-x1*x2) + (-y1*y2) + (-z1*z2);
  //divide by norm (2)
  scal /= x2*x2 + y2*y2 + z2*z2;

  //calculate position on sphere
  x1 = x1 + x2*2*scal;
  y1 = y1 + y2*2*scal;
  z1 = z1 + z2*2*scal;

  //calculate angles
  double lat=asin(y1);
  double lon;
  if (z1>=0 && x1>=0)
    lon = atan(x1/z1);
  else if(z1<0 && x1>=0)
    lon = atan(-z1/x1) + PI/2;
  else if(z1<0 && x1<0)
    lon = atan(x1/z1) + PI;
  else //if(z1>=0 && x1<0)
    lon = atan(-z1/x1) + 3*PI/2;

  return cvPoint2D64f(lon / PI * 10800, lat / PI * 10800);
}

CvPoint stereographical_to_image(CvPoint stereographical, const struct stereographical_params *s_params, const struct stereo_projection_params *proj_params, const struct image_params *im_params, const struct projection_params *params) {
  double x1,y1,z1,x2,y2,z2;

  //adapt image size
  x1 =   (stereographical.x - proj_params->img_center_x ) / s_params->image_size_factor;
  y1 = - (stereographical.y - proj_params->img_center_y) / s_params->image_size_factor;

  //image rotation
  x2 = x1*proj_params->c_rot1 - y1*proj_params->s_rot1;
  y2 = x1*proj_params->s_rot1 + y1*proj_params->c_rot1;

  //move to desired latitude
  z1 = proj_params->c_lat1 - y2*proj_params->s_lat1;
  y2 = proj_params->s_lat1 + y2*proj_params->c_lat1;

  //move to desired longitude
  z2 = z1*proj_params->c_lon1 - x2*proj_params->s_lon1;
  x2 = z1*proj_params->s_lon1 + x2*proj_params->c_lon1;

  z1 = proj_params->minus_z1;
  x1 = proj_params->minus_x1;
  y1 = proj_params->minus_y1;

  //calculate vector projectionpoint -> imagepoint
  x2 += x1;
  y2 += y1;
  z2 += z1;

  //project vector -(1) to (2)
  double scal = (x1*x2) + (y1*y2) + (z1*z2);
  //divide by norm (2)
  scal /= x2*x2 + y2*y2 + z2*z2;

  //map position to sphere to imagee function
  double x = z2*2*scal - z1;
  double y = x2*2*scal - x1;
  double z = y2*2*scal - y1;

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
