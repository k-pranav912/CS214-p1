#include <sys/fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

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
//TODO: add another parameter; page width to driver
int driver(char *path, int page_width) {
	struct dirent *de;

	DIR *dr = opendir(path);

	if (dr == 0) {
		perror(path);
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
			//foo(in_fd, out_fd, page_width);
			
			//testing
			write(out_fd, "aa\nb", 4);

			close(in_fd);
			close(out_fd);
		}
	}
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

	//printf("page_width: %d\n", page_width);

	/*
	if (argc == 2) {
		//foo(0, 1, page_width);
	} else {
		if (is_dir(argv[2])) {
			driver(argv[2], page_width);
		} else {
			int inp_fd = open(argv[2], O_RDONLY);

			if (inp_fd == -1) {
				perror("Error opening file.\n");
				return 0;
			}

			//foo(inp_fd, 1, page_width);

			close(inp_fd);
		}
	}
	*/

	return 1;
}
