#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "catalogue.h"
#include "params.h"
#include "log.h"
#include "render.h"
#include "transform.h"

IplImage * sph_image;

void render_close() {
  if (par_sph_file)
    cvSaveImage(par_sph_file, sph_image, 0);
  if (par_sph_win)
    cvDestroyWindow("Spherical image");
  cvReleaseImage(&sph_image);
}

void render_init() {
  sph_image = cvCreateImage(cvSize(par_sph_width, par_sph_height), 8, 3);
  cvZero(sph_image);

  if (par_sph_win) {
    cvNamedWindow("Spherical image", 1);
  }

  atexit(&render_close);
}

void render() {
   for (int i = 0; i < cat_size; i++) {
     CvPoint spherical = sph_to_sph_image(cvPoint2D64f(cat_stars[i].longmin, cat_stars[i].latmin));
     cvCircle(sph_image, spherical, 8-cat_stars[i].mag, CV_RGB(255,0,0), 3, CV_AA, 0);
     dlprintf("draw star: x=%d y=%d radius=%d\n", spherical.x, spherical.y, 8-cat_stars[i].mag);

  }
}
