/*
@author Patrick Kupcha (netid kupcha)
Rutgers CS419, Section 02, Fall 2020
sbdecrypt.c

Block encryption with cipher block chaining and padding - decryption
*/

#include <stdio.h>
#include <stdlib.h>

int isPadding(unsigned char curr){
   if (curr == (unsigned char) 1 || curr == (unsigned char) 2 || curr == (unsigned char) 3 || curr == (unsigned char) 4 || curr == (unsigned char) 5 || curr == (unsigned char) 6 || curr == (unsigned char) 7 || curr == (unsigned char) 8 || curr == (unsigned char) 9 || curr == (unsigned char) 10 || curr == (unsigned char) 11 || curr == (unsigned char) 12 || curr == (unsigned char) 13 || curr == (unsigned char) 14 || curr == (unsigned char) 15 || curr == (unsigned char) 16){
      return 1;
   }
   else{
      return 0;
   }
}

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
   //input should be `sbdecrypt password ciphertext plaintext`
   if (argc != 4){
      printf("ERROR: improper input.\n");
   }
   FILE *ciphertext;
   ciphertext = fopen(argv[2], "r");
   if (ciphertext == NULL){
      printf("ERROR opening ciphertext file\n");
      return 0;
   }
	FILE *plaintext;
	plaintext = fopen(argv[3], "w");
	if (plaintext == NULL){
		printf("ERROR opening plaintext file\n");
		return 0;
	}
   //iterations now holds how many 16 byte blocks we will decrypt
   fseek(ciphertext, 0, SEEK_END);
   int ciphertextSize = ftell(ciphertext);
   int iterations = ciphertextSize / 16;
   fseek(ciphertext, 0, SEEK_SET);

   unsigned long seed = getSeed((unsigned char*) argv[1]);
   printf("using seed=%lu from password=\"%s\"\n",seed, argv[1]);
   unsigned char lastNumber = (unsigned char) numberGen(seed);
   unsigned char initVector[16];
   initVector[0] = lastNumber;
   int i;
   //get initializationVector
   for (i = 1; i < 16; i++){
      initVector[i] = numberGen(lastNumber);
      lastNumber = initVector[i];
   }
   unsigned char plaintextBlock[16];
   unsigned char tempBlock[16];
   unsigned char ciphertextBlock[16];
   unsigned char lastCiphertextBlock[16];
   unsigned char keyBytes[16];
   int firstBlock = 1;
   unsigned char first;
   unsigned char second;
   unsigned char temp;
   //we calculated how many blocks of text there will be, program will run this many loops
   while (iterations > 0){
      //get a block of 16 bytes of ciphertext
      fread(ciphertextBlock, sizeof(unsigned char), sizeof(ciphertextBlock), ciphertext);
      for (i = 0; i < 16; i++){
         keyBytes[i] = numberGen(lastNumber);
         lastNumber = keyBytes[i];
         tempBlock[i] = keyBytes[i] ^ ciphertextBlock[i];
      }
      for (i = 15; i >= 0; i--){
         first = keyBytes[i] & 0xf;
         second = (keyBytes[i] >> 4) & 0xf ;
         temp = tempBlock[first];
         tempBlock[first] = tempBlock[second];
         tempBlock[second] = temp;
      }
      if (firstBlock == 1){
         if (iterations == 1){
            for (i = 0; i < 16; i++){
               plaintextBlock[i] = tempBlock[i] ^ initVector[i];
               if (!isPadding(plaintextBlock[i])){
                  fputc(plaintextBlock[i], plaintext);
               }
            }
         }
         else{
            for (i = 0; i < 16; i++){
               plaintextBlock[i] = tempBlock[i] ^ initVector[i];
               fputc(plaintextBlock[i], plaintext);
               lastCiphertextBlock[i] = ciphertextBlock[i];
            }
         }
         firstBlock = 0;
      }
      else{
         if (iterations == 1){
            for (i = 0; i < 16; i++){
               plaintextBlock[i] = tempBlock[i] ^ lastCiphertextBlock[i];
               if (!isPadding(plaintextBlock[i])){
                  fputc(plaintextBlock[i], plaintext);
               }
            }
         }
         else{
            for (i = 0; i < 16; i++){
               plaintextBlock[i] = tempBlock[i] ^ lastCiphertextBlock[i];
               fputc(plaintextBlock[i], plaintext);
               lastCiphertextBlock[i] = ciphertextBlock[i];
            }
         }
      }
      iterations--;
   }

}
