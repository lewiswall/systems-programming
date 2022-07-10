#include "types.h"
#include "user.h"
#include "rect.h"

int main(int argc, char* argv[])
{
    setvideomode(0x13);

    char *arg4[] = { "stage4", 0 };
    char *arg2[] = { "stage2", 0 };
    int fork1;
    int fork2;
    fork1 = fork();

    if(fork1 == 0)
    {
       exec("stage4", arg4);
    }

    fork2 = fork();
    if(fork2 == 0)
    {
        exec("stage2", arg2);
    }
    
    int hdc = beginpaint(0);
    setpencolour(26, 63, 63, 0);

    for(int i = 11; i < 21; i++)
    {
        moveto(hdc, i * 10, i * 5);
        selectpen(hdc, 26);
        lineto(hdc, i * 10 + 20, i * 5);
        lineto(hdc, i * 10 + 20, i * 5 + 20); 
        lineto(hdc, i * 10, i * 5 + 20); 
        lineto(hdc, i * 10, i * 5);
        sleep(1);
    }

    setpixel(hdc, 150, 150);
    setpixel(hdc, 152, 152);
    setpixel(hdc, 154, 154);
    endpaint(hdc);
    wait();
    wait();
    getch();
    setvideomode(0x03);
    exit();

}
   