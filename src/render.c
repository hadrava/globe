#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "catalogue.h"
#include "drawings.h"
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
  render_drawings();
  render_catalogue();
  if (par_sph_win) {
    cvShowImage("Spherical image", sph_image);
    cvWaitKey(0);
  }
}

void render_catalogue() {
   for (int i = 0; i < cat_size; i++) {
     CvPoint image = sph_to_sph_image(cvPoint2D64f(cat_stars[i].longmin, cat_stars[i].latmin));
     cvCircle(sph_image, image, par_star_size*(8-cat_stars[i].mag), CV_RGB(255,0,0), 3, CV_AA, 0);
     dlprintf("draw star: x=%d y=%d radius=%d\n", image.x, image.y, par_star_size*(8-cat_stars[i].mag));
  }
}

void render_drawings() {
   for (int i = 0; i < draw_count_lines; i++) {
     CvPoint image1 = sph_to_sph_image(draw_lines[i].points[0]);
     for (int j = 1; j < draw_lines[i].count_points; j++) {
       CvPoint image2 = sph_to_sph_image(draw_lines[i].points[j]);
       cvLine(sph_image, image1, image2, CV_RGB(0,0,255), par_drawings_size*2, CV_AA, 0);
       dlprintf("draw line: x1=%d y1=%d x2=%d y2=%d\n", image1.x, image1.y, image2.x, image2.y);
       image1 = image2;
     }
  }
}
