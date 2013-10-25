#ifndef FIT_H_
#define FIT_H_

#include <opencv2/imgproc/imgproc_c.h>
#include "image.h"

struct fit_point_list {
  CvPoint globe_position_min;
  CvPoint in_image;
  int star;
  struct fit_point_list *next;
  struct fit_point_list *prev;
};

struct objective_data {
  struct image_list *image;
  struct fit_point_list *points;
};

extern struct image_list *fit_active;

void fit_add_point(CvPoint globe_position_min, CvPoint in_image);
void fit_add_point_star(CvPoint globe_position_min, CvPoint in_image, int star);
struct fit_point_list *fit_find_star(int star);
struct fit_point_list *fit_remove_point(struct fit_point_list *point, struct fit_point_list *list);

void fit_do(struct image_list *image, struct fit_point_list *points);
double fit_objective(unsigned n, const double *x, double *grad, void *f_data);
double fit_error(struct image_list *image, struct fit_point_list *points, struct image_params *im_params);

struct fit_point_list *fit_load_points_from_file(char *name);
void fit_make_active(struct image_list *image);
void fit_save_points_to_file(char *name, struct fit_point_list * fitpoints);


#endif
