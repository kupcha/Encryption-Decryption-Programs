/*
@author Patrick Kupcha (netid kupcha)
Rutgers CS419, Section 02, Fall 2020
vdecrypt.c

Binary Vigenère Cipher - Decryption
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
   //proper input should be ./vdecrypt keyfile ciphertext plaintext
   if (argc != 4){
      printf("ERROR: incorrect input\n");
   }
   //get key and store in array for easy access later
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

   FILE *ciphertext;
   ciphertext = fopen(argv[2], "r");
   unsigned char buffer[1];

   FILE *plaintext;
   plaintext = fopen(argv[3], "w");
   if (plaintext == NULL){
      printf("ERROR creating plaintext file\n");
      return 0;
   }
   // if no key given, there is no decrypting to do -- plaintext will be equal to ciphertext
   if (keySize == 0){
      printf("Key size is 0 -- plaintext is ciphertext.\n");
      while(fread(buffer, sizeof(buffer), 1, ciphertext)){
         fputc(buffer[0], plaintext);
      }
   }
   else{
      i = 0;
      //iterate through ciphertext until finished and write to plaintext
      while(fread(buffer, sizeof(buffer), 1, ciphertext)){
         int currCipher = buffer[0];
         int  currKey = key[i % keySize];
         unsigned char currPlain = (unsigned char) 256 - (currKey - currCipher);
         fputc(currPlain, plaintext);
         i++;
      }
   }
}
