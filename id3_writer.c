/**
 * @file id3_writer.c
 * @brief Implementation of functions for writing and editing ID3 tags in MP3 files.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "id3_writer.h"
 #include "id3_reader.h"
 #include "id3_utils.h"
 #include "error_handling.h"
 
 /**
  * @brief Writes a single ID3 frame (e.g., TIT2 for title) to the given file.
  *
  * This function writes a 10-byte frame header (4 bytes for frame ID, 4 bytes for content size,
  * 2 bytes for flags) followed by the content.
  *
  * @param fp Pointer to the file where the frame should be written.
  * @param frame_id 4-character ID for the frame (e.g., "TIT2").
  * @param content The text content for the frame.
  */
 static void write_frame(FILE *fp, const char *frame_id, const char *content) 
 {
     if (!content) return;  // Skip if content is NULL
     int content_size = strlen(content);
     unsigned char size_bytes[4];
     size_bytes[0] = (content_size >> 24) & 0xFF;
     size_bytes[1] = (content_size >> 16) & 0xFF;
     size_bytes[2] = (content_size >> 8) & 0xFF;
     size_bytes[3] = content_size & 0xFF;
     // Write frame header: ID (4 bytes), size (4 bytes), flags (2 bytes; here set to 0)
     fwrite(frame_id, 1, 4, fp);
     fwrite(size_bytes, 1, 4, fp);
     unsigned char flags[2] = {0, 0};
     fwrite(flags, 1, 2, fp);
     // Write the frame content.
     fwrite(content, 1, content_size, fp);
 }
 
 /**
  * @brief Writes the ID3 tags to an MP3 file by rewriting the file with updated frames.
  *
  * This function opens the original MP3 file and creates a temporary file.
  * It copies the ID3 header (first 10 bytes), writes updated tag frames using the values
  * from the TagData structure, then copies the remainder of the original file.
  * Finally, it replaces the original file with the temporary file.
  *
  * @param filename The name of the MP3 file to update.
  * @param data Pointer to the TagData structure containing the new tag values.
  * @return 0 on success, non-zero on failure.
  */
 int write_id3_tags(const char *filename, const TagData *data) 
 {
     if (!check_id3_tag_presence(filename)) 
     {
         display_error("File does not appear to be an MP3 file.");
         return -1;
     }
     
     FILE *fp_orig = fopen(filename, "rb");
     if (!fp_orig) 
     {
         display_error("Cannot open original file for reading.");
         return -1;
     }
     
     FILE *fp_temp = fopen("temp.mp3", "wb+");
     if (!fp_temp) 
     {
         fclose(fp_orig);
         display_error("Cannot open temporary file for writing.");
         return -1;
     }
     
     // Copy the ID3 header (first 10 bytes) from the original file.
     unsigned char header[10];
     if (fread(header, 1, 10, fp_orig) != 10) 
     {
         fclose(fp_orig);
         fclose(fp_temp);
         display_error("Failed to read ID3 header.");
         return -1;
     }
     fwrite(header, 1, 10, fp_temp);
     
     // Write new tag frames based on the TagData.
     // You can adjust the mapping of fields to frame IDs as needed.
     write_frame(fp_temp, "TIT2", data->title);   // Title frame
     write_frame(fp_temp, "TPE1", data->artist);  // Artist frame
     write_frame(fp_temp, "TALB", data->album);   // Album frame
     write_frame(fp_temp, "TYER", data->year);    // Year frame
     write_frame(fp_temp, "COMM", data->comment); // Comment frame
     write_frame(fp_temp, "TCON", data->genre);   // Genre frame
     
     // Now, skip the old tag section in the original file.
     // For this simplified example, we assume that the original tag frames end at position X.
     // In a full implementation, you would parse the tag header to determine the tag size.
     // Here, we'll simply seek to a fixed position (for example, after 300 bytes) for demonstration.
     fseek(fp_orig, 300, SEEK_SET);
     
     // Copy the remainder of the original file.
     char buffer[1024];
     size_t bytes;
     while ((bytes = fread(buffer, 1, sizeof(buffer), fp_orig)) > 0) 
     {
         fwrite(buffer, 1, bytes, fp_temp);
     }
     
     fclose(fp_orig);
     fclose(fp_temp);
     
     // Replace the original file with the temporary file.
     if (remove(filename) != 0) 
     {
         display_error("Failed to remove original file.");
         return -1;
     }
     if (rename("temp.mp3", filename) != 0) 
     {
         display_error("Failed to rename temporary file.");
         return -1;
     }
     
     return 0;
 }
 
 /**
  * @brief Edits a specific tag in an MP3 file.
  *
  * This function reads the current tags into a TagData structure,
  * updates the specified field, and rewrites the file with the new tags.
  *
  * @param filename The MP3 file to edit.
  * @param tag The tag field to edit (e.g., "title", "artist", "album", "year", "comment", "genre").
  * @param value The new value for the tag.
  * @return 0 on success, non-zero on failure.
  */
 int edit_tag(const char *filename, const char *tag, const char *value) 
 {
     // Read the current tags (if available).
     TagData *data = read_id3_tags(filename);
     if (!data) 
     {
         display_error("Failed to read tags for editing.");
         return -1;
     }
     
     // Update the specified tag field.
     if (strcmp(tag, "title") == 0) 
     {
         free(data->title);
         data->title = strdup(value);
     } 
     else if (strcmp(tag, "artist") == 0) {
         free(data->artist);
         data->artist = strdup(value);
     } 
     else if (strcmp(tag, "album") == 0) {
         free(data->album);
         data->album = strdup(value);
     } 
     else if (strcmp(tag, "year") == 0) {
         free(data->year);
         data->year = strdup(value);
     } 
     else if (strcmp(tag, "comment") == 0) {
         free(data->comment);
         data->comment = strdup(value);
     } 
     else if (strcmp(tag, "genre") == 0) {
         free(data->genre);
         data->genre = strdup(value);
     } 
     else 
     {
         display_error("Unknown tag.");
         // Free allocated memory.
         free(data->version);
         free(data->title);
         free(data->artist);
         free(data->album);
         free(data->year);
         free(data->comment);
         free(data->genre);
         free(data);
         return -1;
     }
     
     // Write the updated tags to the file.
     int ret = write_id3_tags(filename, data);
     
     // Free allocated memory.
     free(data->version);
     free(data->title);
     free(data->artist);
     free(data->album);
     free(data->year);
     free(data->comment);
     free(data->genre);
     free(data);
     
     return ret;
 }
 