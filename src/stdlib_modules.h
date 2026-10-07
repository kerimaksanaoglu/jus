#ifndef JUS_STDLIB_MODULES_H
#define JUS_STDLIB_MODULES_H

/* Standart kütüphane modüllerini (matematik, rastgele, zaman, dosya, sistem) tanımlar. */
void defineStandardModules(void);

/* json ve tr modülleri (stdlib_data.c). defineStandardModules tarafından çağrılır. */
void defineDataModules(void);

/* ağ modülü (stdlib_net.c). defineStandardModules tarafından çağrılır. */
void defineNetworkModule(void);

/* Yorumlayıcıya gömülü JUS modülünün kaynağı (embedded.c); böyle bir modül yoksa NULL. */
const char *embeddedModuleSource(const char *name);

/* Çalıştırılan dosyanın yolunu sistem.betik ve sistem.betik_klasörü üyelerine yazar. */
void setScriptPath(const char *path);

/* Programa komut satırından verilen argümanları sistem.argümanlar listesine yazar. */
void setScriptArguments(int count, char **arguments);

#endif
