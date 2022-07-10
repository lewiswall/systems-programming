#include "types.h"
#include "user.h"
#include "rect.h"

int main(int argc, char* argv[])
{
    struct rect rectangle = {130, 50, 155, 100};
    int hdc = beginpaint(0);
    setpencolour(17, 0, 0, 63);
    
    for(int i = 0; i < 10; i++)
    {
        moveto(hdc, i * 10 + 3, i * 5 + 20);
        selectpen(hdc, 17);
        lineto(hdc, i * 10 + 23, i * 5 + 20);
        lineto(hdc, i * 10 + 23, i * 5 + 40); 
        lineto(hdc, i * 10 + 3, i * 5 + 40); 
        lineto(hdc, i * 10 + 3, i * 5 + 20);
        sleep(1);
    }
    fillrect(hdc, &rectangle);
    endpaint(hdc);
    exit();
}