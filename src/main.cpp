/*
===============================================================================

  FILE:  main.cpp

  CONTENTS:

    This tool liberates LiDAR from enslavement by ESRI's closed and proprietary
    lock-in format that is generally known as the "LAZ clone". Read up on the
    controversy via the following links:

  PROGRAMMERS:

    martin.isenburg@rapidlasso.com  -  http://rapidlasso.com

  COPYRIGHT:

    (c) 2015, martin isenburg, rapidlasso - fast tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the LICENSE.txt file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  CHANGE HISTORY:

    25 September 2018 -- updated for compressing LAS 1.4 content
     3 April 2015 -- created to free LiDAR enslaved in ESRI's "LAZ clone"

===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lasliberate.h"

// lasliberate "D:/lastools_aaa/bin/ezlas/ezlas_lossless/LDR030828_213842_0.zlas" "D:/lastools_aaa/bin/ezlas/ezlas_lossless/LDR030828_213842_0.laz"

int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		const int cSize = strlen(argv[1]) + 1;
		wchar_t* file_name_in = new wchar_t[cSize];
		mbstowcs(file_name_in, argv[1], cSize);

		int rc = lasliberate(file_name_in, argv[2]);

		if (rc == 0)
		{
			fprintf(stderr, "SUCCESS: liberated '%s' from ESRI's closed format\n", argv[2]);
		}
		else
		{
			fprintf(stderr, "failed to liberate '%s' from ESRI's closed format\n", argv[2]);
		}

		delete file_name_in;

		return rc;
	}
	else
	{
		fprintf(stderr, "usage:\n");
		fprintf(stderr, "%s locked_up.zlas set_free.las\n", argv[0]);
		fprintf(stderr, "%s imprisoned.zlas liberated.laz\n", argv[0]);
		return 1;
	}
}
