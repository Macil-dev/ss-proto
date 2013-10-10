#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "verface.h"
#include "err.h"
#include "errcodes.h"
#include "env.h"
#include "ver.h"

void
ver_clean_error(ver_env *env)
{
    env->error = 0;
    env->verba_error = 0;
}

void
ver_set_verba_error(ver_env *env, int err)
{
    env->error = 1;
    env->verba_error = err;
}

void
ver_flag_error(ver_env *env)
{
    env->error = 1;
}

int
ver_error(ver_env *env)
{
    return env->error;
}

int
ver_verba_error(ver_env *env)
{
    return env->verba_error;
}

int
ver_init(ver_env *env)
{
    if (env->status == VER_NONE) {
#ifdef VER_INIT_VERBA
        CryptoInit((char*)env->path, (char*)env->path);
        SignInit((char*)env->path, (char*)env->path);
#endif //#ifdef VER_INIT_VERBA
        env->error = 0;
        env->action = VER_INIT;
        env->status = VER_WORK;
    }
    return env->error;
}

int
ver_done(ver_env *env)
{
#ifdef VER_INIT_VERBA
    SignDone();
    CryptoDone();
#endif //#ifdef VER_INIT_VERBA
    env->error = 0;
    env->action = VER_DEINIT;
    env->status = VER_NONE;
    return env->error;
}

int
ver_getCurrID(ver_env* env)
{
    if (env->status == VER_WORK) {
        //Celan error and set current "Verba" action
        ver_clean_error(env);
        env->action = VER_GETCURRID;
        
        uint16_t err;
        
        //Get encryption key ID from "Verba" slot #0
        err = GetCurrID(env->encrId, 'E');
        if (err) {
            ver_set_verba_error(env, err);
            goto err;
        }
        
        err = 0;
        ver_clean_error(env);
        
        //Get encryption key ID from "Verba" slot #0
        err = GetCurrID(env->signId, 'S');
        if (err) {
            ver_set_verba_error(env, err);
            goto err;
        }
    } else {
        //Invalid library state
        ver_flag_error(env);
    }
err:
    return env->error;
}

int
ver_signSpr(ver_env *env)
{
    if(env->status == VER_WORK) {
        //Celan error and set current "Verba" action
        ver_clean_error(env);
        env->action = VER_SIGNSPR;
        uint16_t err;

        //HMAC for encryption open key registry
        err = SignSpr((char*)env->path, 
                      (char*)env->ser,
                      (char*)env->encrId, 'E');
        if (err) {
            ver_set_verba_error(env, err);
            goto err;
        }
        
        //HMAC for signature open key registry
        err = SignSpr((char*)env->path, 
                      (char*)env->ser,
                      (char*)env->signId, 'S');
        if (err) {
            ver_set_verba_error(env, err);
            goto err;
        }

    } else {
        //Invalid library status
        ver_flag_error(env);
    }
err:
    return env->error;
}

void
ver_environment(ver_env *env)
{
    memset(env, 0, sizeof(env));
    env->status = VER_NONE;
}

void
ver_env_set_path(ver_env *env, const char *path)
{
    env->path = path;
}

const ver_errspec*
ver_lookup_error(int code)
{
    if (0 <= code && code <= 12)
        return &err1[code];
    else if (21 <= code && code <= 39)
        return &err2[code-21];
    else if (101 <= code && code <= 127)
        return &err3[code-101];
    else
        return NULL;
}
