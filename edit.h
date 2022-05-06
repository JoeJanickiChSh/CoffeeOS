void app_edit_update(File *fp, int window_x, int window_y)
{
    char *fdata = file_read(fp);
    int c_x = 0, c_y = 0;
    while(*fdata)
    {
        if(*fdata == '\n')
        {
            set_char(window_x+c_x, window_y+c_y, window_color, 0);
            c_x = 0;
            c_y++;
        } 
        else
        {
            set_char(window_x+c_x, window_y+c_y, window_color, *fdata);
            c_x++;
        }
        fdata++;
    }
    set_char(window_x+c_x, window_y+c_y, window_inverse, 0);
}
void app_edit_clear(File *fp, int window_x, int window_y)
{
    char *fdata = file_read(fp);
    int c_x = 0, c_y = 0;
    while(*fdata)
    {
        if(*fdata == '\n')
        {
            c_x = 0;
            c_y++;
        } 
        else
        {
            c_x++;
        }
        fdata++;
    }
    set_char(window_x+c_x, window_y+c_y, window_color, 0);
}


void app_edit(File *fp)
{
    unsigned char running = 1;
    int window_x = 11, window_y = 4;
    char key;
    char prev_key = '\n';
    char arrow;
    char prev_arrow;
    window_at(9,2,64,20,fp->name);

    unsigned int text_ptr = 0;


    app_edit_update(fp, window_x,window_y);
    int length;

    while(running)
    {
        key = keyboard_get();
        arrow = keyboard_arrow();
        if(key && key != prev_key)
        {
            if(key == 8)
            {
               
                app_edit_clear(fp, window_x,window_y);

                length = string_len(file_read(fp));
                fp->data[length-1] = 0;
                fp->size--; 
                
            }
            else
            {
                length = string_len(file_read(fp));
                fp->data[length] = key;
                fp->data[length+1] = 0;
                fp->size++;
            }
            app_edit_update(fp, window_x,window_y);

        }

        if(arrow && arrow != prev_arrow)
        {
            if(arrow == 2)
            {
                running = 0;
                fp->size = string_len(file_read(fp));
            }
        }

        prev_key = key;
        prev_arrow = arrow;
    }
}