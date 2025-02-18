/**
 * @file main.c
 * @brief Main entry point for the MP3 Tag Reader application.
 *
 * This application allows users to view, write, and edit ID3 tags in MP3 files.
 */

 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include "main.h"
 #include "id3_reader.h"
 #include "id3_writer.h"
 #include "error_handling.h"
 
 /**
  * @brief Displays the help message for the MP3 Tag Reader application.
  *
  * This function prints usage instructions and available options.
  */
 void display_help() 
 {
     printf("Usage: mp3tagreader [options] filename\n");
     printf("Options:\n");
     printf("  -h               Display help\n");
     printf("  -v <filename>    View tags in an MP3 file\n");
     printf("  -w <filename>    Write dummy tags to an MP3 file\n");
     printf("  -e <tag> <filename> <value>  Edit a specific tag in an MP3 file\n");
 }
 
 /**
  * @brief Main function for the MP3 Tag Reader application.
  *
  * This function handles command-line arguments and executes the corresponding
  * operations for viewing, writing, or editing MP3 tags.
  *
  * @param argc Argument count.
  * @param argv Argument vector.
  * @return 0 on success, 1 on failure.
  */
 int main(int argc, char *argv[]) 
 {
     // Check if there are enough arguments
     if (argc < 2) 
     {
         display_help();
         return 1;
     }
     
     // Handle different command-line options
     if (strcmp(argv[1], "-h") == 0) 
     {
         // Display help message
         display_help();
     } 
     else if (strcmp(argv[1], "-v") == 0 && argc == 3) 
     {
         // View MP3 tags
         view_tags(argv[2]);
     } 
     else if (strcmp(argv[1], "-w") == 0 && argc == 3) 
     {
         // Write dummy tags to the file
         TagData *data = create_tag_data();
         if (!data) 
         {
             display_error("Memory allocation failed.");
             return 1;
         }
         
         // Assign dummy tag values
         data->version = strdup("ID3v2.3");
         data->title   = strdup("dummy title");
         data->artist  = strdup("dummy artist");
         data->album   = strdup("dummy album");
         data->year    = strdup("dummy year");
         data->comment = strdup("dummy comment");
         data->genre   = strdup("dummy genre");
         
         // Write tags to the MP3 file
         if (write_id3_tags(argv[2], data) == 0) 
         {
             printf("Tags written successfully.\n");
         } 
         else 
         {
             display_error("Failed to write tags.");
         }
         
         // Free allocated memory
         free(data->version);
         free(data->title);
         free(data->artist);
         free(data->album);
         free(data->year);
         free(data->comment);
         free(data->genre);
         free(data);
     } 
     else if (strcmp(argv[1], "-e") == 0 && argc == 5) 
     {
         // Edit a specific tag in the MP3 file
         if (edit_tag(argv[3], argv[2], argv[4]) == 0) 
         {
             printf("Tag edited successfully.\n");
         } 
         else 
         {
             display_error("Failed to edit tag.");
         }
     } 
     else 
     {
         // Display help message for incorrect usage
         display_help();
     }
     
     return 0;
 }
 