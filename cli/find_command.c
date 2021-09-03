#include "cli.h"
#include <string.h>

/**
 * get_commands - array of commands
 * Return: pointer to the first element in the array
 */
command_t *get_commands(void)
{
	static command_t commands[] = {
		{wallet_load, "wallet_load"},
		{wallet_save, "wallet_save"},
		{send_cmd, "send"},
		{mine_cmd, "mine"},
		{info_cmd, "info"},
		{load_cmd, "load"},
		{save_cmd, "save"},
		{NULL, NULL}
	};
	return (commands);
}

/**
 * find_command - fin a command
 * @name: command name
 * Return: NULL if not command found,
 *         or pointer to the found command
 */
command_t *find_command(char *name)
{
	command_t *command = get_commands();

	while (command->name)
	{
		if (strcmp(name, command->name) == 0)
			return (command);
		command += 1;
	}
	return (NULL);
}
