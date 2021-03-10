#include <sys/fcntl.h>
#include <stdlib.h>
#include <stdio.h>
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

int is_dir(char *name) {
	struct stat data;

	int err = stat(name, &data);

	if (err) {
		perror(name);
		return 0;
	}

	if (S_ISDIR(data.st_mode)) {return 1;}

	return 0;
}

// hacky/testing/trying out methods and stuff for main driver function
int r_dir(char* path) {

	struct dirent *de;
	
	DIR *dr = opendir(path);

	if (dr == 0) {
		perror(path);
		return 0;
	}

	while ((de = readdir(dr)) != NULL) {
		printf("Name: %s\n", de->d_name);
		printf("Name length: %ld\n", strlen(de->d_name));
		printf("Type: %d\n", de->d_type);
		//printf("  \n");

		if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) {
			//printf("waewgtfsdg\n");
			continue;
		}

		if (strncmp(de->d_name, "wrap.", 5) == 0) {
			continue;
		}

		char* new_path = malloc(strlen(de->d_name) + strlen(path) + 2);
		new_path = strcpy(new_path, path);
		strcat(new_path, "/");
		strcat(new_path, de->d_name);
		printf("path name: %s\n", new_path);
		printf("Is Directory: %d\n", is_dir(new_path));

		if (!is_dir(new_path)) {
			int x = open(new_path, O_WRONLY|O_TRUNC);
			printf("File Descriptor: %d\n", x);
			close(x);

			char *new_file = malloc(strlen(new_path) + 6);
			strcpy(new_file, path);
			strcat(new_file, "/");
			strcat(new_file, "wrap.");
			strcat(new_file, de->d_name);

			int y = open(new_file, O_WRONLY|O_TRUNC|O_CREAT, 0777);
			write(y, "aa\n", 3);
			close(y);
		}

		printf("--\n");
		free(new_path);
		
	}

	closedir(dr);
	return 1;

}

/*driver function, receives path name of directory as input
 * ignores . and .. directories
 * ignores files with prefix "wrap."
 * goes through each file, creates "wrap." file and calls foo fuction.
 * foo function is stand-in for wrap function.
 * foo has following inputs:
 * 1. file descriptor of original file that is to be read; inp_fd
 * 2. file descriptor of new file, that is to be written to; out_fd
 * 3. page width;
 * if directory has sub-directories, recursively call driver on the subdirectory.
 */
int driver(char *path, int page_width) {
	struct dirent *de;

	DIR *dr = opendir(path);

	if (dr == 0) {
		perror("Error opening directory.\n");
		return 0;
	}

	while ((de = readdir(dr)) != NULL) {
	
		
		if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) {
			continue;
		}

		char* new_path = malloc(strlen(de->d_name) + strlen(path) + 2);
		strcpy(new_path, path);
		strcat(new_path, "/");
		strcat(new_path, de->d_name);

		if (is_dir(new_path)) {
			driver(new_path, page_width);
		} else {

			
			if (strncmp(de->d_name, "wrap.", 5) == 0) {
				continue;
			}

			char *new_file = malloc(strlen(new_path) + 6);
			strcpy(new_file, path);
			strcat(new_file, "/");
			strcat(new_file, "wrap.");
			strcat(new_file, de->d_name);

			int in_fd = open(new_path, O_RDONLY);
			int out_fd = open(new_file, O_WRONLY|O_TRUNC|O_CREAT, 0777);

			if (in_fd == -1 || out_fd == -1) {
				perror("Could not create or open file.\n");
				return 0;
			}

			wrap(in_fd, out_fd, page_width);
			
			//testing
			//write(out_fd, "aa\nb", 4);


			free(new_file);
			close(in_fd);
			close(out_fd);
			
		}

		free(new_path);
	}

	//free(de);
	closedir(dr);

	//free(dr);

	return 1;
}


int main(int argc, char **argv) {
	//printf("%d\n", is_dir("test"));
	//r_dir("test");
	//driver("test");
	/*
	if (argc == 3) {
		printf("%s\n", argv[2]);
	}
	*/

	int page_width = 0;

	if (argc < 2) {
		perror("Not enough arguments.\n");
		return 0;
	}

	page_width = atoi(argv[1]);

	printf("page_width: %d\n", page_width);

	
	if (argc == 2) {
		wrap(0, 1, page_width);
	} else {
		if (is_dir(argv[2])) {
			driver(argv[2], page_width);
		} else {
			int inp_fd = open(argv[2], O_RDONLY);

			if (inp_fd == -1) {
				perror("Error opening file.\n");
				return 0;
			}

			wrap(inp_fd, 1, page_width);

			close(inp_fd);
		}
	}
	

	return 1;
}
