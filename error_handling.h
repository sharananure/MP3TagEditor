#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

/**
 * @brief Displays an error message to the console.
 *
 * This function prints the provided error message to standard error (stderr).
 * It is used for reporting issues encountered during MP3 tag reading/writing.
 *
 * @param message The error message to be displayed.
 */
void display_error(const char *message);

/**
 * @brief Checks if an MP3 file contains an ID3 tag.
 *
 * This function verifies whether the given MP3 file has a valid ID3 metadata tag
 * by reading the file header. It helps determine if the file supports tag editing.
 *
 * @param filename The name of the MP3 file to check.
 * @return 1 if an ID3 tag is present, 0 otherwise.
 */
int check_id3_tag_presence(const char *filename);

#endif // ERROR_HANDLING_H
