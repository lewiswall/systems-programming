#include "types.h"
#include "user.h"
#include "rect.h"

int main(int argc, char* argv[])
{
    setvideomode(0x13);
    int hdc = beginpaint(0);
    setpencolour(25, 0, 0, 63);
    selectpen(hdc, 25);
    
    moveto(hdc, 100, 50);
    lineto(hdc, 200, 50);
    lineto(hdc, 200, 150);
    lineto(hdc, 100, 150);
    lineto(hdc, 100, 50);
    lineto(hdc, 200, 150);
    moveto(hdc, 100, 150);
    lineto(hdc, 200, 50);
    getch();
    setvideomode(0x03);
    exit();
}