# MP3 Tag Editor

## Description
MP3 Tag Editor is a command-line tool for reading, writing, and editing ID3v2 tags in MP3 files. It allows users to view metadata, modify tag fields, and write new tags to MP3 files.

## Features
- View MP3 metadata (Title, Artist, Album, Year, Comment, Genre)
- Edit specific tags
- Write new dummy tags
- Error handling for invalid files

# Compile the source code
gcc main.c id3_reader.c id3_writer.c id3_utils.c error_handling.c -o mp3tagreader
```

## Usage
```sh
# View help
./mp3tagreader -h

# View MP3 tags
./mp3tagreader -v filename.mp3

# Write dummy tags
./mp3tagreader -w filename.mp3

# Edit a specific tag (e.g., Title)
./mp3tagreader -e title filename.mp3 "New Title"
```

## File Structure
```
MP3-Tag-Editor/
│── main.c             # Main entry point
│── id3_reader.c       # Functions for reading ID3 tags
│── id3_writer.c       # Functions for writing/editing ID3 tags
│── id3_utils.c        # Utility functions
│── error_handling.c   # Error handling functions
│── id3_reader.h       # Header file for ID3 reading
│── id3_writer.h       # Header file for ID3 writing
│── id3_utils.h        # Header file for utilities
│── error_handling.h   # Header file for error handling
│── Makefile           # Compilation automation (optional)
│── README.md          # Project documentation
```

## License
This project is licensed under the MIT License.
