Name: Surya Mantha
NetID: stm144

Name: Saipranav Kalapala
NetID: sk2048

Testing Process:

For part 1, a very small phrase was initially tested, such as "The dog walked to the store". This
was tested with multiple different widths. Then, spaces, tabs, and new lines were added to that 
statement randomly to further test the different results at different widths. This statement was also
used to test different kinds of buffers. After that, a smaller statement was used, such as
"testtesttest testtest". This was tested with a width of 10 to see how words that exceeded the width
would work. Spaces, tabs, and newlines with similar statements were tested too. From there, larger
and larger paragraphs were tested, such as randomly typed paragraphs or song lyrics with added new lines
and spaces to see how the wrapping would work.

For part 2, the wrap function was tested using a singular file first, to see whether it was outputting
to standard output, then a directory with a singular file in it, and then a directory with multiple
subdirectories and files. Number of command-line input arguments were also tested. Files starting with "."
and "wrap." were tested to ensure that they were being ignored. Made sure all files and directories that
were opened, had been closed prior to terminating the program. 
