/**
 * @file id3_reader.c
 * @brief Implementation of functions for reading ID3 tags from MP3 files.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "id3_reader.h"
 #include "error_handling.h"
 
 #define FRAME_HEADER_SIZE 10
 
 /**
  * @brief Reads the ID3 tags from an MP3 file by parsing the actual ID3v2 frames.
  *
  * This implementation reads the ID3 header, calculates the tag size, and then iterates
  * through each frame to extract the frame content for known frames.
  *
  * @param filename The name of the MP3 file.
  * @return Pointer to a TagData structure with tag data, or NULL on failure.
  */
 TagData* read_id3_tags(const char *filename) 
 {
     if (!check_id3_tag_presence(filename)) 
     {
        display_error("File does not appear to be an MP3 file.");
        return NULL;
     }
     
     // Open file in Read binary mode
     FILE *fp = fopen(filename, "rb");
     if (!fp) 
     {
        display_error("Cannot open file for reading.");
        return NULL;
     }
     
     // Read the ID3 header (first 10 bytes)
     unsigned char header[10];
     if (fread(header, 1, 10, fp) != 10) 
     {
        display_error("Failed to read ID3 header.");
        fclose(fp);
        return NULL;
     }
     
     // Verify that the header starts with "ID3"
     if (memcmp(header, "ID3", 3) != 0) 
     {
         display_error("No ID3 tag found.");
         fclose(fp);
         return NULL;
     }
     
     // Calculate tag size (sync-safe integer in bytes 6-9)
     int tagSize = ((header[6] & 0x7F) << 21) |
                   ((header[7] & 0x7F) << 14) |
                   ((header[8] & 0x7F) << 7)  |
                    (header[9] & 0x7F);
     
     // Create a TagData structure
     TagData *data = create_tag_data();
     if (!data) 
     {
         display_error("Memory allocation failed.");
         fclose(fp);
         return NULL;
     }

     // For simplicity, store the version as read from the header.
     char verStr[10];
     snprintf(verStr, sizeof(verStr), "ID3v2.%d.%d", header[3], header[4]);
     data->version = strdup(verStr);
     
     // Iterate over frames within the tag size.
     long tagEnd = ftell(fp) + tagSize;
     while (ftell(fp) < tagEnd) 
     {
         char frameId[5] = {0};
         unsigned char sizeBytes[4];
         unsigned char flags[2];
         if (fread(frameId, 1, 4, fp) != 4) break;
         if (fread(sizeBytes, 1, 4, fp) != 4) break;
         if (fread(flags, 1, 2, fp) != 2) break;
         
         // If the frame ID is empty (all zeroes), we've reached the end.
         if (frameId[0] == 0)
             break;
         
         int frameSize = (sizeBytes[0] << 24) |
                         (sizeBytes[1] << 16) |
                         (sizeBytes[2] << 8)  |
                         (sizeBytes[3]);
         
         // Allocate a buffer for frame content
         char *content = (char *)malloc(frameSize + 1);
         if (!content) break;
         if (fread(content, 1, frameSize, fp) != (size_t)frameSize) {
             free(content);
             break;
         }
         content[frameSize] = '\0';
         
         // Based on the frame ID, assign content to the proper field.
         if (strcmp(frameId, "TIT2") == 0) 
         {
             free(data->title);
             data->title = strdup(content);
         } 
         else if (strcmp(frameId, "TPE1") == 0) 
         {
             free(data->artist);
             data->artist = strdup(content);
         } 
         else if (strcmp(frameId, "TALB") == 0) 
         {
             free(data->album);
             data->album = strdup(content);
         } 
         else if (strcmp(frameId, "TYER") == 0) 
         {
             free(data->year);
             data->year = strdup(content);
         } 
         else if (strcmp(frameId, "COMM") == 0) 
         {
             free(data->comment);
             data->comment = strdup(content);
         } 
         else if (strcmp(frameId, "TCON") == 0) 
         {
             free(data->genre);
             data->genre = strdup(content);
         }
         
         free(content);
     }
     
     fclose(fp);
     return data;
 }
 
 /**
  * @brief Displays the metadata contained in a TagData structure.
  *
  * @param data Pointer to the TagData structure.
  */
 void display_metadata(const TagData *data) 
 {
     if (!data) 
     {
         printf("No tag data available.\n");
         return;
     }
     printf("Version: %s\n", data->version ? data->version : "N/A");
     printf("Title:   %s\n", data->title   ? data->title   : "N/A");
     printf("Artist:  %s\n", data->artist  ? data->artist  : "N/A");
     printf("Album:   %s\n", data->album   ? data->album   : "N/A");
     printf("Year:    %s\n", data->year    ? data->year    : "N/A");
     printf("Comment: %s\n", data->comment ? data->comment : "N/A");
     printf("Genre:   %s\n", data->genre   ? data->genre   : "N/A");
 }
 
 /**
  * @brief Reads and displays the tags from the specified MP3 file.
  *
  * @param filename The MP3 file whose tags will be viewed.
  */
 void view_tags(const char *filename) 
 {
     TagData *data = read_id3_tags(filename);
     if (data) 
     {
         display_metadata(data);
         /* Free allocated memory */
         free(data->version);
         free(data->title);
         free(data->artist);
         free(data->album);
         free(data->year);
         free(data->comment);
         free(data->genre);
         free(data);
     }
 }
 