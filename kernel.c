#define MIN(a,b) ((a > b) ? (b) : (a))
#define MAX(a,b) ((a > b) ? (a) : (b))

#include "registers.h"
#include "strings.h"
#include "kmath.h"

#include "screen.h"
#include "ports.h"
#include "keyboard.h"
#include "files.h"


#include "colors.h"

char terminal_color = BGFG(DARK_YELLOW, WHITE);
char terminal_inverse = BGFG(WHITE,DARK_YELLOW);
char window_color = BGFG(BLUE,WHITE);
char window_inverse = BGFG(WHITE, BLUE);
char close_color = BGFG(RED,WHITE);

void overlay()
{
    int i;
    for(i = 0; i < COLS; i++)
    {
        set_char(i, 0, terminal_inverse, '=');
        set_char(i, ROWS - 1, terminal_inverse, '-');
    }
    for(i = 0; i < ROWS; i++)
    {
        set_char(0, i, terminal_inverse, '|');
        set_char(COLS - 1, i, terminal_inverse, '|');
    }

    set_char(0,0,terminal_inverse, '+');
    set_char(COLS-1,0,terminal_inverse, '+');
    set_char(0,ROWS - 1,terminal_inverse, '+');
    set_char(COLS-1,ROWS - 1,terminal_inverse, '+');

    print_at(2,0,terminal_inverse, "CoffeeOS");
}

void window_at(unsigned int x, unsigned int y, unsigned int w, unsigned int h, char *title)
{
    int ix,iy;
    for(ix = 0; ix < w; ix++)
    {
        for(iy = 0; iy < h; iy++)
        {
            set_char(x + ix, y + iy, window_color, ' ');
            set_char(x, y+iy, window_inverse, '|');
            set_char(x + w - 1, y+iy, window_inverse, '|');
        }
        set_char(x + ix, y, window_inverse, '=');
        set_char(x + ix, y + h - 1, window_inverse, '-');
    }
    set_char(x,y, window_inverse, '+');
    set_char(x + w-1,y, window_inverse, '+');
    set_char(x,y + h - 1, window_inverse, '+');
    set_char(x + w - 1,y + h - 1, window_inverse, '+');

    print_at(x+2, y, window_inverse, title);
    print_at(x+w-5, y, close_color, "[X]");
}


void screen_update()
{
    fill(terminal_color);
    window_at(3,3, 64 ,16, "Terminal");
    overlay();
}

char termscreen[56][12] = {0};
int cursor_x = 0;

void k_update()
{
    int x,y;
    for(x = 0; x < 56; x++)
    {
        for(y = 0; y < 12; y++)
        {
            set_char(5 + x, 5 + y, window_color, termscreen[x][y]);
        }
    }

    set_char(5 + cursor_x, 5+ 11, window_inverse, 0);
}

void k_newline()
{
    int x,y;
    for(x = 0; x < 56; x++)
    {
        for(y = 0; y < 11; y++)
        {
            termscreen[x][y] = termscreen[x][y+1];
        }
        termscreen[x][11] = 0;
    }
    cursor_x = 0;

    k_update();
}


void k_prints(char *str)
{
    int i;
    int len = string_len(str);
    for(i = 0; i < len; i++)
    {
        if(str[i] == '\n')
        {
            k_newline();
        }
        else
        {
            termscreen[cursor_x++][11] = str[i];
        }
    }
    k_update();
}

void k_printc(char c)
{
    if(c == '\n')
    {
        k_newline();
    }
    else
    {
        termscreen[cursor_x++][11] = c;
    }
    k_update();

}

void k_printi(int x)
{
    int num = (x < 0) ? (-x) : (x);
    if(x < 0)
    {
        k_printc('-');
    }
    int digits = 0;
    int i, j;
    int pow;
    if(num != 0)
    {
        digits = (int)k_log(num);
    }
    for(i = digits; i >= 0; i--)
    {
        pow = 1;
        for(j = 0; j < i; j++)
        {
            pow *= 10;
        }
        k_printc(((num  / pow) % 10) + '0');
    }
    k_update();


}

void k_printf(float x)
{
    k_printi((int)x);
    k_printc('.');
    float rest = x - ((int)x);
    if(rest < 0)
    {
        rest = -rest;
    }
    int i, j;
    int pow;
    for(i = 0; i < 8; i++)
    {
        pow = 1;
        for(j = 0; j <= i; j++)
        {
            pow *= 10;
        }
        k_printc( ((int)(rest * pow)) % 10 + '0' );
    }
    k_update();
}


char scans_prevkey;
void k_scans(char *target)
{
    unsigned char scanning = 1;
    k_prints("\n> ");
    char key;
    char *targetptr = target;
    while(scanning)
    {
        key = keyboard_get();
        if(key && key != scans_prevkey)
        {
            if(key == '\n')
            {
                termscreen[cursor_x][11] = 0;
                k_printc('\n');
                scanning = 0;
            }
            else if(key == 8)
            {
                termscreen[cursor_x--][11] = 0;
                *targetptr = 0;
                if(cursor_x >= 2)
                {
                    targetptr--;

                }
            }
            else
            {
                k_printc(key);
                *targetptr = key;
                targetptr++;
            }
            k_update();
        }
        scans_prevkey = key;
        cursor_x = MAX(2,cursor_x);
    }
    *targetptr = 0;
    cursor_x = 0;

}

char inp[64];
void kernel_main()
{

    fs_init();
    keyboard_init();

    reset_screen();
   
    screen_update();

    int cursor_x = 0;

    char prevchar;
    char inchar;
    int i;


    File *fp;

    for(;;)
    {
        k_scans(inp);

        if(string_cmp(inp,"ls"))
        {
            for(i = 0; i < 64; i++)
            {
                if(fs[i].size != 0)
                {
                    k_prints(fs[i].name);
                    k_prints(" - Size: ");
                    k_printi(fs[i].size);
                    k_prints(" bytes\n");
                }
            }
        }
        else if(string_cmp(inp, "touch"))
        {
            k_prints("File Name:");
            k_scans(inp);
            file_open(inp);
        }
        else if(string_cmp(inp, "delete"))
        {
            k_prints("File Name:");
            k_scans(inp);
            fp = file_open(inp);
            fp->size = 0;
        }
        else if(string_cmp(inp, "cat"))
        {
            k_prints("File Name:");
            k_scans(inp);
            fp = file_open(inp);
            k_prints(file_read(fp));
        }
        else if(string_cmp(inp, "write"))
        {
            k_prints("File Name:");
            k_scans(inp);
            fp = file_open(inp);
            k_prints("Data:");
            k_scans(inp);
            file_write(fp,inp);
        }
        else if(string_cmp(inp, "calc"))
        {
            k_prints("Expression:");
            k_scans(inp);   
            k_printf(k_eval(inp));
            k_printc('\n');
        }
        else
        {
            k_prints("Error: '");
            k_prints(inp);
            k_prints("' is not a command.\n");
        }

    }
   
}
