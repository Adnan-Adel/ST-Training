#ifndef HISTORY_H
#define HISTORY_H

void init_history();                          
void save_to_history(const char* command);   
void show_history();                        
const char* get_previous_command();          
const char* get_next_command();             
void reset_history_index();                 

#endif // HISTORY_H

