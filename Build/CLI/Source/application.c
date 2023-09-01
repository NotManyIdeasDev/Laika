#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "application.h"

application_t* application = NULL;
const char* commands[COMMAND_COUNT] = { "load", "drop", "quit" };

application_t* create_application_context(const char* program_path, u32_t buflen)
{
    if (application) destroy_application_context(application);

    application = (application_t*)malloc(sizeof(application_t));

    application->program_path = (char*)malloc(buflen + 1);
    application->loaded_filename = (char*)malloc(1);
    application->running = 1;
    application->exit_code = -1;
    application->mode = None;

    strcpy(application->program_path, program_path);
    application->program_path[buflen] = NULL;
    application->loaded_filename[0] = NULL;

    return application;
}

i32_t destroy_application_context(application_t* app_ptr)
{
    i32_t exit_code = app_ptr->exit_code;

    free(app_ptr->program_path);
    free(app_ptr->loaded_filename);
    free(app_ptr);

    return exit_code;
}

void application_run(application_t* context)
{
    while (context->running)
    {
        char command[COMMAND_LENGTH];
        printf("[%s] $ ", application_get_mode_text(context->mode));
        fgets(command, COMMAND_LENGTH, stdin);
        command[strlen(command) - 1] = 0;

        application_parse_command(command);
    }
}

void application_display_error_message(i32_t result)
{
    // TODO : Add message for each error code
    printf("Error code: %d\n", result);
}

char* application_get_mode_text(application_mode_t mode)
{
    switch (mode)
    {
    case None:
        return "Convert";
    case Loaded:
        return application->loaded_filename;
    default:
        return "";
    }
}

i32_t command_load(char** params, i32_t count)
{
    if (count == 1) return COMMAND_MISSING_DATA;
    if (count > MAX_LOAD_PARAMS) return COMMAND_OVERFLOW;
   
    char* filename = params[1];
    FILE* file = fopen(filename, "rb");

    if (!file) return COMMAND_INVALID_DATA;

    fclose(file);

    free(application->loaded_filename);
    application->mode = Loaded;
    application->loaded_filename = (char*)malloc(strlen(filename) + 1);
    strcpy(application->loaded_filename, filename);
    application->loaded_filename[strlen(filename)] = 0;

    return COMMAND_SUCCESS;
}

i32_t command_drop(char** params, i32_t count)
{
    if (count > MAX_DROP_PARAMS) return COMMAND_OVERFLOW;

	free(application->loaded_filename);
	application->mode = None;
    application->loaded_filename = (char*)malloc(1);
	application->loaded_filename[0] = 0;

    return COMMAND_SUCCESS;
}

i32_t command_exit(i32_t exit_code)
{
    application->exit_code = exit_code;
    application->running = 0;

    return exit_code;
}

void application_parse_command(char* command)
{
    i32_t size = 0;
    i32_t capacity = 2;
    char** params = (char**)malloc(capacity * sizeof(char*));

	for (i32_t i = 0; i < COMMAND_LENGTH; i++)
	{
        if (command[i] == 32 || command[i] == 0)
        {
            if (size >= capacity)
            {
                capacity *= 1.5f;
                params = (char**)realloc(params, capacity);
            }

            params[size] = (char*)malloc(i + 1);

            const char* source = size == 0 ? command : command + strlen(params[size - 1]) + 1;
            size_t count = size == 0 ? i : i - strlen(params[size - 1]) - 1;

            char* debug = strncpy(params[size], source, count);
            params[size][count] = 0;
            size++;

            if (command[i] == 0) break;
        }
	}

    application_evaluate_command(params, size);
}

void application_evaluate_command(char** params, i32_t count)
{
    if (count == 0) return;

    // Load the first command
    char* command = params[0];

    for(i32_t i = 0; i < COMMAND_COUNT; i++)
    {
        if (strcmp(command, commands[i]) == 0)
        {
            i32_t result = application_invoke_command(i, params, count);
            if (result) application_display_error_message(result);
        }
    }

    // Destroy the parameters information
	for (i32_t i = 0; i < count; i++)
	{
		free(params[i]);
	}

	free(params);
}

int application_invoke_command(i32_t command, char** params, i32_t count)
{
    switch (command)
    {
    case COMMAND_LOAD:
        return command_load(params, count);
    case COMMAND_DROP:
        return command_drop(params, count);
    case COMMAND_EXIT:
        return command_exit(0);
    default:
        return -1;
    }
}
