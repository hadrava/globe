#ifndef FIT_H_
#define FIT_H_

#include <opencv2/imgproc/imgproc_c.h>

struct fit_point_list {
  CvPoint globe_position_min;
  CvPoint in_image;
  int star;
  struct fit_point_list *next;
  struct fit_point_list *prev;
};

extern struct image_list *fit_active;
extern struct fit_point_list *fit_points;;

void fit_add_point(CvPoint globe_position_min, CvPoint in_image);
void fit_add_point_star(CvPoint globe_position_min, CvPoint in_image, int star);
struct fit_point_list *fit_find_star(int star);
struct fit_point_list * fit_remove_point(struct fit_point_list *point, struct fit_point_list *list);

#endif
