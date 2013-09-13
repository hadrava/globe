#include <stdio.h>
#include <stdlib.h>
#include <opencv2/highgui/highgui.hpp>
#include "image.h"
#include "log.h"

struct image_list * image_list_head = NULL;
struct image_params * image_active_params = NULL; //hack

static FILE *param_file;

struct image_params *image_params_cpy(struct image_params *dest, const struct image_params *src) {
  return memcpy(dest, src, sizeof(struct image_params));
}

void image_load(const char* name) {
  struct image_list *img = malloc(sizeof(struct image_list));
  img->image = cvLoadImage(name, CV_LOAD_IMAGE_COLOR);
  lprintf("Loaded image \"%s\" ", name);

  int len = strlen(name);
  char * filename = malloc((len+1)*sizeof(char));
  strcpy(filename, name);
  filename[--len] = 'r';
  filename[--len] = 'a';
  filename[--len] = 'p';
  param_file = fopen(filename, "r");
  if (param_file) {
    fscanf(param_file, "%lf", &img->params.latmin);
    fscanf(param_file, "%lf", &img->params.longmin);
    fscanf(param_file, "%lf", &img->params.distance);
    fscanf(param_file, "%lf", &img->params.directionlatmin);
    fscanf(param_file, "%lf", &img->params.directionlongmin);
    fscanf(param_file, "%lf", &img->params.focallength);
    fscanf(param_file, "%lf", &img->params.zrotationmin);
    fscanf(param_file, "%lf", &img->params.xshiftpix);
    fscanf(param_file, "%lf", &img->params.yshiftpix);
    lprintf("with parameters from file \"%s\".\n", filename);
  }
  else {
    img->params.latmin           = 0;
    img->params.longmin          = 0;
    img->params.distance         = 8;
    img->params.directionlatmin  = 0;
    img->params.directionlongmin = 0;
    img->params.focallength      = 2000;
    img->params.zrotationmin     = 0;
    img->params.xshiftpix        = 0;
    img->params.yshiftpix        = 0;
    lprintf("with default parameters.\n");
  }
  fclose(param_file);
  img->params.paramfilename = filename;

  img->next = image_list_head;
  image_list_head = img;
  image_active_params = &img->params; // TODO: small hack

  atexit(&image_close_all);
}

void image_params_print(const struct image_params *params) {
  vlprintf("image parameters:\n");
  vlprintf("latmin           = %lf\n",params->latmin          );
  vlprintf("longmin          = %lf\n",params->longmin         );
  vlprintf("distance         = %lf\n",params->distance        );
  vlprintf("directionlatmin  = %lf\n",params->directionlatmin );
  vlprintf("directionlongmin = %lf\n",params->directionlongmin);
  vlprintf("focallength      = %lf\n",params->focallength     );
  vlprintf("zrotationmin     = %lf\n",params->zrotationmin    );
  vlprintf("xshiftpix        = %lf\n",params->xshiftpix       );
  vlprintf("yshiftpix        = %lf\n",params->yshiftpix       );
}

void image_close_all() {
  while (image_list_head) {
    struct image_list *img = image_list_head;
    image_list_head = image_list_head->next;
    free(img->params.paramfilename);
    free(img);
  }
}
