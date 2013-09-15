#ifndef INTERACTIVE_H_
#define INTERACTIVE_H_

#include <opencv2/highgui/highgui_c.h>
extern int selected_star;

void interactive_spherical_mouse(int event, int x, int y, int flags, void* param);
void interactive_loop();

#endif
