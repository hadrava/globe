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

CvPoint sph_to_image(CvPoint2D64f spherical, struct image_params *params, int width, int height) {
  double lon = (spherical.x-params->longmin) /10800*PI;//does not work
  double lat = (spherical.y-params->latmin)  /10800*PI;//does not work

  double x = cos(lat)*cos(lon);
  double y = cos(lat)*sin(lon);
  double z = sin(lat);

  x-=params->distance;

  double r = sqrt(x*x + y*y + z*z);
  lon = atan(-y/x);
  lat = asin(z/r);

  if (r*r + 1 > params->distance*params->distance)
    return cvPoint(-1,-1);

  lon -= params->directionlongmin;//does not work
  lat -= params->directionlatmin;//does not work

  x = cos(lat)*cos(lon);
  y = cos(lat)*sin(lon);
  z = sin(lat);

  lat = z*cos(params->zrotationmin) - y*sin(params->zrotationmin);
  y   = z*sin(params->zrotationmin) + y*cos(params->zrotationmin);
  z   = lat;

  y*=params->focallength/x;
  z*=params->focallength/x;

  x=width/2 +y+params->xshiftpix;
  y=height/2-z+params->yshiftpix;


  CvPoint image = cvPoint(x,y);
  return image;
}
