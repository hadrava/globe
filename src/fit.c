#include <stdlib.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <nlopt.h>
#include <strings.h>
#include <string.h>
#include "image.h"
#include "transform.h"
#include "log.h"
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

struct fit_point_list *fit_remove_point(struct fit_point_list *point, struct fit_point_list *list) {
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

void fit_do(struct image_list *image, struct fit_point_list *points) {
  nlopt_algorithm algorithm = NLOPT_LN_NELDERMEAD;
  nlopt_opt object = nlopt_create(algorithm, 7);//assume photo is complete
  lprintf("Optimizing using \"%s\" algorithm.\n", nlopt_algorithm_name(algorithm));

  struct objective_data data = {image, points};
  nlopt_set_min_objective(object, fit_objective, &data);

  //nlopt_result nlopt_set_lower_bounds(nlopt_opt opt, const double* lb);//TODO +-HUGE_VAL
  //nlopt_result nlopt_set_upper_bounds(nlopt_opt opt, const double* ub);
  nlopt_set_stopval(object, 0);
  nlopt_set_maxtime(object, 15);//in sec

  double error;
  nlopt_optimize(object, (double *)&image->params, &error);


  nlopt_destroy(object);
}

double fit_objective(unsigned n, const double *x, double *grad, void *f_data) {
  if (grad) {
    elprintf("Optimization library wants gradient, but we are not computing it!\n");
    elprintf("Fitting will probably fail!\n");
    bzero(grad, n*sizeof(double));
  }
  struct image_params params;
  memcpy(&params, x, n*sizeof(double));
  if (n==7) {
    params.xshiftpix = ((struct objective_data *)f_data)->image->params.xshiftpix;
    params.yshiftpix = ((struct objective_data *)f_data)->image->params.yshiftpix;
  }
  double error = fit_error(((struct objective_data *)f_data)->image, ((struct objective_data *)f_data)->points, &params);
  dlprintf("Current error is %lf\n", error);
  return error;
}

double fit_error(struct image_list *image, struct fit_point_list *points, struct image_params *im_params) {
  double error = 0;
  struct projection_params proj_params;
  sph_to_image_precalculate_projection(im_params, image->image->width, image->image->height, &proj_params);
  while (points) {
    CvPoint star = sph_to_image(sph_to_sph64(points->globe_position_min), im_params, &proj_params);
    error += (points->in_image.x-star.x)*(points->in_image.x-star.x);
    error += (points->in_image.y-star.y)*(points->in_image.y-star.y);

    points = points->next;
  }
  return error;
}
