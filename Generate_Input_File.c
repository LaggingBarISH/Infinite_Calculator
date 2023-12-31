#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include<time.h>
//#include <inttypes.h>

int main(int argc, char const *argv[])
{
	uint32_t i = 0, j = 0;
	srand(time(NULL));
	clock_t timer_start, timer_end;

	char filename[] = "input.bin";
	char desc[] = "\nDo not edit this file!!\no150120002 Baris Sedefoglu\nRest of the data in this file is going to be in binary format and byte order is Little Endian!!!\n";
	uint8_t skip = strlen(desc);
	
	//If set to -1 file size goes upto gigabytes. I didnt even wait for it to finish.
	uint32_t loop = 1000000;
	//Limited large number at 256 bits to not get absolutely massive input file sizes.
	uint8_t large_number_max_bit_lenght = -1;
	
	uint32_t multiplicand = 0;
	uint32_t multiplicand_max_value = -1;

	FILE* file;
	file=fopen(filename, "wb");
	if(file==NULL){
		printf("%s\n", "Yeah that happens.");
		return 1;
	}
	
	fwrite(&skip, 1, 1, file);
	fwrite(desc, 1, skip, file);
	fwrite(&loop, 1, 4, file);
	timer_start = clock();
	for(i = 0; i < loop ; i++){
		uint8_t large_num_len = rand() % ((uint32_t)large_number_max_bit_lenght + 1);
		uint8_t large_num[(large_num_len-1)/8+1];
		large_num[0] = rand() % (1 << large_num_len%8);
		for (j = 1; j < (large_num_len-1)/8+1; j++){
			large_num[j]= (uint8_t)(rand()>>7);
		}
		//multiplicand = (uint16_t)rand() % multiplicand_max_value;
		//Generate 32 bit random number for multiplier.
		multiplicand = (((((uint32_t)rand() << 24) | (((uint32_t)rand() >> 7) << 16) | (((uint32_t)rand() >> 7) << 8) | (((uint32_t)rand() >> 7))) - 1) % multiplicand_max_value) + 1;

		large_num_len=((large_num_len - 1) / 8 + 1);
		fwrite(&large_num_len, 1, 1, file);
		fwrite(&large_num, 1, large_num_len, file);
		fwrite(&multiplicand, 1, 4, file);
		//Show numbers on console
		/*
		for (j = 0; j < large_num_len; j++){
			printf("%02X", large_num[j]);
		}
		printf(" - %04X\n", multiplicand);*/
	}
	fflush(file);
	fclose(file);
	timer_end = clock();
	printf("Generated %u numbers.\n", loop);
	printf("That took %f seconds.\n", (timer_end - timer_start)/1000.0);
	getch();
	return 0;
}
