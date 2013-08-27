#include <stdio.h>
#include <string.h>
#include "params.h"
#include "log.h"

#define PARSE_PAR(PARNAME, PARTYPE, PARVARIABLE) if ((sizeof(PARNAME)==value-argv[i]) && (strncmp(argv[i],PARNAME,sizeof(PARNAME)-1) == 0)) { sscanf(value,PARTYPE,&PARVARIABLE); continue; }
#define PARSE_STRING_PAR(PARNAME, PARVARIABLE) if ((sizeof(PARNAME)==value-argv[i]) && (strncmp(argv[i],PARNAME,sizeof(PARNAME)-1) == 0)) { PARVARIABLE=value; continue; }
#define DETECT_OPT(PARNAME, PARCOMMAND) if (strcmp(argv[i],PARNAME) == 0) { PARCOMMAND; continue; }

int par_verbose = 0;
int par_help = 0;
char * par_logfile = NULL;

void parse_params(int argc, char *argv[]) {
  for(int i=1; i<argc; i++) {
    char* value = strchr(argv[i], '=');
    if (value) {
      value++;
      PARSE_PAR("--verbose", "%d", par_verbose);
      PARSE_STRING_PAR("--log", par_logfile);
    }
    else {
      DETECT_OPT("--verbose", par_verbose++);
      DETECT_OPT("--help", par_help = 1);
    }
    fprintf(stderr, "Unknown parameter: \"%s\"\n", argv[i]); //TODO: how to log this? Log is not prepared yet...
  }
}

void print_params() {
  lprintf("--verbose=%d\n", par_verbose);
  lprintf("--help=%d\n", par_help);
  lprintf("--log=%s\n", par_logfile);
}

void print_available_params(){
  lprintf("Usage: globe [OPTION]\n");
  lprintf("Available parameters:\n");
  lprintf("    --verbose[=LEVEL]     set or increase verbosity level\n");
  lprintf("    --help                display this help message and exit\n");
  lprintf("    --log=FILE            set path to log file\n");
}

