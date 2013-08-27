#ifndef LOG_H_
#define LOG_H_

void log_init();
void log_close();

void vlprintf(const char *format, ...);
void lprintf(const char *format, ...);
void wlprintf(const char *format, ...);
void elprintf(const char *format, ...);

#endif
