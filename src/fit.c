#include <stdlib.h>
#include <opencv2/imgproc/imgproc_c.h>
#include "image.h"
#include "fit.h"

struct image_list * fit_active = NULL;

struct fit_point_list *fit_points = NULL;

void fit_add_point(CvPoint globe_position_min, CvPoint in_image) {
  struct fit_point_list *new_point = malloc(sizeof(struct fit_point_list)); //TODO: free
  new_point->globe_position_min = globe_position_min;
  new_point->in_image = in_image;
  new_point->next = fit_points;
  fit_points = new_point;
}
