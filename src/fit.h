#ifndef FIT_H_
#define FIT_H_

#include <opencv2/imgproc/imgproc_c.h>

struct fit_point_list {
  CvPoint globe_position_min;
  CvPoint in_image;
  struct fit_point_list *next;
};

extern struct image_list *fit_active;
extern struct fit_point_list *fit_points;;

void fit_add_point(CvPoint globe_position_min, CvPoint in_image);

#endif
