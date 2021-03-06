#ifndef PARSE_PARAMS_H_
#define PARSE_PARAMS_H_

extern int par_verbose;
extern int par_help;
extern char * par_log_file;
extern char * par_catalogue_file;
extern int par_sph_width;
extern int par_sph_height;
extern char * par_sph_file;
extern int par_sph_win;
extern float par_star_size;
extern char * par_drawings_file;
extern float par_drawings_size;
extern int par_coor_draw;
extern int par_coor_step;
extern int par_interactive;
extern int par_threads;
extern int par_image_win;
extern int par_image_width;
extern int par_image_height;
extern int par_stereo_width;
extern int par_stereo_height;
extern double par_stereo_latmin;
extern double par_stereo_longmin;
extern double par_stereo_zrotationmin;
extern double par_stereo_size_factor;

void parse_params(int argc, char *argv[]);
void print_params();
void print_available_params();

#endif
