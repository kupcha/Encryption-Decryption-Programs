/*
@author Patrick Kupcha (netid kupcha)
Rutgers CS419, Section 02, Fall 2020
vdecrypt.c

Stream Cipher
*/

#include <stdio.h>
#include <stdlib.h>


/*
This function takes a string password and converts it into a number to be used as a seed for our number generator
*/
unsigned long getSeed(unsigned char *str) {
	unsigned long hash = 0;
	int c;
	while ((c = *str++)){
      hash = c + (hash << 6) + (hash << 16) - hash;
   }
	return hash;
}

/*
linear congruential generator
Input value of n, returns value of (n + 1) in sequence of pseudo-randomly generated numbers
*/
unsigned long numberGen(unsigned long seed){
   int m = 256; //1 byte
   unsigned long a = 1103515245; //multiplier
   unsigned long c = 12345; //increment
   return ((a * seed) + c) % m;
}


int main(int argc, char** argv){
   //input for this program should be : ./scrypt password plaintextfile ciphertextfile
   if (argc != 4){
      printf("ERROR: incorrect input\n");
      return 0;
   }
   unsigned char* password = (unsigned char*) argv[1];
   unsigned long seed = getSeed(password);
   printf("using seed=%lu from password=%s\n", seed, password);
   FILE *input;
   FILE *output;
   input = fopen(argv[2], "r");
   if (input == NULL){
      printf("ERROR in opening input file.\n");
      return 0;
   }
   output = fopen(argv[3], "w");
   if (output == NULL){
      printf("ERROR opening output file.\n");
      return 0;
   }
   unsigned char buffer[1];
   unsigned char keyByte = (unsigned char) numberGen(seed);
   while(fread(buffer, sizeof(buffer), 1, input)){
      unsigned char currCipher = buffer[0] ^ keyByte;
      fputc(currCipher, output);
      keyByte = (unsigned char) numberGen(keyByte);
   }
   fclose(input);
   fclose(output);
}
