/*
 * $Id: filesys-dir.c,v 1.1 2002/11/01 22:44:05 jjo Exp $
 * 
 * Author: JuanJo Ciarlante <jjo@um.edu.ar>
 *         Based on GNU LIBC manual examples
 *         
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

int main (int argc, char *argv[])
{
	DIR *dp;
	struct dirent *ep;
	struct stat stat_buf;
	char *dir;

	dir=argv[1];
	if (!dir) {
		fprintf(stderr, "uso: %s <directorio>\n", argv[0]);
		return 255;
	}

	dp = opendir (dir);
	if (dp != NULL)
	{
		while ((ep = readdir (dp))) {
			/* comparable con "ls -liU" */
			stat(ep->d_name, &stat_buf);
			printf ("[%8ld] %c %08o %10ld %s\n",
				stat_buf.st_ino,
				S_ISDIR(stat_buf.st_mode) ?
					'd' : '-',
				stat_buf.st_mode,
				stat_buf.st_size,
				ep->d_name);
		}
		(void) closedir (dp);
	}
	else
		fprintf(stderr, "No pude abrir el directorio \"%s\"\n",
				dir);

	return 0;
}


