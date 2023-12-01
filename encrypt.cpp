/* encrypt.cpp - Implementation of the encryption functions (XOR, Vigenere)
 * By: Michael 
 *
 */

#include <ctype.h>

void xorCipher(void* message, int messageLength, void* secretKey, int secretKeyLength, void* encBuf) {
    int i;
    char* msg, * key, * enc;                          // Cast buffers to single byte (char) arrays 
    msg = (char*)message;
    key = (char*)secretKey;
    enc = (char*)encBuf;
    for (i = 0; i < messageLength; i++) {
        enc[i] = msg[i] ^ key[i % secretKeyLength];  // XOR encrypt bytewise (loop the key as many times as required
    }
    enc[messageLength] = '\0';                       // Null terminate the ecrypted message
    encBuf = (void*)enc;                             // Encrypted/decrypted buffer   
}

void vigCipher(void* message, int messageLength, void* secretKey, int secretKeyLength, void* encBuf, bool encOrDec) {
    // encOrDec determines if the function is used for encryption or decryption
    int i;
    char n;                                          // Shift amount
    char* msg, * key, * enc;                         // Cast buffers to single byte (char) arrays 
    msg = (char*)message;
    key = (char*)secretKey;
    enc = (char*)encBuf;
    for (i = 0; i < messageLength; i++) {
        // Convert key to upper case and get n from (distance from 'A' % 26)
        // key[i % secretKeyLength] just loops around the key as many times as necessary to match the legth of hte message
        n = (toupper(key[i % secretKeyLength]) - 'A') % 26;
        if (encOrDec == true) {
            enc[i] = (msg[i] + n);                   // Viginere encrypt - if true
        }
        else {
            enc[i] = (msg[i] - n);                   // Viginere decrypt - if false
        }
    }
    enc[messageLength] = '\0';                       // Null terminate the ecrypted message
    encBuf = (void*)enc;                             // Encrypted/decrypted buffer   
}
