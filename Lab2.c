// Vinh Truong 88812807 truongvb
// Paramdeep Atwal 91166595 patwal
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int search(int array[], int first, int last, int value){
	if (first == last){
		printf("pid %d, value: %d\n", getpid(), array[first]);
		if (array[first] == value){
			return first;
		}
		else{
			return -1;
		}
	}
	pid_t pid = fork();
	if (pid == 0){
		return search(array, first, (first + last)/2, value);
	}else{
		wait(NULL);
		return search(array, (first + last)/2 + 1, last, value);
	}
}


int main(int argc, char *argv[]){
	// initializing the array of elements to search through
	int arr[10];
	char line[255];
	char *token;
	const char split[2] = " ";
	int length = 0;
	FILE *file1 = fopen(argv[1], "r");
	fgets(line, sizeof line, file1);
	token = strtok (line, split);
	arr[length++] = atoi(token);
	while ((token = strtok(NULL, split)) != NULL){
		arr[length++] = atoi(token);
	}

	// done initializing the array
	// creating a file for interprocess communication

	FILE *fp = fopen("info.txt", "w");
	fprintf(fp, "-1");
	fclose(fp);

	// done creating file for communication
	// setting up recursive process creation for searching

	int x;
	int pid_ad;
	int parent = getpid();
	pid_t pid = fork();

	// child process will search, parent will take care of post search information display
	if (pid == 0){
		x = search(arr, 0, length-1, atoi(argv[2]));
	}
	else{
		wait(NULL);
		pid_ad = getpid();
	}
	if (getpid() != pid_ad){
		if (x > -1){
			// if there's an index greater than -1, then replace with new value
			FILE *fp = fopen("info.txt", "w");
			fprintf(fp, "%d", x);
		}

		exit(0);
	}

	// print out the value stored in the file, should be the index of the 

	FILE *output = fopen("info.txt", "r");
	fgets(line, sizeof line, output);
	printf("Search output: %s\n", line);
	fclose(output);
}