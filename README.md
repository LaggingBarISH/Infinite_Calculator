<br />
1) Description

We are given an input file containing a number with an arbitrary bit length and a 32 bit multiplier. The program reads these two numbers from the file and multiplies them. After multiplying the results are saved to an output file.

<br />
<br />
2) Input File

The input file is not encoded in ascii chars to preserve space and keep the file sizes low.

File Format:
<pre>
Uint8 skip | char* text | data

Where skip is  the length of the ascii text,
And data is:

Uint32 loop | struct number_segment*

Loop is the number of elements number_segment holds,

And number_segment is in the form of:

Uint8 length | byte data* | Uint32 multiplier

Where length is the length of data.
</pre>

<br />
<br />
3) Output File

The output file is in ascii characters to increase readability.
File Format:
	Number | * | Multiplier | = | Calculation | \n
 
<br />
<br />
4) Reading File

Reading the file is done similarly to a stream to keep memory usage low.
First skip is read then file pointer is increased by skip.
Then loop is read and the program enters a for loop.
In the for loop both the big number and the multiplier are read to memory and then multiplied.
After multiplication the numbers are saved as base10 instead of binary to keep them readable.

<br />
<br />
5) Writing Output

Within the for loop the base10 numbers are written to out.txt.

<br />
<br />
6) Calculations

Since there isn’t a function to process this proprietary data format the program needs to have its own methods to handle the data.

> struct number_segment_uint32_t 

Holds the data.
<br />
<br />
> void free_struct(struct number_segment_uint32_t *_struct)

Frees a struct and all of its child elements by recursively 	going down in the struct.
<br />
<br />
> void reverse_struct(struct number_segment_uint32_t **in)

Reverses the order of elements in a struct.
Needed for the multiplication.
<br />
<br />
> void multiply(struct number_segment_uint32_t* in, uint32_t multiplier)

Multiplies data in a given struct by the given multiplier.
If there is extra carry that needs to be added appends the carry as a node to the struct.
<br />
<br />
> void copy_struct(struct number_segment_uint32_t* out, struct number_segment_uint32_t* in)

Copies the data of struct in to struct out.
Used when we need a copy of the data in a struct.
<br />
<br />
> char* struct_to_char_array(struct number_segment_uint32_t *_struct)

Converts data in the given struct to a base10 representation and returns it in a char array.
<br />
<br />
> char* struct_to_char_array(struct number_segment_uint32_t *_struct)

Main program loop where the files are read, calculating methods are accordingly called, and finally written to a file.
<br />
<br />
<br />
<pre>
> gcc -O3 Generate_Input_File.c -o Generate_Input_File.out

> time ./Generate_Input_File.out
	Generated 1000000 numbers.
	That took 0.225235 seconds.

	real	0m0.229s
	user	0m0.217s
	sys	0m0.012s


> gcc -O3 Crunch_Numbers.c -o Crunch_Numbers.out

> time ./Crunch_Numbers.out
	Input calculations > 100 and FORCE_OUTPUT is false. Not printing calculations to not spam the console.
	Made 1000000 calculations.
	That took 3.780444 seconds.
	
	real	0m3.803s
	user	0m2.240s
	sys	0m1.552s
 </pre>

<br />
<br />
 SYSTEM INFO:

 <br />
 <br />
 <pre>
OS: Debian GNU/Linux 10 (buster) x86_64 
Kernel: 4.19.0-23-amd64 
Uptime: 1 day, 21 hours, 32 mins 
Shell: bash 5.0.3 
CPU: Intel i5-6500 (4) @ 3.600GHz 
GPU: NVIDIA GeForce GTX 980 Ti 
Memory: 4831MiB / 15990MiB 
 </pre>
