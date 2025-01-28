#include "framebuffer.h"
extern char _binary_font_psf_start;
 int xpos;
/*  Save the Y position. */
 int ypos;
/*  Point to the video memory. */
 volatile unsigned char *video;

// TODO: initalizition
//TODO: terminal scrolling
// FIXME: When filesystem implement load from file 
struct PSF_font{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;

};
 struct PSF_font *default_font = (struct PSF_font *)&_binary_font_psf_start;
 unsigned char* fb;
 void cls (void);
 void itoa (char *buf, int base, int d);
 void putchar (int c);
void printf (const char *format, ...);
 unsigned int pixelwidth;
 uint32_t blue_mask;
 uint32_t screen_width;
 uint32_t screen_height;
 unsigned int pitch;
 void putpixel(int x,int y, int color) {
    unsigned where = x*pixelwidth + y*pitch;
    fb[where] = color & 255 ;              // BLUE
    fb[where + 1] = (color >> 8) & 255;   // GREEN
    fb[where + 2] = (color >> 16) & 255;  // RED
}


 void
itoa (char *buf, int base, int d)
{
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;
  
  /*  If %d is specified and D is minus, put ‘-’ in the head. */
  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;

  /*  Divide UD by DIVISOR until UD == 0. */
  do
    {
      int remainder = ud % divisor;
      
      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);

  /*  Terminate BUF. */
  *p = 0;
  
  /*  Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}

/*  Put the character C on the screen. */
 void drawchar(unsigned char c, int x, int y)
{
	int cx,cy;
	int mask[8]={1,2,4,8,16,32,64,128};
	uint8_t* selected_glyph_v1 = (uint8_t*) &_binary_font_psf_start +
	sizeof(struct PSF_font) + (c* default_font->charsize);

	for(cy=0;cy<16;cy++){
		
		for(cx=8;cx>0;cx--){

			if((selected_glyph_v1[cy] & (1<<cx))){
				putpixel(x+8 - cx,y+cy,0xff);
			}
		}
	}
}

 void putchar (int c)
{
  if (c == '\n' || c == '\r')
    {
    newline:
      xpos = 0;
      ypos++;
      if (ypos >= screen_height)
        ypos = 0;
      return;
    }

	drawchar(c,xpos,ypos);

  xpos+= 10 ;
  if (xpos >= screen_width)
    goto newline;
}
