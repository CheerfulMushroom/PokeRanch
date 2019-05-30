#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "pokemon_model_path.h"
#include "string.h"

using namespace std;

char* pokemon_model_path(int pok_id, char* anim_name) {
    
    FILE *pokemon_model_ptr = fopen("pokemon_model_path.dat", "r");
    
    if (pokemon_model_ptr == NULL) {
        
        puts("No such file");
    
    } else {
        
        int input_id;
        
        //char* input_name = new char[20];
        char pok_name[20];
        while (fscanf(pokemon_model_ptr, "%d%s", &input_id, pok_name) == 2) {
            
            if (input_id == pok_id) {
                
                fclose(pokemon_model_ptr);
                
                char* ending = ".dae";
                string path_to_model = "models/";
                path_to_model += pok_name;
                path_to_model += '/';
                path_to_model += anim_name;
                path_to_model += ending;
                char* char_path = new char[50];
                strcpy(char_path, path_to_model.c_str());
                return char_path;
            }
        }
        fclose(pokemon_model_ptr);
      }
    return NULL;
}
