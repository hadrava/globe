#include <stdio.h>
#include <stdlib.h>
#include "catalogue.h"
#include "params.h"
#include "log.h"

FILE * catalogue_handle = NULL;
int cat_size = 0;
struct star * cat_stars = NULL;

void catalogue_close() {
  if (cat_stars)
    free(cat_stars);
}

void catalogue_read() {
  if (par_catalogue_file) {
    catalogue_handle = fopen(par_catalogue_file, "r");
    if (catalogue_handle) {
      int i = 0;
      lprintf("Reading catalogue \"%s\"\n", par_catalogue_file);

      fscanf(catalogue_handle, "%d\n", &cat_size);
      cat_stars = malloc(cat_size * sizeof(struct star));
      atexit(&catalogue_close);
      while ((i<cat_size) && (!feof(catalogue_handle))) {
        struct star * a = cat_stars+i;
	int longmin;
	int latmin;
        fscanf(catalogue_handle, "%d %s %d %d %d %d %d %f %d", &a->num, a->cons, &a->numinc, &longmin, &a->longsec, &latmin, &a->latsec, &a->mag, &a->bscn);
	a->longsec += longmin * 60;
	a->latsec += latmin * 60;
	dlprintf("%d %s %d %d %d %f %d\n", a->num, a->cons, a->numinc, a->longsec, a->latsec, a->mag, a->bscn);
	i++;
      }
      fclose(catalogue_handle);
      lprintf("Loaded %d stars\n", cat_size);
    }
  }
}

