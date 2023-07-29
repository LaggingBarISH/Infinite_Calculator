#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include<time.h>
#define FORCE_OUTPUT 0 //1 True - 0 False
#define _32BitMod10 6

struct number_segment_uint32_t {
	uint32_t number;
	struct number_segment_uint32_t* next_num;
};

void free_struct(struct number_segment_uint32_t *_struct){
	while(_struct != NULL){
		struct number_segment_uint32_t *temp = _struct->next_num;
		free(_struct);
		_struct = temp;
	}
}

void reverse_struct(struct number_segment_uint32_t **in){
    struct number_segment_uint32_t* temp = NULL, *current = *in, *next = NULL;
    while (current != NULL) {
        next = current->next_num;
        current->next_num = temp;
        temp = current;
        current = next;
    }
    *in = temp;
}

void multiply(struct number_segment_uint32_t* in, uint32_t multiplier){
	uint32_t remainder=0;
	while(1){	
		uint64_t calc = (uint64_t)(in->number) * multiplier + remainder;
		remainder = (uint32_t)(calc >> 32);
		in->number = (uint32_t)(calc & ((uint32_t) - 1));
		if(in->next_num == NULL){
			if(remainder != 0){
				in->next_num = malloc(sizeof(struct number_segment_uint32_t));
				in->next_num->next_num = NULL;
				in->next_num->number = remainder;
			}
			break;
		}
		in = in->next_num;
	}
	
}

void copy_struct(struct number_segment_uint32_t* out, struct number_segment_uint32_t* in){
	out->number = in->number;
	while (in->next_num!=NULL){
		out->next_num = malloc(sizeof(struct number_segment_uint32_t));
		in = in->next_num;
		out = out->next_num;
		out->number = in->number;
	}
	out->next_num = NULL;
}

char* struct_to_char_array(struct number_segment_uint32_t *_struct){
	struct number_segment_uint32_t *deep_copy = malloc(sizeof(struct number_segment_uint32_t)), *temp;
	uint32_t depth=1;
	copy_struct(deep_copy, _struct);
	temp = deep_copy;
	while(temp->next_num != NULL){
		depth++;
		temp = temp->next_num;
	}
	temp = deep_copy;
	//Slightly oversized array to fill the numbers with.
	char arr[depth * 10];
	memset(&arr[0], 0, sizeof(char) * depth * 10);


	uint32_t i = 0, j, remainder;
	//Loops the whole number to get all digits.
	while(1){
		remainder = 0;
		//Calculates 1 digit at a time.
		while(1){
			uint8_t mod = deep_copy->number % 10;
			deep_copy->number = (uint32_t)((deep_copy->number + ((uint64_t)remainder << 32)) / 10);
			remainder *= _32BitMod10;
			remainder += mod;
			remainder %= 10;
			if(deep_copy->next_num != NULL) {
				deep_copy = deep_copy->next_num;
			} else {
				break;
			}
		}
		//Add digit to array.
		arr[i] = '0' + remainder;
		i++;

		//Lessen calculations - exit.
		if(temp->number == 0){
			if(temp->next_num == NULL) break;
			temp = temp->next_num;
		}
		deep_copy = temp;
	}
	char* digits = malloc(sizeof(char) * (i + 1));
	
	//Reverse and place a null at the end.
	for(j = 0; j < i; j++){
		*(digits + j) = arr[i - j - 1];
	}
	*(digits + i) = '\0';

	free_struct(deep_copy);
	return digits;
}

int main(int argc, char const *argv[])
{
	clock_t timer_start, timer_end;
	char in_file_name[] = "input.bin";
	char out_file_name[] = "out.txt";
	uint32_t loop, i, j;
	uint8_t _stdout = 1; //Print calculations to stdout.
	uint8_t skip;


	FILE *input_file, *output_file;
	input_file = fopen(in_file_name, "rb");
	output_file = fopen(out_file_name, "wb");
	if (input_file == NULL) return 1;
	if (output_file == NULL) return 1;

	fread(&skip, 1, 1, input_file);
	//Skip the text.
	fseek(input_file, skip, SEEK_CUR);
	//Just for conveniance use loop lenght.
	fread(&loop, 1, 4, input_file);

	if(loop > 100 && !FORCE_OUTPUT){
		_stdout = 0;
		printf("%s\n", "Input calculations > 100 and FORCE_OUTPUT is false. Not printing calculations to not spam the console.");
	}
	timer_start = clock();
	for (i = 0; i < loop; i++){
		uint8_t large_num_len;
		fread(&large_num_len, 1, 1, input_file);
		struct number_segment_uint32_t* big_number = NULL;
		big_number = malloc(sizeof(struct number_segment_uint32_t));
		struct number_segment_uint32_t* big_number_temp_pointer = big_number;
		
		for (j = 0; j < large_num_len; ){
			uint8_t num_part1, num_part2, num_part3, num_part4;

			if (j==0){
				switch(large_num_len % 4){
					case 0:
						fread(&num_part4, 1, 1, input_file);
						fread(&num_part3, 1, 1, input_file);
						fread(&num_part2, 1, 1, input_file);
						fread(&num_part1, 1, 1, input_file);
						j+=4;
						break;
					case 1:
						fread(&num_part1, 1, 1, input_file);
						j+=1;
						break;
					case 2:
						fread(&num_part2, 1, 1, input_file);
						fread(&num_part1, 1, 1, input_file);
						j+=2;
						break;
					case 3:
						fread(&num_part3, 1, 1, input_file);
						fread(&num_part2, 1, 1, input_file);
						fread(&num_part1, 1, 1, input_file);
						j+=3;
						break;

				}
				big_number_temp_pointer->number = (uint32_t)(((uint32_t)num_part4 << 24) | ((uint32_t)num_part3 << 16) | ((uint32_t)num_part2 << 8) | ((uint32_t)num_part1));
				big_number_temp_pointer->next_num = NULL;
				continue;
			}
			big_number_temp_pointer->next_num = malloc(sizeof(struct number_segment_uint32_t));
			big_number_temp_pointer = big_number_temp_pointer->next_num;
			big_number_temp_pointer->next_num = NULL;
			
			fread(&num_part4, 1, 1, input_file);
			fread(&num_part3, 1, 1, input_file);
			fread(&num_part2, 1, 1, input_file);
			fread(&num_part1, 1, 1, input_file);
			big_number_temp_pointer->number = (uint32_t)(((uint32_t)num_part4 << 24) | ((uint32_t)num_part3 << 16) | ((uint32_t)num_part2 << 8) | ((uint32_t)num_part1));

			j+=4;
		}

			uint32_t multiplier;
			fread(&multiplier, 4, 1, input_file);
			char* big_number_result = struct_to_char_array(big_number);

			struct number_segment_uint32_t* result = malloc(sizeof(struct number_segment_uint32_t));

			copy_struct(result, big_number);

			reverse_struct(&result);
			multiply(result, multiplier);
			reverse_struct(&result);
			char* big_number_calc = struct_to_char_array(result);
			if(_stdout) printf("%s * %u = %s\n", big_number_result, multiplier, big_number_calc);
			fprintf(output_file, "%s * %u = %s\n", big_number_result, multiplier, big_number_calc);
			fflush(output_file);
			free(big_number_calc);
			free(big_number_result);
			free_struct(result);
			free_struct(big_number);


	}
	timer_end = clock();
	printf("Made %u calculations.\n", loop);
	printf("That took %f seconds.\n", (timer_end - timer_start)/1000.0);
	fflush(output_file);
	fflush(input_file);
	fclose(output_file);
	fclose(input_file);
	return 0;
}
