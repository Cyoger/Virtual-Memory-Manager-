#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 256
#define SIZE 65536 


// int pagetable[BUF_SIZE]; //pagetable to keep track of the physical addresses of the pages in the main memory
// signed char mainMemory[SIZE]; //main memory to keep track of the contents of the pages in the main memory


int main(int argc, char *argv[])
{
	int pagetable[BUF_SIZE]; //pagetable to keep track of the physical addresses of the pages in the main memory
	signed char mainMemory[SIZE]; //main memory to keep track of the contents of the pages in the main memory

	if (argc != 2)	
	{
		printf("Please enter 2 args: ./a.out(output) and ./addresses.txt(file with physical addresses)\n");
		exit(0);
	}
	char *input = argv[1]; //get the input file name for command line arguments
	char *output = "output.txt"; 
	FILE *IN = fopen(input, "r");
	FILE *OUT = fopen(output, "w");
	char buf[BUF_SIZE];
	
	for (int i = 0; i < 256; i++)
	{
		pagetable[i] = 0; //set the pagetable to 0
	}

	FILE* BS = fopen("BACKING_STORE.bin", "r");
	unsigned char freePage = 0; //variable to keep track of the next free page in the main memory
	while (!feof(IN)) //read the file with logical addresses
	{
		fgets(buf, BUF_SIZE, IN);
		int logicalAddress = atoi(buf); //convert the string to int
		int offset = logicalAddress & 255; //get the offset
		int page = (logicalAddress >> 8) & 255; //get the page number by shifting 8 bits to the right and masking with 255 to get least significant 8 bits
		int frame = pagetable[page]; //get the frame number from the pagetable 
		
		if (frame == 0) //when page fault occurs 
		{
			frame = freePage; //set the physical address to the next free page in the main memory
			freePage++; //free the next page in the main memory 

			//printf("Page number: page %d\n", page); //for debugging purposes
			//printf("frame: %d\n", frame); //for debugging purposes
			//printf("freePage: %d\n", freePage); //for debugging purposes
			//printf("offset: %d\n", offset); //for debugging purposes

			fseek(BS, page * 256, SEEK_SET); //set the file pointer to the page number in the backing store file
			fread(mainMemory + frame * 256, 1, 256, BS); //read the contents of the page from the backing store file to the main memory
			
			pagetable[page] = frame; //read from backing store file and store it in the pagetable	
		}
		int physicalAddress = (frame * 256) + offset; //the formula for physical address is frame * 256 + offset from the textbook 
		signed char byte_value = mainMemory[physicalAddress]; //get the value from the main memory by getting the index at which the value is stored in the main memory
		fprintf(OUT,"Logical address: %d   physical address: %d   Value: %d\n", logicalAddress, physicalAddress, byte_value); //print the output to the output file
		//printf("Logical address: %d   physical address: %d   Value: %d\n", logicalAddress, physicalAddress, byte_value); //print the output to the screen
		
		 
	}
	fclose(IN);
	fclose(OUT);
	fclose(BS);

	printf("done\n");
	printf("output file is output.txt\n");
	

	return 0;
}
