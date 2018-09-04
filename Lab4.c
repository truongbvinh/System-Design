// Vinh Truong 88812807 truongvb
// Paramdeep Atwal 91166595 patwal

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int heap[127];
int blocknum = 1;

int allocate(int size){
	int i = 0;
	for(; i < 127; i++){
		int blockSize = heap[i+1]>>1;

		if ((heap[i + 1]%2) == 0 && blockSize >= size + 2){
			if(blockSize > size + 4){
				// setting the next block's values
				heap[i + size + 3] = (blockSize - size) - 2;
				heap[i + size + 3] <<= 1;

				// setting this block's values
				heap[i] = blocknum;
				heap[i + 1] = size + 2;
				heap[i + 1] <<= 1;
				heap[i + 1] += 1;
				printf("%d\n", blocknum);
				return blocknum++;
			}else{
				// doesn't split if the split doesn't leave useful split
				heap[i] = blocknum;
				heap[i + 1] += 1;
				printf("%d\n", blocknum);
				return blocknum++;
			}
		}else{
			// printf("i = %d, block = %d\n", i, (heap[i]&0b11111110)>>1);
			i = i + (heap[i + 1]>> 1)-1;
		}
	}
	printf("allocate error!, i = %d\n", i);
	return -1;
}

void freeBlock(int block){
	int i = 0;
	for (; i < 127; i++){
		if (heap[i + 1]%2 == 1 && (heap[i]) == block){
			heap[i + 1] -= 1;
			return;
		}else{
			i = i + (heap[i + 1]>> 1)-1;
		}
	}
	printf("free error!\n");
}

void blocklist(){
	printf("Size Allocated Start          End\n");
	int i = 0;
	for (; i < 127; i++){
		printf("%-4d", (heap[i + 1])>>1);
		if (heap[i + 1]%2 == 1)
			printf("%-10s", "yes");
		else
			printf("%-10s", "no");
		printf("%-15d%d\n", i, i + (heap[i + 1]>>1) - 1);
		i = i + (heap[i + 1]>> 1)-1;
	}
}

void write(int blockNum, char x, int copies){
	int address = -1;
	int i = 0;
	for (; i < 127; i++){
		if ((heap[i]) == blockNum){
			address = i;
			break;
		}
	}
	if (address == -1 || ((heap[address + 1]>>1) - 2) < copies){
		printf("writeheap error!\n");
		return;
	}

	for(i = 0; i < copies; i++){
		heap[address + 2 + i] = x;
	}
}

void printheap(int block, int num){
	int i = 0;
	for(; i < 127; i++){
		if ((heap[i]) == block){
			int j = 0;
			int dataSize = (heap[i + 1])>>1;
			dataSize -= 2;
			for(; j < num && j < dataSize; j++){
				printf("%c", heap[i + j + 2]);
			}
			printf("\n");
			return;
		}
		i = i + (heap[i + 1]>> 1)-1;
	}
	printf("printheap error!\n");
}

void printheader(int block){
	int i = 0;
	for(; i < 127; i++){
		if((heap[i]) == block){
			printf("%.2x%.2x\n", heap[i],heap[i+1]);
			return;
		}
		i = i + (heap[i + 1]>> 1)-1;
	}
}

int processInput( char *userInput){

	char actualCmd[255];
	const char split[2] = " ";
	strcpy(actualCmd, strtok(userInput, split));

	if(strcmp(actualCmd, "allocate") == 0){
		int size = atoi(strtok(NULL, split));
		allocate(size);

	}else if(strcmp(actualCmd, "free") == 0){
		int block = atoi(strtok(NULL, split));
		freeBlock(block);

	}else if(strcmp(actualCmd, "blocklist\n") == 0){
		blocklist();

	}else if(strcmp(actualCmd, "writeheap") == 0){
		char x[1];
		int blockNum = atoi(strtok(NULL, split));
		strncpy(x, strtok(NULL, split), 1);
		int copies = atoi(strtok(NULL, split));
		write(blockNum, x[0], copies);

	}else if(strcmp(actualCmd, "printheap") == 0){
		int blockNum = atoi(strtok(NULL, split));
		int size = atoi(strtok(NULL, split));
		printheap(blockNum, size);

	}else if(strcmp(actualCmd, "printheader") == 0){
		int blockNum = atoi(strtok(NULL, split));
		printheader(blockNum);

	}else if(strcmp(actualCmd, "quit\n") == 0){
		return 1;
	}else{
		printf("Invalid command\n");
	}
	return 0;
}

int main(){
	heap[1] = 0b11111110;
	char userInput[100];
	// allocate(10);
	// blocklist();
	// allocate(40);
	// blocklist();
	// write(2, 'X', 20);
	// printheap(2, 10);
	printf(">");
	while(processInput(fgets(userInput, 100, stdin)) == 0){
		printf(">");
	}
	return 0;
}