#ifndef ID3_UTILS_H
#define ID3_UTILS_H

#include <stdlib.h>

/**
 * @brief Structure to hold ID3 tag data.
 */
typedef struct 
{
    char *version; /**< Version of the ID3 tag */
    char *title;   /**< Title of the song */
    char *artist;  /**< Artist of the song */
    char *album;   /**< Album name */
    char *year;    /**< Year of release */
    char *comment; /**< Comment */
    char *genre;   /**< Genre */
    // Add other fields as needed
} TagData;

/**
 * @brief Creates a new TagData structure.
 *
 * @return Pointer to a newly allocated TagData structure.
 */
TagData* create_tag_data();

#endif // ID3_UTILS_H
