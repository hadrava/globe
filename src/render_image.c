#include <opencv2/imgproc/imgproc_c.h>
#include "image.h"
#include "log.h"
#include "transform.h"
#include "render.h"
#include "render_image.h"

static int render_image_threads = 0;

void render_image_init(int threads) {
  render_image_threads = threads;
  atexit(&render_image_close);
}

void render_image_close() {
}

void render_images() {
  struct image_list * list = image_list_head;
  cvZero(sph_image);
  while (list) {
    image_params_print(&list->params);
    struct projection_params proj_params;
    sph_to_image_precalculate_projection(&list->params, list->image->width, list->image->height, &proj_params);
    for (int j=0; j<sph_image->height; j++) {
      for (int i=0; i<sph_image->width; i++) {
        CvPoint position = sph_to_image(sph_image_to_sph(cvPoint(i,j)), &list->params, &proj_params);
	int dst = i*3 + j*sph_image->widthStep;
	int src = position.x*3 + position.y*list->image->widthStep;
	if (position.x >= 0 && position.y >= 0 && position.x < list->image->width && position.y < list->image->height) {
          sph_image->imageData[dst    ] = list->image->imageData[src    ];
          sph_image->imageData[dst + 1] = list->image->imageData[src + 1];
          sph_image->imageData[dst + 2] = list->image->imageData[src + 2];
	}
      }
    }
    list = list->next;
  }
}
