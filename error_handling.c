#include <stdio.h>
#include <string.h>
#include "error_handling.h"

void display_error(const char *message) 
{
    fprintf(stderr, "Error: %s\n", message);
}

int check_id3_tag_presence(const char *filename) 
{
    const char *ext = strrchr(filename, '.');
    if (ext && strcmp(ext, ".mp3") == 0) 
    {
        return 1;
    }
    return 0;
}
