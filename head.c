/*
* head -- a functional clone of UNIX 'head' utility.
* Copyright (C) 2011 Bryan Mulkey (syriacman AT gmail DOT com)
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

typedef struct _fnode
{
	char *name;
	struct _fnode *next;
} fnode;

fnode *head;

fnode *
fnode_alloc()
{
	return (fnode *) malloc (sizeof(fnode));
}

fnode *
insert_after (fnode *cur, char *fname)
{
	fnode *newnode = fnode_alloc ();
	newnode->name = fname;
	cur->next = newnode;
	return newnode;
}

void
process_args (int argc, char **argv)
{
	int i=0, count=0;
	fnode *cur = head, *prev;
	for (i=1; i < argc; i++) {
		if (strncmp (argv[i], "-", 1) != 0) {
			count++;
			if (count < 2) {
				head = fnode_alloc();
				head->name = argv[i];
				prev = head;
			} else {
				prev = insert_after (prev, argv[i]);
			}
		}
	}
}

void
print_heads()
{
	FILE *fptr;
	fnode *file = head;
	int c, newlines = 0;

	for (file = head; file != NULL; file = file->next) {
		fptr = fopen (file->name, "r");
		if (fptr == NULL) {
			perror ("fopen");
			continue;
		}

		printf ("\n==> %s <==\n", file->name);

		while ((c = fgetc (fptr)) != EOF && newlines < 10) {
			putchar (c);
			if (c == '\n') {
				++newlines;
			}
		}
		newlines = 0;
		fclose (fptr);
	}
	
	if (head == NULL) {
		fptr = stdin;
		while ((c = fgetc(fptr)) != EOF && newlines < 10) {
			putchar(c);
			if (c == '\n') {
				++newlines;
			}
		}
	}
}

void
free_args()
{
	fnode *node = head;
	while (node != NULL) {
		fnode *temp = node->next;
		free (node);
		node = temp;
	}
}

int
main (int argc, char **argv)
{
	process_args (argc, argv);
	print_heads ();
	free_args ();
	return 0;
}
