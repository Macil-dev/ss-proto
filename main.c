#define UNICODE 1
#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "verface.h"
#include "env.h"
#include "err.h"
#include "ver.h"

void
usage()
{
    wprintf(L"Usage:\n\tss <base dir> <key series>\n");
}

void
report_error(ver_env *env)
{
    const ver_errspec *err;
    err = ver_lookup_error(ver_verba_error(env));
    if (err == NULL) {
        fwprintf(stderr, L"Неизвестный код ошибки Verba %d.\n", ver_verba_error(env));
        return;
    }
    if (err->code != env->verba_error) {
        fwprintf(stderr, L"Ошибка при поиске описания ошибки с кодом %d.\n", ver_verba_error(env));
        return;
    }

    fwprintf(stderr, L"Ошибка %d %s %s\n", err->code, err->mnemonic, err->description);   
}

int
main(int argc, char** argv)
{
#ifdef VER_MSVC_UNICODE_QUIRK
    _setmode(_fileno(stdout), 0x20000);
    _setmode(_fileno(stderr), 0x20000);
#endif //#ifdef VER_MSVC_UNICODE_QUIRK
        if (argc < 3) {
            usage();
        }
        int e = 0;
        ver_env *env = malloc(sizeof(ver_env));
        ver_environment(env);
        ver_env_set_path(env, argv[1]);
        strncpy(env->ser, argv[2], 6);
        
        ver_init(env);

        ver_getCurrID(env);
        if (!ver_error(env)) {
            wchar_t encridb[VER_ENCRYPTION_ID_LENGTH+1];
            wmemset(encridb, 0, VER_ENCRYPTION_ID_LENGTH+1);
            const char* env_encrid = env->encrId;
            mbsrtowcs(encridb, &env_encrid, VER_ENCRYPTION_ID_LENGTH, NULL);
            wchar_t signidb[VER_SIGN_ID_LENGTH+1];
            wmemset(signidb, 0, VER_SIGN_ID_LENGTH+1);
            const char *env_signid = env->signId;
            mbsrtowcs(signidb, &env_signid, VER_SIGN_ID_LENGTH);
            wprintf(L"Загружены ключи: %s - %s\n", encridb, signidb);
        } else {
            report_error(env);
            e = 1;
            goto cleanup;
        }
         
        ver_signSpr(env);
        if (ver_error(env)) {
            report_error(env);
            e = 1;
            goto cleanup;
        }

        wprintf(L"Справочники подписаны.\n");

cleanup:
        ver_done(env);
        return e;
}
