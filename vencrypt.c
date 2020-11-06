/*
@author Patrick Kupcha (netid kupcha)
Rutgers CS419, Section 02, Fall 2020
vencrypt.c

Binary Vigenère Cipher - Encryption
*/

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv){
   //input should be format "vencrypt keyfile plaintext ciphertext"
   if (argc != 4){
      printf("ERROR: incorrect input\n");
   }
   //stores key in array
   FILE *keyfile;
   keyfile = fopen(argv[1], "rb");
   fseek(keyfile, 0, SEEK_END);
   int keySize = ftell(keyfile);
   fseek(keyfile, 0, SEEK_SET);
   unsigned char key[keySize];
   int i = 0;
   for (i = 0; i < keySize; i++) {
      unsigned char value;
      fread(&value, 1, 1, keyfile);
      key[i] = value;
   }
   fclose(keyfile);
   printf("keyfile=%s, length=%i\n", argv[1], keySize);

   FILE *plaintext;
   plaintext = fopen(argv[2], "r");
   unsigned char buffer[1];

   FILE *ciphertext;
   ciphertext = fopen(argv[3], "w");
   if (ciphertext == NULL){
      printf("ERROR creating ciphertext file\n");
      return 0;
   }
   if (keySize == 0){
      while(fread(buffer, sizeof(buffer), 1, plaintext)){
         fputc(buffer[0], ciphertext);
      }
      //write plaintext to ciphertext
   }
   else{
      i = 0;
      while(fread(buffer, sizeof(buffer), 1, plaintext)){ 
         int currPlain = buffer[0];
         int  currKey = key[i % keySize];
         unsigned char currCipher = (unsigned char) (currPlain + currKey) % 256;
         fputc(currCipher, ciphertext);
         i++;
      }
   }
}
