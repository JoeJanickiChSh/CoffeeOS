
#define COLS 80
#define ROWS 25

void set_char(unsigned int x, unsigned int y, char color, char c)
{
    if(x < 0 || x >= COLS || y < 0 || y >= ROWS)
    {
        return;
    }
    int address = x * 2 + (y * COLS * 2);
    video_memory[address] = c;
    video_memory[address + 1] = color;
}

void print_at(unsigned int x, unsigned int y, char color, char *text)
{
   
    int i = 0;
    int index = 0;
    int line_x = 0;
    int line_y = 0;
    while (text[index])
    {
        if (text[index] == '\n')
        {
            line_x = 0;
            line_y++;
        }
        else
        {
            set_char(x + line_x, y + line_y, color, text[index]);
            line_x++;   
        }
        index++;
    }
}

void fill(char color)
{
    unsigned int i = 0;
    while (i < (80 * 25 * 2))
    {
        video_memory[i] = ' ';
        i++;
        video_memory[i] = color;
        i++;
    }
}

void reset_screen()
{
    unsigned int i = 0;
    while (i < (80 * 25 * 2))
    {
        video_memory[i] = ' ';
        i++;
        video_memory[i] = 0;
        i++;
    }
}
