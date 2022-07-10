#include "types.h"
#include "defs.h"
#include "memlayout.h"
#include "x86.h"
#include "rect.h"
#include "spinlock.h"
#include "window.h"

struct {
	struct spinlock lock;
	struct window window[20];
} windows;

void clear320x200x256() {
	// You need to put code to clear the video buffer here.  Initially, 
	// you might just set each pixel to black in a nested loop, but think
	// about faster ways to do it. 
	//
	// This function is called from videosetmode.
	uchar *a = (uchar*)P2V(0xA0000);
	
	for (int x = 0; x < 320; x++){
		for (int y = 0; y < 200; y++){
			a[320 * y + x] = 0;
		}
	}
}

int sys_setpixel(void){
	
	int hdc;

	if (argint(0, &hdc) < 0){
        cprintf("failure\n");
        return -1;
    }

	if (argint(1, &windows.window[hdc].x) < 0){
		cprintf("failure\n");
        return -1;
	}

	if (argint(2, &windows.window[hdc].y) < 0){
		cprintf("failure\n");
        return -1;
	}
	acquire(&windows.lock);

	uchar *a = (uchar*)P2V(0xA0000);

	a[320 * windows.window[hdc].y + windows.window[hdc].x] = windows.window[hdc].index;
	release(&windows.lock);
	return 0;
}

int sys_moveto(void){
	int hdc;

	if (argint(0, &hdc) < 0){
        cprintf("failure\n");
        return -1;
    }

	if (argint(1, &windows.window[hdc].x) < 0){			
		cprintf("failure\n");
        return -1;
	}

	if (argint(2, &windows.window[hdc].y) < 0){			
		cprintf("failure\n");
        return -1;
	}
	
	if(windows.window[hdc].x < 0){
		windows.window[hdc].x = 0;
	}
	else if(windows.window[hdc].x >= 320){
		windows.window[hdc].x = 319;
	}

	if(windows.window[hdc].y < 0){
		windows.window[hdc].y = 0;
	}
	else if(windows.window[hdc].y >= 200){
		windows.window[hdc].y = 199;
	}
	return 0;
}

int sys_lineto(void){

	int hdc;

	int fx;
	int fy;

	int dx;
	int dy;
	int d;

	int posdx;
	int posdy;



	if (argint(0, &hdc) < 0){
        cprintf("failure\n");
        return -1;
    }

	if (argint(1, &fx) < 0){
		cprintf("failure\n");
        return -1;
	}

	if (argint(2, &fy) < 0){
		cprintf("failure\n");
        return -1;
	}

	acquire(&windows.lock);

	uchar *a = (uchar*)P2V(0xA0000);
	a[320 * windows.window[hdc].y + windows.window[hdc].x] = windows.window[hdc].index;

	release(&windows.lock);

	if(fx < 0){
		fx = 0;
	}
	else if(fx >= 320){
		fx = 319;
	}

	if(fy < 0){
		fy = 0;
	}
	else if(fy >= 200){
		fy = 199;
	}


	dx = fx - windows.window[hdc].x;
	dy = fy - windows.window[hdc].y;

	if (dx < 0){
		posdx = dx * -1;
	}
	else{
		posdx = dx;
	}

	if (dy < 0){
		posdy = dy * -1;
	}
	else{
		posdy = dy;
	}

	

	if (posdx > posdy) {			//if slope is smaller than 1
		d = 2 * posdy - posdx;

		while(windows.window[hdc].x != fx){
			if(dx < 0){
				windows.window[hdc].x = windows.window[hdc].x -1;
			}
			else{
				windows.window[hdc].x = windows.window[hdc].x + 1;
			}

			if(d < 0){
				d = d + 2 * posdy;
			}
			else{
				if(dy < 0){
					windows.window[hdc].y = windows.window[hdc].y -1;
				}
				else{
					windows.window[hdc].y = windows.window[hdc].y + 1;
				}
				d = d + (2 * posdy) - (2 * posdx);
			}
			acquire(&windows.lock);
			uchar *a = (uchar*)P2V(0xA0000);
			a[320 * windows.window[hdc].y + windows.window[hdc].x] = windows.window[hdc].index;
			release(&windows.lock);
		}
		
	}
	else{					// when slope is greater or equal to 1
		d = 2 * posdx - posdy;
		
		while(windows.window[hdc].y != fy){
			if(dy < 0){
				windows.window[hdc].y = windows.window[hdc].y -1;
			}
			else{
				windows.window[hdc].y = windows.window[hdc].y + 1;
			}

			if(d < 0){
				d = d + 2 * posdx;
			}
			else{
				if(dx < 0){
					windows.window[hdc].x = windows.window[hdc].x - 1;
				}
				else{
					windows.window[hdc].x = windows.window[hdc].x + 1;
				}
				d = d + (2 * posdx) - (2 * posdy);
			}
			acquire(&windows.lock);
			uchar *a = (uchar*)P2V(0xA0000);
			a[320 * windows.window[hdc].y + windows.window[hdc].x] = windows.window[hdc].index;
			release(&windows.lock);
			
		}
		
	}
	return 0;

}

