#ifndef DRAWINGS_H_
#define DRAWINGS_H_

#include <opencv2/imgproc/imgproc_c.h>

struct drawing {
  int count_points;
  CvPoint2D64f * points;
};

extern int draw_count_lines;
extern struct drawing * draw_lines;

void drawings_close();
void drawings_read();

#endif
