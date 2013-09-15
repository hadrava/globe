#include <opencv2/highgui/highgui_c.h>
#include "log.h"
#include "render.h"
#include "image.h"
#include "fit.h"
#include "transform.h"
#include "catalogue.h"
#include "interactive.h"

int selected_star = -1;

void interactive_spherical_mouse(int event, int x, int y, int flags, void* param) {
  switch (event) {
    case CV_EVENT_LBUTTONDOWN:
      if (selected_star == -1)
        selected_star = find_nearest_star(sph_image_to_sph(cvPoint(x, y)));
      else
        selected_star = -1;
      render_catalogue();
      break;;
    case CV_EVENT_RBUTTONDOWN:
      if (selected_star != -1)
        fit_add_point(cvPoint(cat_stars[selected_star].longmin, cat_stars[selected_star].latmin), sph_to_image_slow(sph_image_to_sph(cvPoint(x, y)), &fit_active->params, fit_active->image->width, fit_active->image->height));
      break;;
    case CV_EVENT_MOUSEMOVE:
    case CV_EVENT_MBUTTONDBLCLK:
    default:
      dlprintf("Unhandled mouse event No. %d\n", event);
  }
}

void interactive_loop() {
  if (!fit_active)
    fit_active = image_list_head;
  if (!fit_active) {
    wlprintf("No input image (please specify with --img=FILE). Leaving interactive mode!\n");
    return;
  }

  cvSetMouseCallback("Spherical image", &interactive_spherical_mouse, NULL);

  int key = -1;
  lprintf("Press Esc to exit.\n");
  while (key != 27) {
    key = display(20);
    switch (key) {
      case -1:
      case 27:
        break;
      case 'a':
        fit_active->params.latmin +=600;
	render();
	break;
      case 'z':
        fit_active->params.latmin -=600;
	render();
	break;
      case 's':
        fit_active->params.longmin +=600;
	render();
	break;
      case 'x':
        fit_active->params.longmin -=600;
	render();
	break;
      case 'd':
        fit_active->params.distance +=0.1;
	render();
	break;
      case 'c':
        fit_active->params.distance -=0.1;
	render();
	break;
      case 'f':
        fit_active->params.directionlatmin +=0.3;
	render();
	break;
      case 'v':
        fit_active->params.directionlatmin -=0.3;
	render();
	break;
      case 'g':
        fit_active->params.directionlongmin +=0.3;
	render();
	break;
      case 'b':
        fit_active->params.directionlongmin -=0.3;
	render();
	break;
      case 'h':
        fit_active->params.focallength += 100;
	render();
	break;
      case 'n':
        fit_active->params.focallength -= 100;
	render();
	break;
      case 'j':
        fit_active->params.zrotationmin += 1000;
	render();
	break;
      case 'm':
        fit_active->params.zrotationmin -= 1000;
	render();
	break;
      case 'k':
        fit_active->params.xshiftpix += 100;
	render();
	break;
      case ',':
        fit_active->params.xshiftpix -= 100;
	render();
	break;
      case 'l':
        fit_active->params.yshiftpix += 100;
	render();
	break;
      case '.':
        fit_active->params.yshiftpix -= 100;
	render();
	break;
      case 65471: //<F2>
        image_save_params(fit_active);
	break;
      default:
        dlprintf("Unhandled keypress %i\n", key);
    }
  }
}
