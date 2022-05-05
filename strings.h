void string_copy(char *dest, char *src)
{
    char *dest_ptr = dest;
    char *src_ptr = src;

    while(*src_ptr)
    {
        *dest_ptr = *src_ptr;
        dest_ptr++;
        src_ptr++;
    }
    *dest_ptr = 0;
}

int string_len(char *string)
{
    int i = 0;
    while(string[i])
    {
        i++;
    }
    return i;
}

unsigned char string_cmp(char *a, char *b)
{
    int alen = string_len(a); 
    int i;
    if(alen != string_len(b))
    {
        return 0;
    }

    for(i = 0; i < alen; i++)
    {
        if(a[i] != b[i])
        {
            return 0;
        }
    }

    return 1;

}



