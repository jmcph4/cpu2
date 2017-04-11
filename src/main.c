#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "error.h"
#include "isa.h"
#include "state.h"
#include "control.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s input_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    unsigned int res = 0;

    State state;

    FILE* input_file = fopen(argv[1], "rb");

    if(input_file == NULL)
    {
        fprintf(stderr, "[ERROR] Failed to open machine code file.\n");
        return EXIT_FAILURE;
    }

    /* get file size */
    fseek(input_file, 0, SEEK_END);
    size_t input_file_size = ftell(input_file);
    rewind(input_file);

    if(input_file_size % (ISA_WORD_SIZE / 8) != 0)
    {
        fprintf(stderr, "Input machine code misaligned.\n");
        return EXIT_FAILURE;
    }

    unsigned int num_words = input_file_size / (ISA_WORD_SIZE / 8);

    uint32_t* code = calloc(num_words, sizeof(uint32_t));

    if(code == NULL)
    {
        return EXIT_FAILURE;
    }

    for(unsigned int i=0;i<num_words;i++)
    {
        int words_read = fread(&code[i], sizeof(uint32_t), 1, input_file);

        if(words_read != 1)
        {
            fprintf(stderr, "[ERROR] Failed to read machine code file.\n");
            free(code);
            return EXIT_FAILURE;
        }
    }

    res = state_init(&state);

    if(res != CPU2_ERR_SUCCESS)
    {
        fprintf(stderr, "[ERROR] Failed to initialise CPU state.\n");
        free(code);
        return EXIT_FAILURE;
    }

    process(code, num_words, &state); /* main loop */

    /* clean up */
    state_free(&state);
    free(code);

    return EXIT_SUCCESS;
}

