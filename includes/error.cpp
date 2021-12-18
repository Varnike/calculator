#include "error.h"

int ERRNUM = NO_ERR;

const char *errmsg(int errt)
{
        const char *messages[] = {
                "Success",
                "Unknown error type!",
                "Calloc error!",
                "An error occured while reading from file!",
                "An error occured while writing into file!",
		"Structure pointer error!",
		"Buffer error!",
		"An error occured while trying to get file size!",
		"An error occured while closing file!",
		"An error occured while opening file!",
		"Realloc error!",
		"Underflow error!",
		"Stack structure was destroyed, but not initialized!",
		"Stack NULL pointer!",
		"Uninizialized stack data!",
		"Stack overflow!",
		"Invalid stack for StackCtor!",
		"Invalid structure left canary!",
		"Invalid structure right canary!",
		"Both canaries are invalid",
		"Invalid data right canary!",
		"Invalid data left canary!",
		"Invalid hash, data corrupted!"
        };

	const char *proc_messages[] = {
		"Assembler: syntax error!",
		"Unknown command error!",
		"Unknown value!",
		"Wrong input file version!",
		"Invalid file signature!",
		"Invalid binary data!",
		"Error occured while reading header!",
		"Invalid input data for assembler!",
		"Labels limit exceeded!",
		"Syntax error: several labels of same name!"
	};

        if (errt >= 0   && errt <= ERRS_CNT)
                return messages[errt];

	if (errt >= 100 && errt < PROC_ERRS + 100)
		return proc_messages[errt - 100];
        return messages[UNKNOWN_ERR];
}

