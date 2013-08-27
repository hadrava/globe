#ifndef CATALOGUE_H_
#define CATALOGUE_H_

struct star {
  int num;
  char cons[4];
  int numinc;
  int longmin;
  int latmin;
  float mag;
  int bscn;
};

extern int cat_size;
extern struct star * cat_stars;

void catalogue_close();
void catalogue_read();

#endif
