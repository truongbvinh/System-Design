// Vinh Truong 88812807 truongvb
// Paramdeep Atwal 91166595 patwal
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


typedef struct {

	unsigned char vpagenum;
	unsigned char valid; // valid = 1 indicates page in memory
	unsigned char dirty; // dirty = 1 indicates page has been written while in memory
	unsigned char pn;    // pn    = number of the page where this virtual page can be found 
} PageTableEntry;

static int mainMem[8] = { -1, -1, -1, -1, -1, -1, -1, -1};
static int diskMem[16] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
static int lastUsed[4] = {-1, -1, -1, -1};

static PageTableEntry pgTable[8] = {
	{ 0, 0, 0, 0 },
	{ 1, 0, 0, 1 },
	{ 2, 0, 0, 2 },
	{ 3, 0, 0, 3 },
	{ 4, 0, 0, 4 },
	{ 5, 0, 0, 5 },
	{ 6, 0, 0, 6 },
	{ 7, 0, 0, 7 },
};

void showptable();
void showmain(int page);
void showdisk(int page);
void read(int address);
void write(int address, int data);

void printUsed(){
	int i = 0;
	for (; i < 4; i++){
		printf("%d, ", lastUsed[i]);
	}
	printf("\n");
}

void trackUsed(int memIndex){
	int in = -1;
	int i = 0;
	for (; i < 4; i++){
		if (lastUsed[i] == memIndex)
			in = i;
	}
	if (in == -1){
		i = 3;
		for (; i > 0; i--){
			lastUsed[i] = lastUsed[i-1];
		}
		lastUsed[0] = memIndex;
	}else{
		for (; in > 0; in--){
			lastUsed[in] = lastUsed[in-1];
		}
		lastUsed[0] = memIndex;
	}
}

void pgFault(int address){

	int vpagenum = address/2;
	int used[4] = {0, 0, 0, 0};
	int i = 0;
	for (; i < 8; i++){
		if (pgTable[i].valid == 1)
			used[pgTable[i].pn] = 1;
	}
	i = 0;
	for (; i < 4; i++){
		if (used[i] == 0){
			trackUsed(i);
			mainMem[i*2] = diskMem[pgTable[vpagenum].pn*2];
			mainMem[i*2 + 1] = diskMem[pgTable[vpagenum].pn*2 + 1];
			pgTable[vpagenum].valid = 1;
			pgTable[vpagenum].pn = i;

			return;
		}
	}
	// if we get here this means there was no available spaces in mainMem

	int pnNumber = lastUsed[3];
	i = 0;
	int entryNumber;
	for (; i < 8; i++){
		if (pgTable[i].valid == 1 && pgTable[i].pn == pnNumber){
			trackUsed(pnNumber);
			diskMem[i*2] = mainMem[pnNumber*2];
			diskMem[i*2 + 1] = mainMem[pnNumber*2 + 1];
			pgTable[i].pn = i;
			pgTable[i].valid = 0;
			pgTable[i].dirty = 0;
			mainMem[pnNumber*2] = diskMem[pgTable[vpagenum].pn*2];
			mainMem[pnNumber*2 + 1] = diskMem[pgTable[vpagenum].pn*2 + 1];
			pgTable[vpagenum].valid = 1;
			pgTable[vpagenum].dirty = 0;
			pgTable[vpagenum].pn = pnNumber;

			return;
		}
	}
}

void showptable()
{
	printf("VPageNum          Valid		Dirty		PN\n");
	int pages = 0;;
	for(; pages < 8; pages++)
	{
		printf("%d                 %d		%d		%d\n", pgTable[pages].vpagenum, pgTable[pages].valid, pgTable[pages].dirty, pgTable[pages].pn);
	}
}

void showmain(int page)
{
	if(page > 3) // cause it starts from 0 but do i even need this if i assume that input is correct?
	{
		return;
	}

	int add1 = page * 2; 
	int add2 = page * 2 + 1;
	
	
	printf("Address     Contents\n");
	printf("%d          %d\n", add1, mainMem[add1]);
	printf("%d          %d\n", add2, mainMem[add2]);
}

void showdisk(int page){


	int add1 = page * 2;
	int add2 = page * 2 + 1;

	printf("Address     Contents\n");
	printf("%d          %d\n", add1, diskMem[add1]);
	printf("%d          %d\n", add2, diskMem[add2]);

}

void read(int address){


	int vpagenum = address/2;

	if(pgTable[vpagenum].valid != 1 ){
		fflush(stdout);
		pgFault(address);

	}
	trackUsed(pgTable[vpagenum].pn);
	int pageNum = pgTable[vpagenum].pn;
	int offset = address % 2;
	int paddress = pageNum*2 + offset;
	printf("Address: %d  Value: %d \n", address, mainMem[paddress]);

	
}



void write(int address, int data){

	int vpagenum = address/2;

	if (pgTable[vpagenum].valid != 1){
		pgFault(address);

	}
	trackUsed(pgTable[vpagenum].pn);
	fflush(stdout);
	int pageNum = pgTable[vpagenum].pn;
	int offset = address % 2;
	int paddress = pageNum*2 + offset;
	mainMem[paddress] = data;
	pgTable[vpagenum].dirty = 1;

}

int processInput( char *userInput){

	char actualCmd[255];
	const char split[2] = " ";
	strcpy(actualCmd, strtok(userInput, split));

	if(strcmp(actualCmd, "read") == 0){
		int addr = atoi(strtok(NULL, split));
		read(addr);

	}else if(strcmp(actualCmd, "write") == 0){

		int addr = atoi(strtok(NULL, split));
		int val = atoi(strtok(NULL, split));
		write(addr, val);

	}else if(strcmp(actualCmd, "showmain") == 0){
		int page = atoi(strtok(NULL, split));
		showmain(page);

	}else if(strcmp(actualCmd, "showdisk") == 0){
		int page = atoi(strtok(NULL, split));
		showdisk(page);

	}else if(strcmp(actualCmd, "showptable\n") == 0){
		showptable();

	}else if(strcmp(actualCmd, "quit\n") == 0){
		return 1;
	}
	return 0;
}

int main()
{
	char userInput[100];
	printf("$ ");
	while(processInput(fgets(userInput, 100, stdin)) == 0){
		printf("$ ");
	}
	return 0;
}
