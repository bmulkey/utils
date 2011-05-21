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
	puts("got here");
	fnode *newnode = fnode_alloc ();
	newnode->name = fname;
	cur->next = newnode;
	puts("returning new node");
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
