#include "params.h"
#include "help.h"
#include "log.h"
#include "catalogue.h"
#include "render.h"
#include "drawings.h"
#include "image.h"

int main(int argc, char *argv[]) {
  parse_params(argc, argv);
  log_init();

  if (par_verbose)
    print_params();

  if (par_help) {
    print_help();
    return 0;
  }

  catalogue_read();
  drawings_read();

  render_init();
  int key = -1;
  lprintf("Press Esc to exit.\n");
  while (key != 27) {
    key = render(20);
    switch (key) {
      case 'a':
        image_active_params->latmin +=600;
	break;;
      case 'z':
        image_active_params->latmin -=600;
	break;;
      case 's':
        image_active_params->longmin +=600;
	break;;
      case 'x':
        image_active_params->longmin -=600;
	break;;
      case 'd':
        image_active_params->distance +=0.1;
	break;;
      case 'c':
        image_active_params->distance -=0.1;
	break;;
      case 'f':
        image_active_params->directionlatmin +=0.1;
	break;;
      case 'v':
        image_active_params->directionlatmin -=0.1;
	break;;
      case 'g':
        image_active_params->directionlongmin +=0.1;
	break;;
      case 'b':
        image_active_params->directionlongmin -=0.1;
	break;;
      case 'h':
        image_active_params->focallength += 100;
	break;;
      case 'n':
        image_active_params->focallength -= 100;
	break;;
      case 'j':
        image_active_params->zrotationmin += 0.3;
	break;;
      case 'm':
        image_active_params->zrotationmin -= 0.3;
	break;;
      case 'k':
        image_active_params->xshiftpix += 100;
	break;;
      case ',':
        image_active_params->xshiftpix -= 100;
	break;;
      case 'l':
        image_active_params->yshiftpix += 100;
	break;;
      case '.':
        image_active_params->yshiftpix -= 100;
	break;;
      default:
        ;;
    }
  }
  return 0;
}
