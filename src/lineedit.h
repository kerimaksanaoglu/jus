#ifndef JUS_LINEEDIT_H
#define JUS_LINEEDIT_H
#include "common.h"
/* Tamamlama: prefix (uzunluğu prefixLength bayt) ile başlayan her aday için add(candidate, context) çağrılır. */
typedef void (*LineEditAddFn)(const char *candidate, void *context);
typedef void (*LineEditCompleteFn)(const char *prefix, int prefixLength, LineEditAddFn add, void *context);
/* Standart girdi ve çıktı bir uçbirimse (ve Windows'ta konsol kipi ayarlanabiliyorsa) doğru. */
bool lineEditSupported(void);
/* Bir satır okur. Dönen metin malloc ile ayrılmış, satır sonu içermeyen UTF-8'dir; çağıran free eder.
   Girdi kapandıysa (boş satırda Ctrl+D / Windows'ta Ctrl+Z ya da gerçek EOF) NULL döner.
   Ctrl+C yazılan satırı iptal eder ve boş metin ("") döndürür. complete NULL olabilir. */
char *lineEdit(const char *prompt, LineEditCompleteFn complete);
void lineEditHistoryAdd(const char *line);
/* Geçmişi dosyadan yükler / dosyaya yazar; dosya açılamazsa sessizce geçer. */
void lineEditHistoryLoad(const char *path);
void lineEditHistorySave(const char *path);
#endif
