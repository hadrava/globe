#include <opencv2/imgproc/imgproc_c.h>
#include <pthread.h>
#include "image.h"
#include "log.h"
#include "transform.h"
#include "render.h"
#include "render_image.h"

static long long int render_image_threads = 0;//TODO void hack
static pthread_t *render_tids;
static pthread_barrier_t render_barrier_run;
static pthread_barrier_t render_barrier_finish;
static volatile unsigned char render_thread_running;

void *render_thread_main(void *arg) {
  while (render_thread_running) {
    pthread_barrier_wait(&render_barrier_run);
    render_images_threaded((long long int) arg);//TODO void hack
    pthread_barrier_wait(&render_barrier_finish);
  }
  pthread_exit(NULL);
}

void render_images_threaded(int line) {
  struct image_list * list = image_list_head;
  while (list) {
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
    list = list->next;
  }
}

void render_image_init(int threads) {
  render_image_threads = 0;
  render_tids = malloc((threads-1)*sizeof(pthread_t));
  pthread_barrier_init(&render_barrier_run, NULL, threads);
  pthread_barrier_init(&render_barrier_finish, NULL, threads);
  render_thread_running = 1;
  while (--threads) {
    pthread_attr_t *thAttr = NULL;
    pthread_create(render_tids+render_image_threads, thAttr, render_thread_main, (void *) render_image_threads);//TODO void hack
    render_image_threads++;
  }

  atexit(&render_image_close);
}

void render_image_close() {
  pthread_barrier_wait(&render_barrier_run);
  render_thread_running = 0;
  pthread_barrier_wait(&render_barrier_finish);
  while (render_image_threads--) {
    void *loop_return;
    pthread_join(render_tids[render_image_threads], &loop_return);
  }
}

void render_images() {
  cvZero(sph_image);
  pthread_barrier_wait(&render_barrier_run);
  render_images_threaded(render_image_threads);
  pthread_barrier_wait(&render_barrier_finish);
}
