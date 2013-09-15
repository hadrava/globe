#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include "log.h"
#include "params.h"

static int log_opened = 0;
FILE * log_handle = NULL;


void log_init() {
  if (par_log_file) {
    log_handle = fopen(par_log_file, "a");
    if (log_handle) {
      time_t time_now = time(NULL);
      struct tm * now = localtime(&time_now);
      log_opened = 1;
      lprintf("Opening logfile \"%s\" on %s", par_log_file, asctime(now));
      atexit(&log_close);
    }
  }
}

void log_close() {
  if (log_opened && log_handle) {
    time_t time_now = time(NULL);
    struct tm * now = localtime(&time_now);
    lprintf("Closing logfile \"%s\" on %s", par_log_file, asctime(now));
    log_opened = 0;
    fclose(log_handle);
  }
}

void dlprintf(const char *format, ...) {
  if (par_verbose >= 2) {
    va_list args;
    if (log_opened) {
      va_start(args, format);
      vfprintf(log_handle, format, args);
      va_end(args);
    }

    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
  }
}

void vlprintf(const char *format, ...) {
  if (par_verbose) {
    va_list args;
    if (log_opened) {
      va_start(args, format);
      vfprintf(log_handle, format, args);
      va_end(args);
    }

    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
  }
}

void lprintf(const char *format, ...) {
  va_list args;
  if (log_opened) {
    va_start(args, format);
    vfprintf(log_handle, format, args);
    va_end(args);
  }

  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
}

void wlprintf(const char *format, ...) {
  if (par_verbose+1) {
    va_list args;
    if (log_opened) {
      fprintf(log_handle, "Warning: ");
      va_start(args, format);
      vfprintf(log_handle, format, args);
      va_end(args);
    }

    fprintf(stderr, "Warning: ");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
  }
}
void elprintf(const char *format, ...) {
  va_list args;
  if (log_opened) {
    fprintf(log_handle, "ERROR: ");
    va_start(args, format);
    vfprintf(log_handle, format, args);
    va_end(args);
  }

  fprintf(stderr, "ERROR: ");
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
}
