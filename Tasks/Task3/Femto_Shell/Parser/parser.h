/* --------------------- Includes --------------------- */
#include "../String/my_string.h"
#include <unistd.h>

/* --------------------- Macros Declarations --------------------- */
#define COMMANDS_LENGTH         4

/* --------------------- data types Declarations --------------------- */
typedef struct
{
    char* token;
    char* token_type;
}token_t;

/* --------------------- Functions Declarations --------------------- */
uint8_t is_valid_Command(char* str);

void Tokenizer(char* src);

