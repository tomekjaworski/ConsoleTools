#include <stdio.h>
#include "new_conio.h"


int main(void)
{
    set_screen_size(80, 30);
    hide_cursor();
    int i = 'A';
    while (1)
    {
        gotoxy(20, 20);
        if (i % 2 == 0)
            set_foreground(LIGHT_RED);
        else
            set_foreground(LIGHT_GREEN);
        printf("%c", i);
        i++;
        
        if (i == 'Z')
            i = 'A';
        sleep(50);
    }
}

