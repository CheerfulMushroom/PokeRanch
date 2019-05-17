#include <stdio.h>
#include <stdlib.h>
#include "pokemon_model_path.h"
char* pokemon_model_path(int pok_id) {
    
    FILE *pokemon_model_ptr = fopen("pokemon_model_path.dat", "r");
    if (pokemon_model_ptr == NULL) {
        puts("No such file");
    } else {
        int input_id;
        char* input_name = new char[20];
        while (fscanf(pokemon_model_ptr, "%d%s", &input_id, input_name) == 2) {
            if (input_id == pok_id) {
                fclose(pokemon_model_ptr);
                return input_name;
            }
        }
        fclose(pokemon_model_ptr);
      }
    return NULL;
}
