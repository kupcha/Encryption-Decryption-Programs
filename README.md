# Encryption-Decryption-Programs

Call make to compile all programs.

Usage:
1. Binary Vigenère Cipher

vencrypt: "./vencrypt keyfile plaintextFile ciphertextFile" 

-- where keyfile is a .txt file containing the key, plaintextFile is the file to be encrypted, and ciphertextFile is the file the ciphertext will be written to.

vdecrypt: "./vdecrypt keyfile ciphertextFile plaintextFile" 

-- where keyfile is a .txt file containing the key, ciphertextFile is file to be decrypted, and plaintextFile is the file plaintext will be written to after decryption takes place.

2. Stream cipher

scrypt (encrypt): "./scrypt password plaintextFile ciphertextFile"

scrypt (decrypt): "./scrypt password ciphertextFile plaintextFile"

-- in this case, the password is passed as an argument instead of in a file. We use the same program for encryption an decryption

3. Block encryption with cipher block chaining and padding

sbencrypt: "./sbencrypt password plaintextFile ciphertextFile"

sbdecrypt: "./sbdecrypt password ciphertextFile plaintextFile"

-- again, password is passed as an argument instead of a file


