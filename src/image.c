#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <opencv2/highgui/highgui_c.h>
#include "image.h"
#include "log.h"
#include "params.h"
#include "fit.h"

struct image_list * image_list_head = NULL;
struct image_list * image_list_tail = NULL;

struct image_params *image_params_cpy(struct image_params *dest, const struct image_params *src) {
  return memcpy(dest, src, sizeof(struct image_params));
}

void image_load(char *name) {
  struct image_list *img = malloc(sizeof(struct image_list));
  img->image = cvLoadImage(name, CV_LOAD_IMAGE_COLOR);
  lprintf("Loaded image \"%s\" ", name);

  int len = strlen(name); //TODO WARN image name can be shorter!!
  char *filename = malloc((len+1)*sizeof(char));
  char *fitfilename = malloc((len+1)*sizeof(char));
  strcpy(filename, name);
  strcpy(fitfilename, name);
  filename[len-3] = 'p';
  filename[len-2] = 'a';
  filename[len-1] = 'r';
  fitfilename[len-3] = 'f';
  fitfilename[len-2] = 'i';
  fitfilename[len-1] = 't';
  FILE *param_file = fopen(filename, "r");
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
    fclose(param_file);
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
  img->params.paramfilename = filename;
  img->fitfilename = fitfilename;
  img->fitpoints = fit_load_points_from_file(img->fitfilename);
  fit_make_active(img);

  img->filename = name;
  img->display_window = par_image_win;
  if (img->display_window) {
    lprintf("creating window named \"%s\" for image param \"%s\"\n", img->filename, img->params.paramfilename);
    img->window_image = cvCreateImage(cvSize(par_image_width, par_image_height), 8, 3);
    cvZero(img->window_image);
    cvNamedWindow(img->filename, CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
    setlocale(LC_ALL, "C");//cvNamedWindow changed locale, so we set it back
    cvShowImage(img->filename, img->window_image);
  }

  img->next = image_list_head;
  img->prev = NULL;
  if (img->next)
    img->next->prev = img;
  else
    image_list_tail = img;
  image_list_head = img;

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
    if (img->display_window) {
      cvDestroyWindow(img->filename);
      cvReleaseImage(&img->window_image);
    }
    free(img->params.paramfilename);
    free(img);
  }
  image_list_tail = NULL;
}

void image_save_params(struct image_list *img) {
  lprintf("Saving image params of image \"%s\" to file \"%s\"\n", img->filename, img->params.paramfilename);
  FILE *param_file = fopen(img->params.paramfilename, "w");
  fprintf(param_file, "%lf\n", img->params.latmin);
  fprintf(param_file, "%lf\n", img->params.longmin);
  fprintf(param_file, "%lf\n", img->params.distance);
  fprintf(param_file, "%lf\n", img->params.directionlatmin);
  fprintf(param_file, "%lf\n", img->params.directionlongmin);
  fprintf(param_file, "%lf\n", img->params.focallength);
  fprintf(param_file, "%lf\n", img->params.zrotationmin);
  fprintf(param_file, "%lf\n", img->params.xshiftpix);
  fprintf(param_file, "%lf\n", img->params.yshiftpix);
  fclose(param_file);
}
