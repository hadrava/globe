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
  double lon = spherical.x / 10800 * PI;
  double lat = spherical.y / 10800 * PI;

  double x = cos(lat)*cos(lon);
  double y = cos(lat)*sin(lon);
  double z = sin(lat);

  double a = x*cos(-params->longmin/10800*PI) - y*sin(-params->longmin/10800*PI);
         y = x*sin(-params->longmin/10800*PI) + y*cos(-params->longmin/10800*PI);

         x = a*cos(-params->latmin/10800*PI) - z*sin(-params->latmin/10800*PI);
         z = a*sin(-params->latmin/10800*PI) + z*cos(-params->latmin/10800*PI);


  //move to camera coordinates (and mirror)
  x-=params->distance;
  x=-x;

  double r = sqrt(x*x + y*y + z*z);

  if (r*r + 1 > params->distance*params->distance)
    return cvPoint(-1,-1);

  a = x*cos(-params->directionlatmin/10800*PI) - z*sin(-params->directionlatmin/10800*PI);
  z = x*sin(-params->directionlatmin/10800*PI) + z*cos(-params->directionlatmin/10800*PI);

  x = a*cos(-params->directionlongmin/10800*PI) - y*sin(-params->directionlongmin/10800*PI);
  y = a*sin(-params->directionlongmin/10800*PI) + y*cos(-params->directionlongmin/10800*PI);

  a   = z*cos(params->zrotationmin/10800*PI) - y*sin(params->zrotationmin/10800*PI);
  y   = z*sin(params->zrotationmin/10800*PI) + y*cos(params->zrotationmin/10800*PI);
  z   = a;

  y*=params->focallength/x;
  z*=params->focallength/x;

  x=width/2 +y+params->xshiftpix;
  y=height/2-z+params->yshiftpix;

  return cvPoint(x,y);
}
