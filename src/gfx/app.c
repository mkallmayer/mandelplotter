#include <stdio.h>
#include "gfx.h"
#include "complex.h"

// gcc app.c gfx.c -o app -lX11 -lm

const int HEIGHT    = 1000;
const int WIDTH     = 1000;
const int MAX_ITERS = 75;

int iterate(long double complex z, long double complex s)
{
    int iters = 0;
    while (creal(s)*creal(s) + cimag(s)*cimag(s) <= 4 && iters < MAX_ITERS)
    {
         s = s * s + z;
         iters++;
    }
    return iters;
}

long double complex to_complex(int x, int y)
{
    // takes grid coordinates and returns corresponding point on complex plane
    return (x-(WIDTH/2)) * (2.0/WIDTH) - 0.5 + (y-(HEIGHT/2.0)) * (2.0/HEIGHT) * I;
}

void calc_draw(long double complex s)
{
    gfx_clear();

    int values[WIDTH][HEIGHT];
    
    // calculate all the values first
    for (int x=0; x<WIDTH; x++)
    {
        for (int y=0; y<HEIGHT; y++)
        {
            long double complex z = to_complex(x, y);
            values[x][y] = iterate(z, s);
        }
    }

    // draw pixels
    for (int x=0; x<WIDTH; x++)
    {
        for (int y=0; y<HEIGHT; y++)
        {
            int r, g, b;
            float val = (float) values[x][y]/MAX_ITERS * 255.0;
            r = (int) (val * (100.0 / 255.0));
            g = (int) (val * (195.0 / 255.0));
            b = (int) (val * (220.0 / 255.0));
            gfx_color(r, g, b);
            gfx_point(x, y);
        }
    }
}

void redraw_single(int x, int y, long double complex s)
{
    long double complex z = to_complex(x, y);
    int r, g, b;
    float val = (float) iterate(z, s)/MAX_ITERS * 255.0;
    r = (int) (val * (100.0 / 255.0));
    g = (int) (val * (195.0 / 255.0));
    b = (int) (val * (220.0 / 255.0));
    gfx_color(r, g, b);
    gfx_point(x, y);

}

void crosshair(int x, int y)
{
    gfx_color(100, 100, 200);
    gfx_line(x-3, y, x+3, y);
    gfx_line(x, y-3, x, y+3);
    // gfx_point(x, y);
    return;
}

int main()
{
	char c;

	// Open a new window for drawing.
	gfx_open(WIDTH,HEIGHT,"mandel");

	// Set the current drawing color to green.
	gfx_color(0, 200, 100);

	// Draw a triangle on the screen.

        int ch_x, ch_y;  //crosshair pos
        ch_x = WIDTH/2;
        ch_y = HEIGHT/2;

        long double complex s;

	while(1) {
		// Wait for the user to press a character.
		c = gfx_wait();

		// Quit if it is the letter q.
		if(c==' ') { s = to_complex(ch_x, ch_y); calc_draw(s); }
                if(c=='w') { ch_y -= 5; crosshair(ch_x, ch_y); redraw_single(ch_x, ch_y+5, s); }
                if(c=='a') { ch_x -= 5; crosshair(ch_x, ch_y); redraw_single(ch_x+5, ch_y, s); }
                if(c=='s') { ch_y += 5; crosshair(ch_x, ch_y); redraw_single(ch_x, ch_y-5, s); }
                if(c=='d') { ch_x += 5; crosshair(ch_x, ch_y); redraw_single(ch_x-5, ch_y, s); }
		if(c=='q') break;
	}

	return 0;
}
