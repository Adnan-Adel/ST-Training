#ifndef ALIAS_H
#define ALIAS_H

void init_aliases();
void add_alias(const char* alias, const char* command);
void expand_aliases(char** args);

#endif

