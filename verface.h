#ifndef VERFACE_H
#define VERFACE_H

uint16_t CryptoInit(char *pathToSecret, char *pathToBase);
uint16_t SignInit(char *pathToSecret, char *pathToBase);

uint16_t WINAPI CryptoDone(void);
uint16_t WINAPI SignDone(void);

#define VER_ENCRYPTION_ID_LENGTH 11
#define VER_SIGN_ID_LENGTH 13
#define VER_SERIES_LENGTH 6

uint16_t WINAPI GetCurrID(uint8_t *curr_ID, uint8_t S_or_E);

uint16_t WINAPI SignSpr(uint8_t *base_dir, uint8_t *ser, uint8_t *my_ID, uint8_t S_or_E);

#endif //#ifndef VERFACE_H
