#include <stdio.h>
#include <stdlib.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "image.h"
#include "log.h"
#include "transform.h"
#include "render.h"

static struct image_list * image_list_head = NULL;
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
  char * filename = malloc(len*sizeof(char));
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

  img->next = image_list_head;
  image_list_head = img;
  image_active_params = &img->params; // TODO: small hack
}

void image_params_print(const struct image_params *params) {
  lprintf("image parameters:\n");
  lprintf("latmin           = %lf\n",params->latmin          );
  lprintf("longmin          = %lf\n",params->longmin         );
  lprintf("distance         = %lf\n",params->distance        );
  lprintf("directionlatmin  = %lf\n",params->directionlatmin );
  lprintf("directionlongmin = %lf\n",params->directionlongmin);
  lprintf("focallength      = %lf\n",params->focallength     );
  lprintf("zrotationmin     = %lf\n",params->zrotationmin    );
  lprintf("xshiftpix        = %lf\n",params->xshiftpix       );
  lprintf("yshiftpix        = %lf\n",params->yshiftpix       );
}

void image_close_all() {
  while (image_list_head) {
    struct image_list *img = image_list_head;
    image_list_head = image_list_head->next;
    free(img);
  }
}

void render_images() {
  struct image_list * list = image_list_head;
  while (list) {
    image_params_print(&list->params);
    for (int j=0; j<sph_image->height; j++) {
      for (int i=0; i<sph_image->width; i++) {
        CvPoint position = sph_to_image(sph_image_to_sph(cvPoint(i,j)), &list->params, list->image->width, list->image->height);
	int dst = i*3 + j*sph_image->widthStep;
	int src = position.x*3 + position.y*list->image->widthStep;
	if (position.x < 0 || position.y < 0 || position.x > list->image->width || position.y > list->image->height) {
          sph_image->imageData[dst    ] = 0;
          sph_image->imageData[dst + 1] = 0;
          sph_image->imageData[dst + 2] = 0;
	}
	else {
          sph_image->imageData[dst    ] = list->image->imageData[src    ];
          sph_image->imageData[dst + 1] = list->image->imageData[src + 1];
          sph_image->imageData[dst + 2] = list->image->imageData[src + 2];
	}
      }
    }
    list = list->next;
  }
}
