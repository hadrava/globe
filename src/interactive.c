#include "log.h"
#include "render.h"
#include "image.h"
#include "interactive.h"

void interactive_loop() {
  int key = -1;
  lprintf("Press Esc to exit.\n");
  while (key != 27) {
    key = display(20);
    switch (key) {
      case 'a':
        image_active_params->latmin +=600;
	render();
	break;;
      case 'z':
        image_active_params->latmin -=600;
	render();
	break;;
      case 's':
        image_active_params->longmin +=600;
	render();
	break;;
      case 'x':
        image_active_params->longmin -=600;
	render();
	break;;
      case 'd':
        image_active_params->distance +=0.1;
	render();
	break;;
      case 'c':
        image_active_params->distance -=0.1;
	render();
	break;;
      case 'f':
        image_active_params->directionlatmin +=0.3;
	render();
	break;;
      case 'v':
        image_active_params->directionlatmin -=0.3;
	render();
	break;;
      case 'g':
        image_active_params->directionlongmin +=0.3;
	render();
	break;;
      case 'b':
        image_active_params->directionlongmin -=0.3;
	render();
	break;;
      case 'h':
        image_active_params->focallength += 100;
	render();
	break;;
      case 'n':
        image_active_params->focallength -= 100;
	render();
	break;;
      case 'j':
        image_active_params->zrotationmin += 1000;
	render();
	break;;
      case 'm':
        image_active_params->zrotationmin -= 1000;
	render();
	break;;
      case 'k':
        image_active_params->xshiftpix += 100;
	render();
	break;;
      case ',':
        image_active_params->xshiftpix -= 100;
	render();
	break;;
      case 'l':
        image_active_params->yshiftpix += 100;
	render();
	break;;
      case '.':
        image_active_params->yshiftpix -= 100;
	render();
	break;;
      default:
        ;;
    }
  }
}
