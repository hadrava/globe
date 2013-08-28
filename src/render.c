#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "catalogue.h"
#include "drawings.h"
#include "params.h"
#include "log.h"
#include "render.h"
#include "transform.h"

IplImage * sph_image;

CvFont bg_font;
CvFont fg_font;

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

  cvInitFont(&bg_font, CV_FONT_HERSHEY_SIMPLEX, 0.3, 0.4, 0.0, 3, 8);
  cvInitFont(&fg_font, CV_FONT_HERSHEY_SIMPLEX, 0.3, 0.4, 0.0, 1, 8);

  if (par_sph_win) {
    cvNamedWindow("Spherical image", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
  }

  atexit(&render_close);
}

void render() {
  if (par_coor_draw)
    render_coordinates();
  render_drawings();
  render_catalogue();
  if (par_sph_win) {
    cvShowImage("Spherical image", sph_image);
    cvWaitKey(0);
  }
}

void render_catalogue() {
  int cons_center_x, cons_center_y, cons_stars;
  for (int i = 0; i < cat_size; i++) {
    CvPoint image_pt = sph_to_sph_image(cvPoint2D64f(cat_stars[i].longmin, cat_stars[i].latmin));
    cvCircle(sph_image, image_pt, par_star_size*(8-cat_stars[i].mag), CV_RGB(255,0,0), 3, CV_AA, 0);
    dlprintf("draw star: x=%d y=%d radius=%d\n", image_pt.x, image_pt.y, par_star_size*(8-cat_stars[i].mag));
    if (cat_stars[i].numinc == 1)
      cons_center_x = cons_center_y = cons_stars = 0;
    cons_center_x += image_pt.x;
    cons_center_y += image_pt.y;
    cons_stars++;
    if ((i == cat_size-1) || (cat_stars[i+1].numinc == 1)) {
      cvPutText(sph_image, cat_stars[i].cons, cvPoint(cons_center_x/cons_stars,cons_center_y/cons_stars), &bg_font, CV_RGB(0, 0, 0));
      cvPutText(sph_image, cat_stars[i].cons, cvPoint(cons_center_x/cons_stars,cons_center_y/cons_stars), &fg_font, CV_RGB(255,255,0));
    }
  }
}

void render_drawings() {
  for (int i = 0; i < draw_count_lines; i++) {
    CvPoint image_pt1 = sph_to_sph_image(draw_lines[i].points[0]);
    for (int j = 1; j < draw_lines[i].count_points; j++) {
      CvPoint image_pt2 = sph_to_sph_image(draw_lines[i].points[j]);
      cvLine(sph_image, image_pt1, image_pt2, CV_RGB(0,0,255), par_drawings_size*2, CV_AA, 0);
      dlprintf("draw line: x1=%d y1=%d x2=%d y2=%d\n", image_pt1.x, image_pt1.y, image_pt2.x, image_pt2.y);
      image_pt1 = image_pt2;
    }
  }
}

void render_coordinates() {
  int step = 60 * par_coor_step;
  CvPoint image_pt1, image_pt2, text_pt;
  char label[10];
  for (int i = 0; i < 21600; i+=step) {
    image_pt1 = sph_to_sph_image(cvPoint2D64f(i, -5400));
    image_pt2 = sph_to_sph_image(cvPoint2D64f(i, +5400));
    text_pt   = sph_to_sph_image(cvPoint2D64f(i + step/10, step/10));
    sprintf(label, "%i", i/60);

    cvLine(sph_image, image_pt1, image_pt2, CV_RGB(127,127,127), par_drawings_size*2, CV_AA, 0);
    cvPutText(sph_image, label, text_pt, &bg_font, CV_RGB(0, 0, 0));
    cvPutText(sph_image, label, text_pt, &fg_font, CV_RGB(127, 127, 127));
  }
  for (int i = 0; i < 5400; i+=step) {
    image_pt1 = sph_to_sph_image(cvPoint2D64f(0, i));
    image_pt2 = sph_to_sph_image(cvPoint2D64f(21600, i));
    text_pt   = sph_to_sph_image(cvPoint2D64f(step/10, i + step/10));
    sprintf(label, "+%i", i/60);
    if (i == 0)
      sprintf(label, "0");

    cvLine(sph_image, image_pt1, image_pt2, CV_RGB(127,127,127), par_drawings_size*2, CV_AA, 0);
    cvPutText(sph_image, label, text_pt, &bg_font, CV_RGB(0, 0, 0));
    cvPutText(sph_image, label, text_pt, &fg_font, CV_RGB(127, 127, 127));


    image_pt1 = sph_to_sph_image(cvPoint2D64f(0, -i));
    image_pt2 = sph_to_sph_image(cvPoint2D64f(21600, -i));
    text_pt   = sph_to_sph_image(cvPoint2D64f(step/10, -i + step/10));
    sprintf(label, "-%i", i/60);
    if (i == 0)
      sprintf(label, "0");

    cvLine(sph_image, image_pt1, image_pt2, CV_RGB(127,127,127), par_drawings_size*2, CV_AA, 0);
    cvPutText(sph_image, label, text_pt, &bg_font, CV_RGB(0, 0, 0));
    cvPutText(sph_image, label, text_pt, &fg_font, CV_RGB(127, 127, 127));
  }
}
