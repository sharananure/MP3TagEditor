#ifndef ID3_READER_H
#define ID3_READER_H

#include "id3_utils.h"

/**
 * @brief Reads ID3 metadata tags from an MP3 file.
 *
 * This function reads ID3 tags from the given MP3 file and stores them
 * in a dynamically allocated TagData structure. The caller is responsible
 * for freeing the allocated memory using free_tag_data().
 *
 * @param filename The name of the MP3 file to read.
 * @return A pointer to a TagData structure containing the metadata,
 *         or NULL if an error occurs.
 */
TagData* read_id3_tags(const char *filename);

/**
 * @brief Displays the metadata stored in a TagData structure.
 *
 * This function prints the metadata (e.g., title, artist, album, year, comment, genre)
 * stored in the given TagData structure to the console.
 *
 * @param data Pointer to the TagData structure containing metadata to display.
 */
void display_metadata(const TagData *data);

/**
 * @brief Reads and displays the ID3 metadata from an MP3 file.
 *
 * This function reads ID3 tags from the specified MP3 file and prints
 * the metadata information to the console.
 *
 * @param filename The name of the MP3 file to read and display metadata from.
 */
void view_tags(const char *filename);

#endif // ID3_READER_H
