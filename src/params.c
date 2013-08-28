#include <stdio.h>
#include <string.h>
#include "params.h"
#include "log.h"

#define PARSE_PAR(PARNAME, PARTYPE, PARVARIABLE) if ((sizeof(PARNAME)==value-argv[i]) && (strncmp(argv[i],PARNAME,sizeof(PARNAME)-1) == 0)) { sscanf(value,PARTYPE,&PARVARIABLE); continue; }
#define PARSE_STRING_PAR(PARNAME, PARVARIABLE) if ((sizeof(PARNAME)==value-argv[i]) && (strncmp(argv[i],PARNAME,sizeof(PARNAME)-1) == 0)) { PARVARIABLE=value; continue; }
#define DETECT_OPT(PARNAME, PARCOMMAND) if (strcmp(argv[i],PARNAME) == 0) { PARCOMMAND; continue; }

#define DEF_PAR_SPH_WIDTH 21600
#define DEF_PAR_SPH_HEIGHT 10800
#define DEF_PAR_STAR_SIZE 1.0
#define DEF_PAR_DRAWINGS_SIZE 1.0
#define DEF_PAR_COOR_STEP 10

int par_verbose = 0;
int par_help = 0;
char * par_log_file = NULL;
char * par_catalogue_file = NULL;
int par_sph_width = DEF_PAR_SPH_WIDTH;
int par_sph_height = DEF_PAR_SPH_HEIGHT;
char * par_sph_file = NULL;
int par_sph_win = 0;
float par_star_size = DEF_PAR_STAR_SIZE;
char * par_drawings_file = NULL;
float par_drawings_size = DEF_PAR_DRAWINGS_SIZE;
int par_coor_draw = 0;
int par_coor_step = DEF_PAR_COOR_STEP;

void parse_params(int argc, char *argv[]) {
  for(int i=1; i<argc; i++) {
    char* value = strchr(argv[i], '=');
    if (value) {
      value++;
      PARSE_PAR("--verbose", "%d", par_verbose);
      PARSE_STRING_PAR("--log", par_log_file);
      PARSE_STRING_PAR("--catalogue", par_catalogue_file);
      PARSE_PAR("--spherical-width", "%d", par_sph_width);
      PARSE_PAR("--spherical-height", "%d", par_sph_height);
      PARSE_STRING_PAR("--spherical-file", par_sph_file);
      PARSE_PAR("--spherical-window", "%d", par_sph_win);
      PARSE_PAR("--star-size", "%f", par_star_size);
      PARSE_STRING_PAR("--drawings", par_drawings_file);
      PARSE_PAR("--drawings-size", "%f", par_drawings_size);
      PARSE_PAR("--coordinates-draw", "%d", par_coor_draw);
      PARSE_PAR("--coordinates-step", "%d", par_coor_step);
    }
    else {
      DETECT_OPT("--verbose", par_verbose++);
      DETECT_OPT("--help", par_help = 1);
      DETECT_OPT("--spherical-window", par_sph_win = 1);
      DETECT_OPT("--coordinates-draw", par_coor_draw = 1);
    }
    fprintf(stderr, "Unknown parameter: \"%s\"\n", argv[i]); //TODO: how to log this? Log is not prepared yet...
  }
}

void print_params() {
  lprintf("--verbose=%d\n", par_verbose);
  lprintf("--help=%d\n", par_help);
  lprintf("--log=%s\n", par_log_file);
  lprintf("--catalogue=%s\n", par_catalogue_file);
  lprintf("--spherical-width=%d\n", par_sph_width);
  lprintf("--spherical-height=%d\n", par_sph_height);
  lprintf("--spherical-file=%s\n", par_sph_file);
  lprintf("--spherical-window=%d\n", par_sph_win);
  lprintf("--star-size=%f\n", par_star_size);
  lprintf("--drawings=%s\n", par_drawings_file);
  lprintf("--drawings-size=%f\n", par_drawings_size);
  lprintf("--coordinates-draw=%d\n", par_coor_draw);
  lprintf("--coordinates-step=%d\n", par_coor_step);
}

void print_available_params(){
  lprintf("Usage: globe [OPTION]\n");
  lprintf("Available parameters:\n");
  lprintf("    --verbose[=LEVEL]         set or increase verbosity level\n");
  lprintf("    --help                    display this help message and exit\n");
  lprintf("    --log=FILE                log everything to file\n");
  lprintf("    --catalogue=FILE          read star catalogue\n");
  lprintf("    --spherical-width=PIX     set width of spherical image (defult: %d)\n", DEF_PAR_SPH_WIDTH);
  lprintf("    --spherical-height=PIX    set height of spherical image (default: %d)\n", DEF_PAR_SPH_HEIGHT);
  lprintf("    --spherical-file=FILE     save spherical image to file\n");
  lprintf("    --spherical-window[=1]    show sperical image in window\n");
  lprintf("    --star-size=COEFF         set star size (default: %f)\n", DEF_PAR_STAR_SIZE);
  lprintf("    --drawings=FILE           read drawings from file\n");
  lprintf("    --drawings-size=COEFF     set drawings size (default: %f)\n", DEF_PAR_DRAWINGS_SIZE);
  lprintf("    --coordinates-draw[=1]    draw spherical coordinates\n");
  lprintf("    --coordinates-step=DEG    set coordinate spacing in degrees (default: %d)\n", DEF_PAR_COOR_STEP);
}

