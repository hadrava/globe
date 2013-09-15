#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <pthread.h>
#include "image.h"
#include "log.h"
#include "transform.h"
#include "render.h"
#include "fit.h"
#include "params.h"
#include "render_image.h"

static int render_image_threads = 0;
static pthread_t *render_tids;
static int *render_thread_numbers;
static pthread_barrier_t render_barrier_run;
static pthread_barrier_t render_barrier_finish;
static volatile unsigned char render_thread_running;

void *render_thread_main(void *arg) {
  while (1) {
    pthread_barrier_wait(&render_barrier_run);
    if (!render_thread_running)
      break;
    render_images_threaded(*(int *)arg, 0);
    pthread_barrier_wait(&render_barrier_finish);
  }
  pthread_exit(NULL);
}

void render_images_threaded(int line, int verbose) {
  struct image_list * list = image_list_tail;
  while (list) {
    if (verbose)
      image_params_print(&list->params);
    struct projection_params proj_params;
    sph_to_image_precalculate_projection(&list->params, list->image->width, list->image->height, &proj_params);

    for (int j=line; j<sph_image->height; j+=render_image_threads+1) {
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
    list = list->prev;
  }
}

void render_images_window() {
  struct image_list * list = image_list_tail;
  while (list) {
    if (list->display_window) {
      cvResize(list->image, list->window_image, CV_INTER_CUBIC);
      struct projection_params proj_params;
      sph_to_image_precalculate_projection(&list->params, list->image->width, list->image->height, &proj_params);
      if (list == fit_active)
        render_fit_distances(list->window_image, list->image, fit_points);
      cvShowImage(list->filename, list->window_image);
    }
    list = list->prev;
  }
}

void render_fit_distances(IplImage *window_image, const IplImage *image, struct fit_point_list *points) {
  while (points) {
    cvCircle(window_image, image_to_image_window(points->globe_position_min, image, window_image), par_star_size, CV_RGB(255,0,0), 3, CV_AA, 0);
    cvCircle(window_image, image_to_image_window(points->in_image, image, window_image), par_star_size, CV_RGB(255,255,0), 3, CV_AA, 0);

    points = points->next;
  }
}

void render_image_init(int threads) {
  render_image_threads = 0;
  render_tids = malloc((threads-1)*sizeof(pthread_t));
  render_thread_numbers = malloc((threads-1)*sizeof(int));
  pthread_barrier_init(&render_barrier_run, NULL, threads);
  pthread_barrier_init(&render_barrier_finish, NULL, threads);
  render_thread_running = 1;
  while (--threads) {
    pthread_attr_t *thAttr = NULL;
    render_thread_numbers[render_image_threads] = render_image_threads;
    pthread_create(render_tids+render_image_threads, thAttr, render_thread_main, render_thread_numbers+render_image_threads);
    render_image_threads++;
  }

  atexit(&render_image_close);
}

void render_image_close() {
  render_thread_running = 0;
  pthread_barrier_wait(&render_barrier_run);
  while (render_image_threads--) {
    void *loop_return;
    pthread_join(render_tids[render_image_threads], &loop_return);
  }
}

void render_images() {
  cvZero(sph_image);
  pthread_barrier_wait(&render_barrier_run);
  render_images_threaded(render_image_threads, 1);
  pthread_barrier_wait(&render_barrier_finish);
  render_images_window();
}
