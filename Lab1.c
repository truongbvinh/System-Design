// Vinh Truong 88812807
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct prereq{
	char val[10][19];
	int numRequired;
	int total;
};
struct class{
	char name[10];
	int quarter;
	struct prereq prereqs;
};

int indexOfValue(struct class classes[], char value[]){
	int i;
	for (i = 0; i < 20; i++){
		if (strcmp(classes[i].name, value) == 0)
			return i;
	}
	return -1;
}

char* trimWhitespace(char *line){
	char *end;
	end = line + strlen(line)-1;
	while (end > line && isspace((unsigned char)*end)) end--;
	*(end+1) =0;
	return line;
}

void printClasses(struct class classes[], int length){
	int i;
	for (i = 0; i < length; i++){
		printf("Class: %s - Quarter: %d - Number of prereqs: %x", classes[i].name, classes[i].quarter, classes[i].prereqs.numRequired);
		printf("- Prereqs: ");
		fflush(stdout);
		int j;
		for (j = 0; j < classes[i].prereqs.numRequired; j++){
			printf(" %s", classes[i].prereqs.val[j]);
			fflush(stdout);
		}
		printf("\n");
	}
}

void printPlan(struct class classes[], int length){
	int indexes[20];
	int taken = 0;
	int year = 1;
	int quart = 1;
	int skip = 0;
	int class = 0;
	while (&free){
		class = 0;
		if (quart == 4){
			year++;
			quart = 1;
		}
		if (taken == length || year > 5)
			break;

		int i;
		for (i = 0; i < length; i++){
			skip = 0;
			int j;
			for (j = 0; j < taken; j++)
				if (i == indexes[j])
					skip = 1;
			if (skip) continue;

			if (classes[i].prereqs.numRequired == 0 && classes[i].quarter == quart){
				if (class == 0){
					class = 1;
					printf("%d %d", year, quart);
				}
				printf(" %s", classes[i].name);
				indexes[taken++] = i;

				int j;
				for (j = 0; j < length; j++){
					int k;
					for (k = 0; k < classes[j].prereqs.total; k++){
						char pre[10];
						strcpy(pre, classes[j].prereqs.val[k]);
						// printf("-%d %d %d--%s %s-", j, k, i, pre, classes[i].name);
						if (strcmp(pre, classes[i].name) == 0)
							classes[j].prereqs.numRequired--;
					}
				}
			}
		}


		quart++;
		if (class == 1)
			printf("\n");
	}
}

int main(int argc, char *argv[]){

	struct class classes[20];
	char gradClasses[10][20];
	int numClasses = 0;

	FILE *file1 = fopen(argv[1], "r");
	FILE *file2 = fopen(argv[2], "r");
	FILE *file3 = fopen(argv[3], "r");
	
	char line[255];
	if (file1 == NULL || file2 == NULL || file3 == NULL){
		printf("Error: opening files");
	}else{
		const char split[2] = " ";

		int counter = 0;
		while (fgets(line, sizeof line, file1) != NULL){
			if (strcmp(line, "\n") == 0)
				continue;
			struct class temp;
			strcpy(temp.name, strtok(line, split));
			temp.quarter = atoi(strtok(NULL, split));
			temp.prereqs.numRequired = 0;
			temp.prereqs.total = 0;
			classes[counter++] = temp;
			numClasses++;
		}


		char *token;
		while (fgets(line, sizeof line, file2) != NULL){
			int index = indexOfValue(classes, strtok(line, split));
			if (index >= -1){
				struct prereq temp;
				temp.numRequired = 0;
				temp.total = 0;
				int counter = 0;
				while ((token = strtok(NULL, split)) != NULL){
					strcpy(temp.val[counter], trimWhitespace(token));
					temp.numRequired++;
					temp.total++;
					counter++;
				}
				classes[index].prereqs = temp;
			}
		}
		
		while (fgets(line, sizeof line, file3) != NULL){
			char *temp = strtok(line, split);
			strcpy(gradClasses[0], temp);
			int counter = 1;
			while ((temp) != NULL){
				strcpy(gradClasses[counter++], temp);
				temp = strtok(NULL, split);
			}
		}
		printPlan(classes, numClasses);

		fclose(file1);
		fclose(file2);
		fclose(file3);
	}
	return 0;
}