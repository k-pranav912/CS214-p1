#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

int isdir(char *name) {
	struct stat data;

	int err = stat(name, &data);

	if (err) {
		perror(name);
		return 0;
	}

	if (S_ISDIR(data.st_mode)) {return 1;}

	return 0;
}

int r_dir(char* path) {

	struct dirent *de;
	
	DIR *dr = opendir(path);

	if (dr == 0) {
		perror(path);
		return 0;
	}

	while ((de = readdir(dr)) != NULL) {
		printf("Name: %s\n", de->d_name);
		printf("Type: %d\n", de->d_type);
		//printf("  \n");

		if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) {
			//printf("waewgtfsdg\n");
			continue;
		}

		if (de->d_type == 4) {
			//printf("true\n");
			char* new_path = malloc(strlen(de->d_name) + strlen(path) + 2);
			new_path = strcpy(new_path, path);
			strcat(new_path, "/");
			strcat(new_path, de->d_name);
			printf("path name: %s\n", new_path);
		} else {
			//printf("false\n");
		}
	}

	closedir(dr);
	return 1;

}

int main() {
	printf("%d\n", isdir("test"));
	r_dir("test");
}
