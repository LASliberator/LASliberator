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
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>

#include "lasliberate.h"

// lasliberate "D:/lastools_aaa/bin/ezlas/ezlas_lossless/LDR030828_213842_0.zlas" "D:/lastools_aaa/bin/ezlas/ezlas_lossless/LDR030828_213842_0.laz"

static void bad_first_argument(const char* argument)
{
	fprintf(stderr, "ERROR: first argument needs to be either the file name of locked up points\n");
	fprintf(stderr, "       such as 'closed.zlas' or the name of a directory containing multiple\n");
	fprintf(stderr, "       miserable files such as 'C:\\pointclouds\\enslaved_LiDAR', but the given\n");
	fprintf(stderr, "       argument '%s' is neither.\n", argument);
}

static void bad_second_argument(const char* argument)
{
	fprintf(stderr, "ERROR: if the first argument is the name of a directory containing multiple\n");
	fprintf(stderr, "       miserable files such as 'C:\\pointclouds\\enslaved_LiDAR' then the second\n");
	fprintf(stderr, "       argument must be the name of an existing directory where the free points\n");
	fprintf(stderr, "       will be stored, but the given argument '%s' is not.\n", argument);
}

int main(int argc, char* argv[])
{
	if ((argc == 2) || (argc == 3))
	{
		struct stat info;

		// does input file or directory exist?

		if (stat(argv[1], &info) != 0)
		{
			bad_first_argument(argv[1]);
			return 1;
		}

		// is it a file or a directory?

		if (info.st_mode & S_IFDIR)
		{
			// it is a directory.

			char file_name_out[512];
			size_t file_name_out_base_len = 0;

			if (argc == 3)
			{
				// then second argument must be directory too

				if (stat(argv[2], &info) != 0)
				{
					bad_second_argument(argv[2]);
					return 1;
				}

				// copy the directory name as the base of the output file name

				strcpy(file_name_out, argv[2]);
				file_name_out_base_len = strlen(file_name_out);
				if ((file_name_out[file_name_out_base_len - 1] == '\\') || (file_name_out[file_name_out_base_len - 1] == '/')) file_name_out_base_len--;
				file_name_out[file_name_out_base_len] = '\\';
				file_name_out_base_len++;
			}

			int processed_files = 0;
			int failed_files = 0;
			HANDLE h;
			WIN32_FIND_DATA info;

			size_t len = strlen(argv[1]);
			if ((argv[1][len-1] == '\\') || (argv[1][len-1] == '/')) len--;

			wchar_t* file_name = new wchar_t[len+10];
			mbstowcs(file_name, argv[1], len);
			file_name[len] = '\\';
			file_name[len + 1] = '*';
			file_name[len + 2] = '.';
			file_name[len + 3] = 'z';
			file_name[len + 4] = 'l';
			file_name[len + 5] = 'a';
			file_name[len + 6] = 's';
			file_name[len + 7] = '\0';

			wchar_t file_name_in[512];
			h = FindFirstFile(file_name, &info);
			if (h != INVALID_HANDLE_VALUE)
			{
				mbstowcs(file_name_in, argv[1], len);
				file_name_in[len] = '\\';
				do
				{
					swprintf(&file_name_in[len + 1], L"%ws", info.cFileName);
					if (argc == 3)
					{
						wcstombs(&file_name_out[file_name_out_base_len], &file_name_in[len + 1], 512);
					}
					else
					{
						wcstombs(file_name_out, file_name_in, 512);
					}
					size_t len1 = strlen(file_name_out);
					while ((len1 > 0) && (file_name_out[len1] != '.')) len1--;
					file_name_out[len1 + 1] = 'l';
					file_name_out[len1 + 2] = 'a';
					file_name_out[len1 + 3] = 'z';
					file_name_out[len1 + 4] = '\0';
					
					int rc = lasliberate(file_name_in, file_name_out);

					if (rc == 0)
					{
						fprintf(stderr, "SUCCESS: liberated '%s' from ESRI's closed format\n", file_name_out);
						processed_files++;
					}
					else
					{
						fprintf(stderr, "failed to liberate '%s' from ESRI's closed format\n", file_name_out);
						failed_files++;
					}
				} while (FindNextFile(h, &info));
				FindClose(h);
			}
			return (processed_files ? (failed_files ? 1 : 0) : 1);
		}
		else
		{
			// is it a file.

			size_t len = strlen(argv[1]) + 1;
			wchar_t* file_name_in = new wchar_t[len];
			mbstowcs(file_name_in, argv[1], len);

			// create output name if none was specified

			char* file_name_out = 0;

			if (argc == 2)
			{
				file_name_out = _strdup(argv[1]);
				size_t len = strlen(file_name_out);
				while ((len > 0) && (file_name_out[len] != '.')) len--;
				if (len == 0)
				{
					bad_first_argument(argv[1]);
					return 1;
				}
				file_name_out[len + 1] = 'l';
				file_name_out[len + 2] = 'a';
				file_name_out[len + 3] = 'z';
				file_name_out[len + 4] = '\0';
			}
			else
			{
				file_name_out = argv[2];
			}
			int rc = lasliberate(file_name_in, file_name_out);

			if (rc == 0)
			{
				fprintf(stderr, "SUCCESS: liberated '%s' from ESRI's closed format\n", file_name_out);
			}
			else
			{
				fprintf(stderr, "failed to liberate '%s' from ESRI's closed format\n", file_name_out);
			}

			delete file_name_in;
			if (argc == 2) free(file_name_out);
			return rc;
		}
	}
	else
	{
		fprintf(stderr, "usage:\n");
		fprintf(stderr, "%s lidar.zlas\n", argv[0]);
		fprintf(stderr, "%s locked_up.zlas set_free.las\n", argv[0]);
		fprintf(stderr, "%s imprisoned.zlas liberated.laz\n", argv[0]);
		fprintf(stderr, "%s C:\\LiDAR_data\\imprisoned\n", argv[0]);
		fprintf(stderr, "%s C:\\LiDAR_data\\enslaved C:\\LiDAR_data\\freed\n" , argv[0]);
		return 1;
	}
}
