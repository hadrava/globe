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
  char * paramfilename;
};

struct image_list {
  IplImage *image;
  IplImage *window_image;
  struct image_params params;
  char *filename;
  int display_window;
  struct image_list *next;
  struct image_list *prev;
};

extern struct image_list *image_list_head;
extern struct image_list *image_list_tail;

struct image_params *image_params_cpy(struct image_params *dest, const struct image_params *src);
void image_load(char *name);
void image_params_print(const struct image_params *params);
void image_close_all();

#endif
