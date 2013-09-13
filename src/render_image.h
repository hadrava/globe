#ifndef RENDER_IMAGE_H_
#define RENDER_IMAGE_H_

void *render_thread_main(void *arg);
void render_images_threaded(int line);
void render_image_init(int threads);
void render_image_close();
void render_images();

#endif
