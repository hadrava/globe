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
	int longdeg;
	int latdeg;
        int ret = fscanf(catalogue_handle, "%d %s %d %d %d %d %d %f %d", &a->num, a->cons, &a->numinc, &longdeg, &a->longmin, &latdeg, &a->latmin, &a->mag, &a->bscn);
	if (ret!=9)
	  break;
	a->longmin += longdeg * 60;
	a->latmin += latdeg * 60;
	dlprintf("%d %s %d %d %d %f %d\n", a->num, a->cons, a->numinc, a->longmin, a->latmin, a->mag, a->bscn);
	i++;
      }
      fclose(catalogue_handle);
      if (cat_size != i) {
        wlprintf("Catalogue file \"%s\" is corrupted: Loaded %d stars, should contain %d stars.\n", par_catalogue_file, i, cat_size);
        cat_size = i;
      }
      else
        lprintf("Loaded %d stars\n", cat_size);
    }
  }
}

