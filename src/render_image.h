#ifndef RENDER_IMAGE_H_
#define RENDER_IMAGE_H_

#include "fit.h"
#include "transform.h"
#include <opencv2/imgproc/imgproc_c.h>

void *render_thread_main(void *arg);
void render_images_threaded(int line, int verbose);
void render_images_window();
void render_fit_distances(IplImage *window_image, const IplImage *image, struct fit_point_list *points, const struct image_params *im_params, struct projection_params *proj_params);
void render_image_init(int threads);
void render_image_close();
void render_images();

#endif
