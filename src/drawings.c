#include <stdio.h>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc_c.h>
#include "drawings.h"
#include "params.h"
#include "log.h"

FILE * drawings_handle = NULL;
int draw_count_lines = 0;
struct drawing * draw_lines = NULL;

void drawings_close() {
  while (draw_count_lines--) {
    free(draw_lines[draw_count_lines].points);
    if (!draw_count_lines)
      free(draw_lines);
  }
}

void drawings_read() {
  if (par_drawings_file) {
    drawings_handle = fopen(par_drawings_file, "r");
    if (drawings_handle) {
      int i = 0;
      lprintf("Reading drawings \"%s\"\n", par_drawings_file);

      fscanf(drawings_handle, "%d\n", &draw_count_lines);
      draw_lines = malloc(draw_count_lines * sizeof(struct drawing));
      while ((i<draw_count_lines) && (!feof(drawings_handle))) {
        int j = 0;
        fscanf(drawings_handle, "%d\n", &draw_lines[i].count_points);
	draw_lines[i].points = malloc(draw_lines[i].count_points * sizeof(CvPoint2D64f));
        while ((j<draw_lines[i].count_points) && (!feof(drawings_handle))) {
          double longdeg;
          double latdeg;
          fscanf(drawings_handle, "%lf %lf", &longdeg, &latdeg);
	  draw_lines[i].points[j].x = longdeg * 60;
	  draw_lines[i].points[j].y = latdeg * 60;
	  dlprintf("Input drawings point: long=%f lat=%f\n", draw_lines[i].points[j].x, draw_lines[i].points[j].y);
	  j++;
	}
	i++;
      }
      atexit(&drawings_close);
      fclose(drawings_handle);
      lprintf("Loaded %d drawings\n", draw_count_lines);
    }
  }
}