// was used for debugging - changed based on the scenario 
int sys_print(void){		
	cprintf("hello");
	return 0;
}

int sys_setpencolour(void){
	int r;
	int g;
	int b;
	int set;
	

	if (argint(0, &set) < 0){
        cprintf("failure\n");
        return -1;
    }

	if (argint(1, &r) < 0){
        cprintf("failure\n");
        return -1;
    }

	if (argint(2, &g) < 0){
        cprintf("failure\n");
        return -1;
    }

	if (argint(3, &b) < 0){
        cprintf("failure\n");
        return -1;
    }

	if(set > 255 || set < 16){
		cprintf("index must be in range 16 - 255\n");
		return -1;
	}

	if(r < 0){
		r = 0;
	}
	else if(r > 63){
		r = 63;
	}

	if(g < 0){
		g = 0;
	}
	else if(g > 63){
		g = 63;
	}

	if(b < 0){
		b = 0;
	}
	else if(b > 63){
		b = 63;
	}

	cprintf("red: %d, green: %d, blue: %d\n", r, g, b);

	acquire(&windows.lock);

	outb(0x3C8, set);
	outb(0x3C9, r);
	outb(0x3C9, g);
	outb(0x3C9, b);

	release(&windows.lock);

	return 0;

}

int sys_selectpen(void){
	
	int select;
	int hdc;

	if (argint(0, &hdc) < 0){
        cprintf("failure\n");
        return -1;
    }

	if (argint(1, &select) < 0){
        cprintf("failure\n");
        return -1;
    }


	if(select < 16 || select > 255){
		cprintf("index must be in range 16 - 255\n");
		return -1;
	}
	else{
		windows.window[hdc].index = select;
		cprintf("index: %d\n", windows.window[hdc].index);
	}

	return 0;
}

int sys_fillrect(void){
	struct rect *rectangle;
	int hdc;

	if (argint(0, &hdc) < 0){
        cprintf("failure\n");
        return -1;
    }
	if (argptr(1, (void*)&rectangle, sizeof(*rectangle)) < 0){
        cprintf("failure\n");
        return -1;
    }
	acquire(&windows.lock);

	uchar *a = (uchar*)P2V(0xA0000);
 
	for(int i = rectangle->left; i <= rectangle->right; i++){
		for(int j = rectangle->top; j <= rectangle->bottom; j++){
			a[320 * j + i] = windows.window[hdc].index;
		}
	}

	release(&windows.lock);

	return 0;
}

int sys_beginpaint(void){
	struct window* win;
	int counter = 0;
	
	acquire(&windows.lock);
	
	for(win = windows.window; win < windows.window + 20; win++){
		if(win->available == 0){
			win->available = 1;
			release(&windows.lock);
			return counter;
		}
		counter++;
	}
	release(&windows.lock);
	
	return -1;
}

int sys_endpaint(void){
	int hdc;

	if (argint(0, &hdc) < 0){
        cprintf("failure\n");
        return -1;
    }

	acquire(&windows.lock);
	windows.window[hdc].available = 0;
	release(&windows.lock);
	return 0;
}

void initGraphicsLock(){
	initlock(&windows.lock, "graphLock");
}

