#include "types.h"
#include "user.h"
#include "rect.h"

int main(int argc, char* argv[])
{ 
    struct rect rectangle = {100, 50, 125, 100};
    int hdc = beginpaint(0);
    setpencolour(23, 63, 0, 0);
    
    for(int i = 0; i < 10; i++)
    {
        moveto(hdc, i * 10, i * 5);
        selectpen(hdc, 23);
        lineto(hdc, i * 10 + 20, i * 5);
        lineto(hdc, i * 10 + 20, i * 5 + 20); 
        lineto(hdc, i * 10, i * 5 + 20); 
        lineto(hdc, i * 10, i * 5);
        sleep(1);
    }
    fillrect(hdc, &rectangle);

    endpaint(hdc);
    exit();
}