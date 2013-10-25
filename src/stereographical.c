#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <opencv2/highgui/highgui_c.h>
#include "stereographical.h"
#include "transform.h"
#include "log.h"
#include "render.h"
#include "params.h"
#include "image.h"

struct stereographical_list *stereographical_list_head = NULL;
struct stereographical_list *stereographical_list_tail = NULL;


void render_stereographical(struct stereographical_list *stereo) {
  struct image_list *list = image_list_tail;
  while (list) {
    struct projection_params proj_params;
    sph_to_image_precalculate_projection(&list->params, list->image->width, list->image->height, &proj_params);
    struct stereo_projection_params stereo_params;
    stereographical_to_sph_precalculate_projection(&stereo->params, stereo->image->width, stereo->image->height, &stereo_params);

    for (int j=0; j<stereo->image->height; j++) {
      for (int i=0; i<stereo->image->width; i++) {
        CvPoint position = sph_to_image(stereographical_to_sph(cvPoint(i,j), &stereo->params, &stereo_params), &list->params, &proj_params);

        int dst = i*3 + j*stereo->image->widthStep;
        int src = position.x*3 + position.y*list->image->widthStep;

        if (position.x >= 0 && position.y >= 0 && position.x < list->image->width && position.y < list->image->height) {
          stereo->image->imageData[dst    ] = list->image->imageData[src    ];
          stereo->image->imageData[dst + 1] = list->image->imageData[src + 1];
          stereo->image->imageData[dst + 2] = list->image->imageData[src + 2];
        }
      }
    }
    list = list->prev;
  }
}

void render_all_stereographical() {
  struct stereographical_list *stereo = stereographical_list_head;
  while (stereo) {
    render_stereographical(stereo);
    if (stereo->display_window)
      cvShowImage(stereo->filename, stereo->image);
    stereo = stereo->next;
  }
}

void stereographical_save(const struct stereographical_list *stereo) {
  cvSaveImage(stereo->filename, stereo->image, 0);
}

void stereographical_save_all() {
  const struct stereographical_list *stereo = stereographical_list_head;
  while (stereo) {
    stereographical_save(stereo);
    stereo = stereo->next;
  }
}

void stereographical_prepare(char *name) {
  struct stereographical_list *stereo = malloc(sizeof(struct stereographical_list));
  stereo->image = cvCreateImage(cvSize(par_stereo_width, par_stereo_height), 8, 3);
  cvZero(stereo->image);
  lprintf("Prepared output stereographical image \"%s\".\n", name);

  stereo->params.latmin            = par_stereo_latmin;
  stereo->params.longmin           = par_stereo_longmin;
  stereo->params.zrotationmin      = par_stereo_zrotationmin;
  stereo->params.image_size_factor = par_stereo_size_factor;

  stereographical_params_print(&stereo->params);//TODO

  stereo->filename                 = name;
  stereo->display_window           = par_image_win;

  if (stereo->display_window) {
    lprintf("Creating window named \"%s\" for stereographical image \"%s\"\n", stereo->filename, stereo->filename);
    cvNamedWindow(stereo->filename, CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
    setlocale(LC_ALL, "C");//cvNamedWindow changed locale, so we set it back
    cvShowImage(stereo->filename, stereo->image);
  }

  stereo->next = stereographical_list_head;
  stereo->prev = NULL;
  if (stereo->next)
    stereo->next->prev = stereo;
  else
    stereographical_list_tail = stereo;
  stereographical_list_head = stereo;

  atexit(&stereographical_close_all);
}

void stereographical_params_print(const struct stereographical_params *params) {
  lprintf("stereographical parameters:\n");
  lprintf("latmin            = %lf\n",params->latmin           );
  lprintf("longmin           = %lf\n",params->longmin          );
  lprintf("zrotationmin      = %lf\n",params->zrotationmin     );
  lprintf("image_size_factor = %lf\n",params->image_size_factor);
}

void stereographical_close_all() {
  stereographical_save_all();//TODO: HACK
  while (stereographical_list_head) {
    struct stereographical_list *stereo = stereographical_list_head;
    stereographical_list_head = stereographical_list_head->next;
    stereographical_params_print(&stereo->params);//TODO
    if (stereo->display_window) {
      cvDestroyWindow(stereo->filename);
    }
    cvReleaseImage(&stereo->image);
    free(stereo);
  }
  stereographical_list_tail = NULL;
}
