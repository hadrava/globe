#include <opencv2/imgproc/imgproc_c.h>
#include "image.h"
#include "fit.h"

struct image_list * fit_active = NULL;

struct fit_point_list *fit_points = NULL;

void fit_add_point(CvPoint globe_position_min, CvPoint in_image) {
}
