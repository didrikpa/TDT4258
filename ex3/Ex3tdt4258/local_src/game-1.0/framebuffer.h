#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

// COLOR(5 bit red, 6 bit green, 5 bit blue)
#define COLOR(red, green, blue) (red << 11) + (green << 5) + (blue)

void setupFB(); // used to load framebuffer file and clear screen
void updateRect(int dx, int dy, int width, int height); // update screen on rectangle
void clearScreen(); // clear the entire screen
void drawRect(int x0, int y0, int width, int height, uint16_t *col);
void deleteRect(int x, int y, int length, int height);

#endif
																