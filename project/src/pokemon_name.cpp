#include <stdio.h>
#include <stdlib.h>
#include "pokemon_name.h"
char* pokemon_name(int pok_id) {
    
    FILE *pokemon_name_ptr = fopen("pokemon_name.dat", "r");
    if (pokemon_name_ptr == NULL) {
        puts("No such file");
    } else {
        int input_id;
        char* input_name = new char[20];
        while (fscanf(pokemon_name_ptr, "%d%s", &input_id, input_name) == 2) {
            if (input_id == pok_id) {
                fclose(pokemon_name_ptr);
                return input_name;
            }
        }
        fclose(pokemon_name_ptr);
      }
    return NULL;
}
