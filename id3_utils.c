#include <stdlib.h>
#include <string.h>
#include "id3_utils.h"

/**
 * @brief Allocates and initializes a new TagData structure.
 *
 * This function dynamically allocates memory for a TagData structure
 * and initializes all string pointers to NULL. The caller is responsible
 * for filling in the fields and freeing the allocated memory using free_tag_data().
 *
 * @return A pointer to the newly allocated TagData structure, or NULL if allocation fails.
 */
TagData* create_tag_data() 
{
    TagData *data = (TagData *)malloc(sizeof(TagData));
    if (data) 
    {
        data->version = NULL;
        data->title = NULL;
        data->artist = NULL;
        data->album = NULL;
        data->year = NULL;
        data->comment = NULL;
        data->genre = NULL;
        // Initialize other fields as needed
    }
    return data;
}

/**
 * @brief Frees memory allocated for a TagData structure.
 *
 * This function deallocates all dynamically allocated memory within
 * a TagData structure, including each individual string field. Finally,
 * it frees the TagData structure itself.
 *
 * @param data Pointer to the TagData structure to be freed.
 */
void free_tag_data(TagData *data) 
{
    if (data) 
    {
        free(data->version);
        free(data->title);
        free(data->artist);
        free(data->album);
        free(data->year);
        free(data->comment);
        free(data->genre);
        // Free other fields as needed
        free(data);
    }
}
