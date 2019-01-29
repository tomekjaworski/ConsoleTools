#include <stdio.h>
#include "../new_conio.h"

int clamp(int a, int b, int c)
{
	if (a < b)
		return b;
	if (a > c)
		return c;
	return a;
}

int main(void)
{
    set_screen_size(80, 30);
    hide_cursor();
    int i = 'A';
	int x = 20, y = 20;
    while (1)
    {
	
    	int key = read_key(50);

    	gotoxy(x, y);
		putchar(' ');

		x -= key == VK_LEFT;
		x += key == VK_RIGHT;
		y -= key == VK_UP;
		y += key == VK_DOWN;
		x = clamp(x, 0, 79);
		y = clamp(y, 0, 29);


		gotoxy(x, y);
        if (i % 2 == 0)
            set_foreground(LIGHT_RED);
        else
            set_foreground(LIGHT_GREEN);
        printf("%c", i);
        i++;
        
        if (i == 'Z')
            i = 'A';
    }
}

