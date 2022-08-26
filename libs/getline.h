/* Copyright (C) 1991 Free Software Foundation, Inc.
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

/* CHANGED FOR VMS */

/*
 * <getline.c>
 **
 ** HISTORY:
 **	 8 Jul 94  FM	Include "HTUtils.h" for memory allocation and free()
 **			substitutions with VAXC on VMS.
 **
 */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Read up to (and including) a newline from STREAM into *LINEPTR
   (and null-terminate it). *LINEPTR is a pointer returned from malloc (or
   NULL), pointing to *N characters of space.  It is realloc'd as
   necessary.  Returns the number of characters read (not including the
   null terminator), or -1 on error or EOF.  */

int getline(char** lineptr, size_t* n, FILE* stream) {
    static char line[256];
    void* ptr;
    unsigned int len;

    if (lineptr == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (ferror(stream))
        return -1;

    if (feof(stream))
        return -1;

    fgets(line, 256, stream);

    ptr = strchr(line, '\n');
    if (ptr)
        *(char*)ptr = '\0';

    len = strlen(line);

    if ((len + 1) < 256) {
        ptr = realloc(*lineptr, 256);
        if (ptr == NULL)
            return (-1);
        *lineptr = (char*)ptr;
        *n = 256;
    }

    strcpy(*lineptr, line);
    return (len);
}