typedef struct File {
    char data[1024];
    char name[16];
    unsigned int size;
} File;

File fs[64];

void fs_init()
{
    int i;
    for(i = 0; i < 64; i++)
    {
        fs[i].size = 0;
    }
}

File *file_open(char *fname) {
    int i;
    int file_index = -1;
    File *fs_ptr = fs;
    for(i = 0; i < 64; i++)
    {
        if(string_cmp(fs_ptr->name, fname) && fs_ptr->size != 0)
        {
            file_index = i;
            break;
        }
        fs_ptr++;
    }

    if(file_index == -1)
    {
        fs_ptr = fs;
        for(i = 0; i < 64; i++)
        {
            if(fs_ptr->size == 0)
            {
                file_index = i;
                break;
            }
            fs_ptr++;
        }
        fs_ptr->size = 1;
    }
    
    string_copy(fs_ptr->name, fname);

    return fs_ptr;
}

char *file_read(File *f)
{
    return f->data;
}

char *file_write(File *f, char *text)
{
    string_copy(f->data, text);
    f->size = string_len(text) + 1;
}
