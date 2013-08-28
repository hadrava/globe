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

void parse_params(int argc, char *argv[]);
void print_params();
void print_available_params();

#endif
