#ifndef RENDER_H_
#define RENDER_H_

#include <opencv2/imgproc/imgproc_c.h>

IplImage * sph_image;

void render_init();
void render_close();
int render(int delay);
void render_catalogue();
void render_drawings();
void render_coordinates();

#endif
