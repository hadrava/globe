#include "params.h"
#include "help.h"
#include "log.h"
#include "catalogue.h"
#include "render.h"
#include "drawings.h"

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
  render();
  return 0;
}
