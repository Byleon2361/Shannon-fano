Shannon-Fano

To compile the program you need to enter:
$make

Compression is carried out with an argument
command line -c (compress), and decompress with the -d (decompress) argument.
The -o option specifies the output file name.

Example:
Compression:
$./bin/Shannon -c -o compressedFile.sfc 10ThousandWords.txt
  where compressedFile.sfc is the file where the compressed text will be written, 10ThousandWords.txt is the source text
Decompress:
$./bin/Shannon -d -o res.txt compressedFile.sfc
  where compressedFile.sfc is the file to be decompress, res.txt is the file in which the result will be written.