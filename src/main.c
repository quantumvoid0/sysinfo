#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"

int main(int argc, char **argv) {
	char out[OUT_SZ];
	int rc = dispatch_command(argc, argv, out, sizeof(out));
	if (out[0] != '\0')
		fputs(out, stdout);
	return rc;
}
