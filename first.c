#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

#define BUFFER 128

int wrap(int in_fd, int out_fd, int width)
{
	int file_in = in_fd;
	int file_out = out_fd;
	int space_toggle = 0;
	int new_line_toggle = 0;
	int first_toggle = 0;
	int length_count = 0;
	int read_val = 1;
	char temp_string[BUFFER + 2];
	int temp_string_count = 0;
	int ret_val = 1;
	while(read_val != 0){
		char read_string[BUFFER + 1];
		read_val = read(file_in, read_string, BUFFER); 
		read_string[BUFFER] = '\0';
		for(int i = 0; i < read_val; i++){
			if(isspace(read_string[i]) != 0){
				if(space_toggle == 0){
					if(length_count > width && new_line_toggle != 2 && first_toggle != 0){
						write(file_out, "\n", 1);
						length_count = temp_string_count;
					}
					if(temp_string_count > width){ret_val = 0;}
					for(int i = 0; i < temp_string_count; i++){
						write(file_out, &temp_string[i], 1);
						first_toggle = 1;
					}
					write(file_out, " ", 1);
					temp_string_count = 0;
					space_toggle = 1;
					length_count++;
				}
				if(new_line_toggle == 1){
					if(read_string[i] == '\n'){
						write(file_out, "\n\n", 2);
						new_line_toggle = 2;
						length_count = temp_string_count; 
					}
				}
				else{new_line_toggle = 1;}
			}
			else{
				space_toggle = 0;
				new_line_toggle = 0;
				temp_string[temp_string_count] = read_string[i];
				temp_string_count++;
				length_count++;
			}
		}	
	}
	if(length_count - temp_string_count >= width){write(file_out, "\n", 1);}
	write(file_out, &temp_string, temp_string_count);
	return ret_val;
}
int main(int argc, char* argv[])
{
	int in_fd = open("in.txt", O_RDONLY);
	int out_fd = open("out.txt", O_WRONLY|O_TRUNC|O_CREAT, 0777);
	int val = wrap(in_fd, out_fd, 30);
	printf("%d\n", val);
	
}	
