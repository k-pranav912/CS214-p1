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

	if (S_ISDIR()data.st_mode) {return 1;}

	return 0;
}

int main() {

}
