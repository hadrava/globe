#include <stdio.h>
#include <stdlib.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "image.h"
#include "log.h"

static struct image_list * image_list_head = NULL;

struct image_params *image_params_cpy(struct image_params *dest, const struct image_params *src) {
  return memcpy(dest, src, sizeof(struct image_params));
}

void image_load(char* name) {
  struct image_params params; //TODO
  struct image_list *img = malloc(sizeof(struct image_list));
  img->image = cvLoadImage(name, CV_LOAD_IMAGE_COLOR);
  lprintf("Loaded image \"%s\" ", name);
  image_params_cpy(&img->params, &params);
  lprintf("with default parameters\n", name);
  img->next = image_list_head;
  image_list_head = img;
}

void image_close_all() {
  while (image_list_head) {
    struct image_list *img = image_list_head;
    image_list_head = image_list_head->next;
    free(img);
  }
}

