#ifndef STEREOGRAPHICAL_H_
#define STEREOGRAPHICAL_H_

#include <opencv2/imgproc/imgproc_c.h>

struct stereographical_params {
  double latmin;
  double longmin;
  double zrotationmin;
  double image_size_factor;
};

struct stereographical_list {
  IplImage *image;
  struct stereographical_params params;
  char *filename;
  int display_window;
  struct stereographical_list *next;
  struct stereographical_list *prev;
};

extern struct stereographical_list *stereographical_list_head;
extern struct stereographical_list *stereographical_list_tail;

void render_stereographical(struct stereographical_list *stereo);
void render_all_stereographical();
void stereographical_save(const struct stereographical_list *stereo);
void stereographical_save_all();
void stereographical_prepare(char *name);
void stereographical_params_print(const struct stereographical_params *params);
void stereographical_close_all();

#endif
