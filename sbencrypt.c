/*
@author Patrick Kupcha (netid kupcha)
Rutgers CS419, Section 02, Fall 2020
sbencrypt.c

Block encryption with cipher block chaining and padding - encryption
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

int main (int argc, char** argv){
   //proper input: sbencrypt password plaintextfile ciphertextfile
   if (argc != 4){
      printf("ERROR: improper input.\n");
   }
   FILE *plaintext;
   plaintext = fopen(argv[2], "r+");
   if (plaintext == NULL){
      printf("ERROR opening plaintext file\n");
      return 0;
   }
	FILE *ciphertext;
	ciphertext = fopen(argv[3], "w");
	if (ciphertext == NULL){
		printf("ERROR opening ciphertext file\n");
		return 0;
	}
	// this block of text adds padding to the end of the plaintext file
   fseek(plaintext, 0, SEEK_END);
   int plaintextSize = ftell(plaintext);
   int bytesToPad = 16 - (plaintextSize % 16);
   unsigned char byteToPad[1];
   byteToPad[0] = (unsigned char) bytesToPad;
   int i;
   for (i = 0; i < bytesToPad; i++){
      fwrite(byteToPad, 1, 1, plaintext);
   }
	fseek(plaintext, 0, SEEK_SET);

   unsigned long seed = getSeed((unsigned char*) argv[1]);
	printf("using seed=%lu from password=\"%s\"\n",seed, argv[1]);
	unsigned char lastNumber = (unsigned char) numberGen(seed);
	unsigned char initVector[16];
   initVector[0] = lastNumber;
	//fill initializationVector
   for (i = 1; i < 16; i++){
      initVector[i] = (unsigned char) numberGen(lastNumber);
		lastNumber = initVector[i];
   }
   unsigned char plaintextBlock[16];
	unsigned char tempBlock[16];
	unsigned char ciphertextBlock[16];
	unsigned char keyBytes[16];
	int firstBlock = 1;
	unsigned char first;
	unsigned char second;
	unsigned char temp;
   while(fread(plaintextBlock, sizeof(unsigned char), sizeof(plaintextBlock), plaintext)){
		//if first block of plaintext - we XOR tempBlock w/ initializationVector first
		if (firstBlock == 1){
			for (i = 0; i < 16; i++){
				tempBlock[i] = plaintextBlock[i] ^ initVector[i];
				//at the same time fill keyStream
				keyBytes[i] = (unsigned char) numberGen(lastNumber);
				lastNumber = keyBytes[i];
			}
			for (i = 0; i < 16; i++){
				//find which bytes of temp block to swa, then swap them
				first = keyBytes[i] & 0xf;
				second = (keyBytes[i] >> 4) & 0xf ;
				temp = tempBlock[first];
				tempBlock[first] = tempBlock[second];
				tempBlock[second] = temp;
			}
			for (i = 0; i < 16; i++){
				//final ciphertext = tempBlock XOR keystream
				ciphertextBlock[i] = tempBlock[i] ^ keyBytes[i];
				//write to ciphertextfile
				fputc(ciphertextBlock[i], ciphertext);
			}
			firstBlock = 0;
		}
		else{
			//same loop as above, instead use previous ciphertextBlock instead of initializationVector
			for (i = 0; i < 16; i++){
				tempBlock[i] = plaintextBlock[i] ^ ciphertextBlock[i];
				keyBytes[i] = (unsigned char) numberGen(lastNumber);
				lastNumber = keyBytes[i];
			}
			for (i = 0; i < 16; i++){
				first = keyBytes[i] & 0xf;
				second = (keyBytes[i] >> 4) & 0xf ;
				temp = tempBlock[first];
				tempBlock[first] = tempBlock[second];
				tempBlock[second] = temp;
			}
			for (i = 0; i < 16; i++){
				ciphertextBlock[i] = tempBlock[i] ^ keyBytes[i];
				fputc(ciphertextBlock[i], ciphertext);
			}

		}
   }
	fclose(ciphertext);
	fclose(plaintext);
}
