#include <stdlib.h>
#include <opencv2/imgproc/imgproc_c.h>
#include "image.h"
#include "fit.h"

struct image_list *fit_active = NULL;

struct fit_point_list *fit_points = NULL;

void fit_add_point(CvPoint globe_position_min, CvPoint in_image) {
  struct fit_point_list *new_point = malloc(sizeof(struct fit_point_list)); //TODO: free
  new_point->globe_position_min = globe_position_min;
  new_point->in_image = in_image;
  new_point->star = -1;
  new_point->next = fit_points;
  new_point->prev = NULL;
  if (new_point->next)
    new_point->next->prev = new_point;
  fit_points = new_point;
}

void fit_add_point_star(CvPoint globe_position_min, CvPoint in_image, int star) {
  struct fit_point_list *new_point = fit_find_star(star);
  if (!new_point) {
    new_point = malloc(sizeof(struct fit_point_list)); //TODO: free
    new_point->next = fit_points;
    new_point->prev = NULL;
    if (new_point->next)
      new_point->next->prev = new_point;
    fit_points = new_point;
  }
  new_point->globe_position_min = globe_position_min;
  new_point->in_image = in_image;
  new_point->star = star;
}

struct fit_point_list *fit_find_star(int star) {
  struct fit_point_list *point = fit_points;
  while (point) {
    if (point->star == star)
      return point;
    point = point->next;
  }
  return NULL;
}

struct fit_point_list * fit_remove_point(struct fit_point_list *point, struct fit_point_list *list) {
  if (!point)
    return list;
  if (list == point)
    list = list->next;
  if (point->next)
    point->next->prev = point->prev;
  if (point->prev)
    point->prev->next = point->next;
  free(point);
  return list;
}
