#ifndef demo_battle_cli_glue_H
#define demo_battle_cli_glue_H

void glue_input_command__(long actor_key, const unsigned char* data, int size);
void glue_battle_entry__(long actor_key, const unsigned char* data, int size);
void glue_battle_exit__(long actor_key, const unsigned char* data, int size);

#endif
