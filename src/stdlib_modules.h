#ifndef JUS_STDLIB_MODULES_H
#define JUS_STDLIB_MODULES_H

/* Standart kütüphane modüllerini (matematik, rastgele, zaman, dosya, sistem) tanımlar. */
void defineStandardModules(void);

/* Programa komut satırından verilen argümanları sistem.argümanlar listesine yazar. */
void setScriptArguments(int count, char **arguments);

#endif
