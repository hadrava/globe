#ifndef IMAGE_H_
#define IMAGE_H_

#include <opencv2/imgproc/imgproc_c.h>

struct image_params {
  double latmin;
  double longmin;
  double distance; // globe radius is 1
  double directionlatmin;
  double directionlongmin;
  double focallength;
  double zrotationmin;
  double xshiftpix;
  double yshiftpix;
};

struct image_list {
  IplImage *image;
  struct image_params params;
  struct image_list *next;
};

struct image_params * image_active_params;

void image_load(const char* name);
struct image_params *image_params_cpy(struct image_params *dest, const struct image_params *src);
void image_close_all();

void render_images();

#endif
