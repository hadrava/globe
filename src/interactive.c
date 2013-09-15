#include "log.h"
#include "render.h"
#include "image.h"
#include "fit.h"
#include "interactive.h"

void interactive_loop() {
  if (!fit_active)
    fit_active = image_list_head;
  if (!fit_active) {
    wlprintf("No input image (please specify with --img=FILE). Leaving interactive mode!\n");
    return;
  }

  int key = -1;
  lprintf("Press Esc to exit.\n");
  while (key != 27) {
    key = display(20);
    switch (key) {
      case 'a':
        fit_active->params.latmin +=600;
	render();
	break;;
      case 'z':
        fit_active->params.latmin -=600;
	render();
	break;;
      case 's':
        fit_active->params.longmin +=600;
	render();
	break;;
      case 'x':
        fit_active->params.longmin -=600;
	render();
	break;;
      case 'd':
        fit_active->params.distance +=0.1;
	render();
	break;;
      case 'c':
        fit_active->params.distance -=0.1;
	render();
	break;;
      case 'f':
        fit_active->params.directionlatmin +=0.3;
	render();
	break;;
      case 'v':
        fit_active->params.directionlatmin -=0.3;
	render();
	break;;
      case 'g':
        fit_active->params.directionlongmin +=0.3;
	render();
	break;;
      case 'b':
        fit_active->params.directionlongmin -=0.3;
	render();
	break;;
      case 'h':
        fit_active->params.focallength += 100;
	render();
	break;;
      case 'n':
        fit_active->params.focallength -= 100;
	render();
	break;;
      case 'j':
        fit_active->params.zrotationmin += 1000;
	render();
	break;;
      case 'm':
        fit_active->params.zrotationmin -= 1000;
	render();
	break;;
      case 'k':
        fit_active->params.xshiftpix += 100;
	render();
	break;;
      case ',':
        fit_active->params.xshiftpix -= 100;
	render();
	break;;
      case 'l':
        fit_active->params.yshiftpix += 100;
	render();
	break;;
      case '.':
        fit_active->params.yshiftpix -= 100;
	render();
	break;;
      default:
        ;;
    }
  }
}
