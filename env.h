#ifndef ENV_H
#define ENV_H

typedef enum 
{
    VER_INVALID,
    VER_NONE,
    VER_WORK,
    VER_INITERROR
} ver_status;

typedef enum
{
     VER_INIT,
     VER_DEINIT,
     VER_GETCURRID,
     VER_SIGNSPR
} ver_action;


typedef struct
{
    ver_status status;
    ver_action action;
    unsigned int error;
    unsigned int verba_error;
    const char *path;
    uint8_t ser[VER_SERIES_LENGTH+1];
    uint8_t encrId[VER_ENCRYPTION_ID_LENGTH+1];
    uint8_t signId[VER_SIGN_ID_LENGTH+1];
} ver_env;

#endif //#ifndef ENV_H
