#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "params.h"
#include "transform.h"

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
