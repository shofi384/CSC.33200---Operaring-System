#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main (int argc, char* argv[]) {
	char* filepath = argv[1];
	int returnval;
	
	if(argc != 2) {
		printf("\n Usage: please provide the file to open\n");
		return 1;
	}

	// Check file existence
	returnval = access(filepath, F_OK);
	if (returnval == 0)
		printf ("\n %s exists\n\n", filepath);
	else {
		if (errno == ENOENT)
			printf ("%s does not exist \n%s\n", filepath, strerror(errno));
		else if (errno == EACCES)
			printf ("%s is not accessible\n", filepath);
		return 0;
	}

	// Check read access ...
	returnval = access(filepath, R_OK);
	if (returnval == 0)
		printf (" %s readable\n\n", filepath);
	else perror("Error: ");
	
	// Check write access ...
	returnval = access(filepath, W_OK);
	if(returnval == 0)
		printf(" %s writable\n\n", filepath);
	else printf("\nError: %s", strerror(errno));

	return 0;
}

