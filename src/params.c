#include <stdio.h>
#include <string.h>
#include "params.h"
#include "log.h"
#include "image.h"

#define PARSE_PAR(PARNAME, PARTYPE, PARVARIABLE) if ((sizeof(PARNAME)==value-argv[i]) && (strncmp(argv[i],PARNAME,sizeof(PARNAME)-1) == 0)) { sscanf(value,PARTYPE,&PARVARIABLE); continue; }
#define PARSE_STRING_PAR(PARNAME, PARVARIABLE) if ((sizeof(PARNAME)==value-argv[i]) && (strncmp(argv[i],PARNAME,sizeof(PARNAME)-1) == 0)) { PARVARIABLE=value; continue; }
#define DETECT_OPT(PARNAME, PARCOMMAND) if (strcmp(argv[i],PARNAME) == 0) { PARCOMMAND; continue; }

#define DEF_PAR_SPH_WIDTH 21600
#define DEF_PAR_SPH_HEIGHT 10800
#define DEF_PAR_STAR_SIZE 1.0
#define DEF_PAR_DRAWINGS_SIZE 1.0
#define DEF_PAR_COOR_STEP 10
#define DEF_PAR_THREADS 4
#define DEF_PAR_IMAGE_WIDTH 800
#define DEF_PAR_IMAGE_HEIGHT 600

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
int par_interactive = 0;
int par_threads = DEF_PAR_THREADS;
int par_image_win = 0;
int par_image_width = DEF_PAR_IMAGE_WIDTH;
int par_image_height = DEF_PAR_IMAGE_HEIGHT;


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
      PARSE_STRING_PAR("--img", image_load(value); value);//WARN: macro hack
      PARSE_PAR("--interactive", "%d", par_interactive);
      PARSE_PAR("--threads", "%d", par_threads);
      PARSE_PAR("--image-window", "%d", par_image_win);
      PARSE_PAR("--image-width", "%d", par_image_width);
      PARSE_PAR("--image-height", "%d", par_image_height);
    }
    else {
      DETECT_OPT("--verbose", par_verbose++);
      DETECT_OPT("--help", par_help = 1);
      DETECT_OPT("--spherical-window", par_sph_win = 1);
      DETECT_OPT("--coordinates-draw", par_coor_draw = 1);
      DETECT_OPT("--interactive", par_interactive = 1);
      DETECT_OPT("--image-window", par_image_win = 1);
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
  lprintf("--img=NOT_IMPLEMENTED\n");//TODO
  lprintf("--interactive=%d\n", par_interactive);
  lprintf("--threads=%d\n", par_threads);
  lprintf("--image-window=%d\n", par_image_win);
  lprintf("--image-width=%d\n", par_image_width);
  lprintf("--image-height=%d\n", par_image_height);
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
  lprintf("    --img=FILE                load image of globe (can be used multiple times)\n");
  lprintf("    --interactive[=1]         run in interactive mode\n");
  lprintf("    --threads=NUMBER          specify number of working threads (default: %d)\n", DEF_PAR_THREADS);
  lprintf("    --image-window[=1]        show input image in window (must be before --img)\n");
  lprintf("    --image-width=PIX         set width of input image window (must be before --img, defult: %d)\n", DEF_PAR_IMAGE_WIDTH);
  lprintf("    --image-height=PIX        set height of input image window (must be before --img, default: %d)\n", DEF_PAR_IMAGE_HEIGHT);
}

