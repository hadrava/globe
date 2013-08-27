#ifndef PARSE_PARAMS_H_
#define PARSE_PARAMS_H_

extern int par_verbose;
extern int par_help;
extern char * par_logfile;

void parse_params(int argc, char *argv[]);
void print_params();
void print_available_params();

#endif
