#ifndef VER_H
#define VER_H

void ver_clean_error(ver_env *env);
void ver_set_verba_error(ver_env *env, int err);
void ver_flag_error(ver_env *env);
int ver_error(ver_env *env);
int ver_verba_error(ver_env *env);
int ver_init(ver_env *env);
int ver_done(ver_env *env);
int ver_getCurrID(ver_env* env);
int ver_signSpr(ver_env *env);
void ver_environment(ver_env *env);
void ver_env_set_path(ver_env *env, const char *path);
const ver_errspec* ver_lookup_error(int code);

#endif //#ifndef VER_H
