#include <opencv2/highgui/highgui_c.h>
#include "log.h"
#include "render.h"
#include "render_image.h"
#include "image.h"
#include "fit.h"
#include "transform.h"
#include "catalogue.h"
#include "interactive.h"
#include "stereographical.h"

int selected_star = -1;

void interactive_spherical_mouse(int event, int x, int y, int flags, void* param) {
  struct image_list *image = param;

  switch (event) {
    case CV_EVENT_LBUTTONDOWN:
      if (image)
        break;//TODO convert coordinates
      selected_star = find_nearest_star(sph_image_to_sph(cvPoint(x, y)));
      render_catalogue();
      break;;
    case CV_EVENT_RBUTTONDOWN:
      if (selected_star != -1) {
        CvPoint in_image;
        if (image)
	  in_image = image_to_image(cvPoint(x, y), image->window_image, image->image);
	else
	  in_image = sph_to_image_slow(sph_image_to_sph(cvPoint(x, y)), &fit_active->params, fit_active->image->width, fit_active->image->height);
        fit_add_point_star(cvPoint(cat_stars[selected_star].longmin, cat_stars[selected_star].latmin), in_image, selected_star);
	render_images_window();
      }
      break;;
    case CV_EVENT_RBUTTONDBLCLK:
      if (selected_star != -1) {
	fit_active->fitpoints = fit_remove_point(fit_find_star(selected_star), fit_active->fitpoints);
	render_images_window();
      }
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
  cvSetMouseCallback(fit_active->filename, &interactive_spherical_mouse, fit_active);

  int key = -1;
  lprintf("Press Esc to exit.\n");
  while (key != 27) { //<Escape>
    key = display(20);
    switch (key) {
      case -1: //none
      case 27: //<Escape>
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
      case 10: //<Return>
        fit_do(fit_active, fit_active->fitpoints);
	render();
        break;
      case 'q':
        stereographical_list_head->params.latmin -= 0.1;
	render();
	break;
      case 'w':
        stereographical_list_head->params.latmin += 0.1;
	render();
	break;
      case 'e':
        stereographical_list_head->params.longmin -= 0.1;
	render();
	break;
      case 'r':
        stereographical_list_head->params.longmin += 0.1;
	render();
	break;
      case 't':
        stereographical_list_head->params.zrotationmin -= 0.1;
	render();
	break;
      case 'y':
        stereographical_list_head->params.zrotationmin += 0.1;
	render();
	break;
      case 'u':
        stereographical_list_head->params.image_size_factor /= 1.2;
	render();
	break;
      case 'i':
        stereographical_list_head->params.image_size_factor *= 1.2;
	render();
	break;
      case 65470: //<F1>
        fit_save_points_to_file(fit_active->fitfilename, fit_active->fitpoints);
	break;
      case '+'|0x10000:
 //       zoom_in();
	break;
      case '-':
//        zoom_out();
	break;
      default:
        dlprintf("Unhandled keypress %i, (0x%X)\n", key, key);
    }
  }
}
