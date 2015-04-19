/*
===============================================================================

  FILE:  lasliberate.cpp

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

    see header file

===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Includes for the evil and proprietary lock-in "LAZ clone"

#include "EzLasAPIs.h"

// Includes for the liberating open source LASzip compressor

#include "laszip_dll.h"

// Conversion macros

#define laszip_I8_QUANTIZE(n) (((n) >= 0) ? (laszip_I8)((n)+0.5f) : (laszip_I8)((n)-0.5f))
#define laszip_I16_QUANTIZE(n) (((n) >= 0) ? (laszip_I16)((n)+0.5f) : (laszip_I16)((n)-0.5f))
#define laszip_I32_QUANTIZE(n) (((n) >= 0) ? (laszip_I32)((n)+0.5f) : (laszip_I32)((n)-0.5f))

// DLL error reporting functions

static void reportLAZcloneError(iEzLasObject* pObject)
{
	if (pObject)
	{
		wchar_t* errInfo = NULL;
		pObject->AccessErrorDescription(&errInfo);
		fwprintf(stderr, L"LAZ clone DLL ERROR: %s\n", errInfo);
	}
}

static void reportLASzipError(laszip_POINTER laszip)
{
	if (laszip)
	{
		laszip_CHAR* error;
		if (laszip_get_error(laszip, &error))
		{
			fprintf(stderr, "LASzip DLL ERROR: getting error messages\n");
		}
		fprintf(stderr, "LASzip DLL ERROR MESSAGE: %s\n", error);
	}
}

// Function doing the actual liberation work

int lasliberate(const wchar_t* file_name_in, const char* file_name_out)
{
	unsigned long i;

	// Create "LAZ clone" reader and use proprietary ESRI interface to liberate LAS.

	iEzLasReader* pLAZcloneReader = NULL;

	HRESULT hr = EzLasObjectFactory::CreateObject(EzLasOID_Reader, EzLasIID_Reader, (void**)&pLAZcloneReader);
	if (FAILED(hr))
	{
		fwprintf(stderr, L"Failed to create ESRI's proprietary lock-in format reader.\n");
		return hr;
	}

	// Initialize the "LAZ clone" reader

	hr = pLAZcloneReader->Init(file_name_in, 100);
	if (FAILED(hr))
	{
		reportLAZcloneError(pLAZcloneReader);
		fwprintf(stderr, L"Failed to init ESRI's proprietary lock-in format reader for file '%ws'.\n", file_name_in);
		pLAZcloneReader->Release();
		return hr;
	}

	// Get header of imprisoned LAS file

	EzLasHeader imprisonedHeader;
	hr = pLAZcloneReader->GetLasHeader(imprisonedHeader);
	if (FAILED(hr))
	{
		reportLAZcloneError(pLAZcloneReader);
		fwprintf(stderr, L"Failed to get locked-up LAS header from  ESRI's proprietary format for file '%ws'.\n", file_name_in);
		pLAZcloneReader->Release();
		return hr;
	}

	// Load the liberating DLL

	if (laszip_load_dll())
	{
		fwprintf(stderr, L"Cannot load liberating DLL. is DLL not in path? missing LASzip.dll? wrong DLL?\n");
		pLAZcloneReader->Release();
		return 1;
	}

	// Create the writer that will set the LiDAR free

	laszip_POINTER freedomWriter;
	if (laszip_create(&freedomWriter))
	{
		reportLASzipError(freedomWriter);
		fwprintf(stderr, L"Cannot create writer to set LiDAR data free\n");
		return 1;
	}

	//  Create, initialize, populate, and  liberate the LAS header data

	laszip_header liberatedHeader;

	liberatedHeader.global_encoding = imprisonedHeader.globalEncoding;
	liberatedHeader.file_source_ID = imprisonedHeader.fileSourceID;
	liberatedHeader.project_ID_GUID_data_1 = imprisonedHeader.guid1;
	liberatedHeader.project_ID_GUID_data_2 = imprisonedHeader.guid2;
	liberatedHeader.project_ID_GUID_data_3 = imprisonedHeader.guid3;
	memcpy(liberatedHeader.project_ID_GUID_data_4, imprisonedHeader.guid4, 8);
	liberatedHeader.version_major = imprisonedHeader.verMajor;
	liberatedHeader.version_minor = imprisonedHeader.verMinor;
	memset(liberatedHeader.system_identifier, 0, 32);
	memcpy(liberatedHeader.system_identifier, imprisonedHeader.systemID, 32);
	memset(liberatedHeader.generating_software, 0, 32);
	memcpy(liberatedHeader.generating_software, imprisonedHeader.generatingSoftware, 32);
	liberatedHeader.file_creation_day = imprisonedHeader.fileCreationDayOfYear;
	liberatedHeader.file_creation_year = imprisonedHeader.fileCreationYear;
	liberatedHeader.header_size = imprisonedHeader.headerSize;
	liberatedHeader.offset_to_point_data = imprisonedHeader.offsetToData;
	liberatedHeader.number_of_variable_length_records = imprisonedHeader.numberOfVLRs;
	liberatedHeader.point_data_format = imprisonedHeader.pointDataFormatID;
	liberatedHeader.point_data_record_length = imprisonedHeader.pointDataRecordLength;
	liberatedHeader.number_of_point_records = imprisonedHeader.numberOfPointRecords1_3;
	for (i = 0; i < 5; i++) liberatedHeader.number_of_points_by_return[i] = imprisonedHeader.numberOfPointsByReturn1_3[i];
	liberatedHeader.x_scale_factor = imprisonedHeader.xScaleFactor;
	liberatedHeader.y_scale_factor = imprisonedHeader.yScaleFactor;
	liberatedHeader.z_scale_factor = imprisonedHeader.zScaleFactor;
	liberatedHeader.x_offset = imprisonedHeader.xOffset;
	liberatedHeader.y_offset = imprisonedHeader.yOffset;
	liberatedHeader.z_offset = imprisonedHeader.zOffset;
	liberatedHeader.max_x = imprisonedHeader.xMax;
	liberatedHeader.min_x = imprisonedHeader.xMin;
	liberatedHeader.max_y = imprisonedHeader.yMax;
	liberatedHeader.min_y = imprisonedHeader.yMin;
	liberatedHeader.max_z = imprisonedHeader.zMax;
	liberatedHeader.min_z = imprisonedHeader.zMin;

	liberatedHeader.start_of_waveform_data_packet_record = imprisonedHeader.startOfWaveformData;
	liberatedHeader.start_of_first_extended_variable_length_record = imprisonedHeader.startOfFirstEVLR;
	liberatedHeader.number_of_extended_variable_length_records = imprisonedHeader.numberOfEVLRs;
	liberatedHeader.extended_number_of_point_records = imprisonedHeader.numberOfPointRecords1_4;
	for (i = 0; i < 15; i++) liberatedHeader.extended_number_of_points_by_return[i] = imprisonedHeader.numberOfPointsByReturn1_4[i];

	// Not sure if and where the "LAZ clone" maintains user data that is stored in the header

	liberatedHeader.user_data_in_header_size = 0;
	liberatedHeader.user_data_in_header = 0;

	// Therefore we need to make sure the header size is LAS specification conform

	if (imprisonedHeader.verMajor == 1)
	{
		if (imprisonedHeader.verMinor <= 2)
		{
			if (imprisonedHeader.headerSize != 227)
			{
				fwprintf(stderr, L"Imprisoned header size is %d instead of 227 for imprisoned LAS %d.%d\n", imprisonedHeader.headerSize, imprisonedHeader.verMajor, imprisonedHeader.verMinor);
				return 1;
			}
		}
		else if (imprisonedHeader.verMinor == 3)
		{
			if (imprisonedHeader.headerSize != 235)
			{
				fwprintf(stderr, L"Imprisoned header size is %d instead of 235 for imprisoned LAS %d.%d\n", imprisonedHeader.headerSize, imprisonedHeader.verMajor, imprisonedHeader.verMinor);
				return 1;
			}
		}
		else if (imprisonedHeader.verMinor == 4)
		{
			if (imprisonedHeader.headerSize != 375)
			{
				fwprintf(stderr, L"Imprisoned header size is %d instead of 375 for imprisoned LAS %d.%d\n", imprisonedHeader.headerSize, imprisonedHeader.verMajor, imprisonedHeader.verMinor);
				return 1;
			}
		}
		else
		{
			fwprintf(stderr, L"Imprisoned minor version of LAS %d.%d not known\n", imprisonedHeader.verMajor, imprisonedHeader.verMinor);
		}
	}
	else
	{
		fwprintf(stderr, L"Imprisoned major version of LAS %d.%d not known\n", imprisonedHeader.verMajor, imprisonedHeader.verMinor);
		return 1;
	}

	// Liberate the VLRs and the user data after header

	if (imprisonedHeader.headerSize < imprisonedHeader.offsetToData)
	{
		// Create name of temp file to write VLRs and user data after header into

		const int cSize = strlen(file_name_out) + 1;
		wchar_t* file_name_temp = new wchar_t[cSize];
		if (file_name_temp == 0)
		{
			fwprintf(stderr, L"Failed to allocate wchar_t[%d] for temp file name '%s'\n", cSize, file_name_out);
			return 1;
		}
		mbstowcs(file_name_temp, file_name_out, cSize);

		// Write VLRs and user data after header to temp file (no direct access via "LAZ clone" API)

		unsigned long recordCount = 0;
		UINT64 dataSize = 0;
		hr = pLAZcloneReader->SaveVLRs(file_name_temp, recordCount, dataSize);
		delete[] file_name_temp;
		if (FAILED(hr))
		{
			reportLAZcloneError(pLAZcloneReader);
			fwprintf(stderr, L"Failed to save VLRs and user data after header from \"LAZ clone\" to temp file '%s'.\n", file_name_out);
			return hr;
		}

		// Make sure all the missing data between header and first point record is there

		if (imprisonedHeader.offsetToData != (dataSize + imprisonedHeader.headerSize))
		{
			fwprintf(stderr, L"Imprisoned header's offset to point data of %u does not match its header size of %u plus its VLRs and user data after header size of %u\n", imprisonedHeader.offsetToData, imprisonedHeader.headerSize, (laszip_U32)dataSize);
			return 1;
		}

		// Open temporary file for read

		FILE* file_temp = fopen(file_name_out, "rb");
		if (file_temp == 0)
		{
			fwprintf(stderr, L"Cannot open temp file '%s' with VLRs and user data after header liberated from \"LAZ clone\"");
			return 1;
		}

		UINT64 dataTotal = 0;
		if (imprisonedHeader.numberOfVLRs)
		{
			liberatedHeader.vlrs = (laszip_vlr*)malloc(sizeof(laszip_vlr)*imprisonedHeader.numberOfVLRs);
			if (liberatedHeader.vlrs == 0)
			{
				fwprintf(stderr, L"Failed to allocate laszip_vlr[%d] for liberating VLRs\n", imprisonedHeader.numberOfVLRs);
				return 1;
			}
			for (i = 0; i < imprisonedHeader.numberOfVLRs; i++)
			{
				fread(&(liberatedHeader.vlrs[i]), 54, 1, file_temp);
				if (liberatedHeader.vlrs[i].record_length_after_header)
				{
					liberatedHeader.vlrs[i].data = new laszip_U8[liberatedHeader.vlrs[i].record_length_after_header];
					if (liberatedHeader.vlrs[i].data == 0)
					{
						fwprintf(stderr, L"Failed to allocate payload of %u bytes for VLR %d while liberating %d VLRs\n", liberatedHeader.vlrs[i].record_length_after_header, i, imprisonedHeader.numberOfVLRs);
						return 1;
					}
					fread(liberatedHeader.vlrs[i].data, 1, liberatedHeader.vlrs[i].record_length_after_header, file_temp);
				}
				else
				{
					liberatedHeader.vlrs[i].data = 0;
				}

				dataTotal += (54 + liberatedHeader.vlrs[i].record_length_after_header);
			}
		}

		if (dataTotal < dataSize)
		{
			liberatedHeader.user_data_after_header_size = laszip_U32(dataSize - dataTotal);
			liberatedHeader.user_data_after_header = new laszip_U8[liberatedHeader.user_data_after_header_size];
			if (liberatedHeader.user_data_after_header == 0)
			{
				fwprintf(stderr, L"Failed to allocate %u user data after header while liberating the LAS header\n", liberatedHeader.user_data_after_header_size);
				return 1;
			}
			fread(liberatedHeader.user_data_after_header, 1, liberatedHeader.user_data_after_header_size, file_temp);
		}
		else if (dataTotal > dataSize)
		{
			fwprintf(stderr, L"Found corrupted %u instead of %u bytes while liberating %d VLRs\n", dataTotal, dataSize, imprisonedHeader.numberOfVLRs);
			return 1;
		}

		fclose(file_temp);
		file_temp = 0;

		if (laszip_set_header(freedomWriter, &liberatedHeader))
		{
			reportLASzipError(freedomWriter);
			fwprintf(stderr, L"DLL ERROR: setting liberated LAS header\n");
			return 1;
		}

		// Free all temporary allocated header memory

		if (imprisonedHeader.numberOfVLRs)
		{
			for (i = 0; i < imprisonedHeader.numberOfVLRs; i++)
			{
				if (liberatedHeader.vlrs[i].record_length_after_header)
				{
					delete[] liberatedHeader.vlrs[i].data;
					liberatedHeader.vlrs[i].data = 0;
				}
			}
			free(liberatedHeader.vlrs);
			liberatedHeader.vlrs = 0;
		}

		if (liberatedHeader.user_data_after_header_size)
		{
			delete[] liberatedHeader.user_data_after_header;
			liberatedHeader.user_data_after_header = 0;
		}
	}

	// Enable the creation of spatial indices with open source LAX

	laszip_BOOL create = 1;
	laszip_BOOL append = 0; /* not supported yet */

	if (laszip_create_spatial_index(freedomWriter, create, append))
	{
		reportLASzipError(freedomWriter);
		fwprintf(stderr, L"DLL ERROR: signaling laszip writer to create spatial indexing information\n");
		return 1;
	}

	// Tell the freedom writer to preserve the "generating software" string 

	laszip_BOOL preserve = 1;

	if (laszip_preserve_generating_software(freedomWriter, preserve))
	{
		reportLASzipError(freedomWriter);
		fwprintf(stderr, L"DLL ERROR : preserving generating software string in laszip writer\n");
		return 1;
	}

	// Open the freedom writer that will ultimately set the LAS file free 

	int len = strlen(file_name_out);
	laszip_BOOL compress = ((file_name_out[len - 1] == 'z') || (file_name_out[len - 1] == 'Z'));

	if (laszip_open_writer(freedomWriter, file_name_out, compress))
	{
		reportLASzipError(freedomWriter);
		fwprintf(stderr, L"DLL ERROR : opening laszip writer for '%s'\n", file_name_out);
		return 1;
	}

	// Get a pointer to a structure that will be populated with liberated LiDAR data

	laszip_point* liberatedPoint;

	if (laszip_get_point_pointer(freedomWriter, &liberatedPoint))
	{
		reportLASzipError(freedomWriter);
		fwprintf(stderr, L"DLL ERROR: getting point pointer from laszip writer\n");
		return 1;
	}

	// Liberate the LiDAR points in chunks of 1024

	static const long liberationIntervalSize = 1024;
	long currentLiberation = liberationIntervalSize;

	// Special handling for setting free any imprisoned extra bytes

	char* imprisonedExtraBytes = 0;
	long imprisonedExtraByteSize = imprisonedHeader.pointDataExtraBytesSize;

	if (imprisonedExtraByteSize)
	{
		imprisonedExtraBytes = new char[imprisonedExtraByteSize * liberationIntervalSize];
		if (imprisonedExtraBytes)
		{
			fwprintf(stderr, L"Failed to allocate %u bytes for liberating %u extra bytes per point\n", imprisonedExtraByteSize * liberationIntervalSize, imprisonedExtraByteSize);
		}
	}

	UINT32 pointDataMask = EzLasPointAll;
	EzLasPointInfo imprisonedPoints[liberationIntervalSize];

	// Keeps track of the progess of LiDAR liberation

	UINT64 freedomProgress = 1;
	UINT64 freedomGoal = imprisonedHeader.numberOfPointRecords1_3;
	if (freedomGoal == 0)
	{
		freedomGoal = imprisonedHeader.numberOfPointRecords1_4;
	}

	laszip_I64 count = 0;

	// liberate LAS points

	while (freedomProgress <= freedomGoal)
	{
		if ((freedomGoal - freedomProgress + 1) < liberationIntervalSize)
		{
			currentLiberation = (long)(freedomGoal - freedomProgress + 1);
		}

		// read from lock-in format (aka "LAZ clone")

		hr = pLAZcloneReader->QueryPoints(currentLiberation, freedomProgress, pointDataMask, imprisonedPoints, imprisonedExtraBytes);
		if (FAILED(hr))
		{
			reportLAZcloneError(pLAZcloneReader);
			pLAZcloneReader->Release();
			return hr;
		}

		// liberate to open format (aka LAS or LAZ)

		UINT64 liberationID;

		for (liberationID = 0; liberationID < currentLiberation; liberationID++)
		{
			liberatedPoint->X = laszip_I32_QUANTIZE((imprisonedPoints[liberationID].x - imprisonedHeader.xOffset) / imprisonedHeader.xScaleFactor);
			liberatedPoint->Y = laszip_I32_QUANTIZE((imprisonedPoints[liberationID].y - imprisonedHeader.yOffset) / imprisonedHeader.yScaleFactor);
			liberatedPoint->Z = laszip_I32_QUANTIZE((imprisonedPoints[liberationID].z - imprisonedHeader.zOffset) / imprisonedHeader.zScaleFactor);
			liberatedPoint->intensity = imprisonedPoints[liberationID].intensity;
			liberatedPoint->return_number = (imprisonedPoints[liberationID].returnNumber & 7);
			liberatedPoint->number_of_returns_of_given_pulse = (imprisonedPoints[liberationID].numberOfReturns & 7);
			liberatedPoint->scan_direction_flag = imprisonedPoints[liberationID].scanDirectionFlag;
			liberatedPoint->edge_of_flight_line = imprisonedPoints[liberationID].edgeOfFlightLine;
			liberatedPoint->classification = (imprisonedPoints[liberationID].classCode & 31);
			liberatedPoint->synthetic_flag = imprisonedPoints[liberationID].bIsSyntheticPoint;
			liberatedPoint->keypoint_flag = imprisonedPoints[liberationID].bIsKeyPoint;
			liberatedPoint->withheld_flag = imprisonedPoints[liberationID].bIsWithheld;
			liberatedPoint->scan_angle_rank = laszip_I8_QUANTIZE(imprisonedPoints[liberationID].scanAngle);
			liberatedPoint->user_data = imprisonedPoints[liberationID].userData;
			liberatedPoint->point_source_ID = imprisonedPoints[liberationID].pointSourceID;
			liberatedPoint->extended_scan_angle = laszip_I16_QUANTIZE(imprisonedPoints[liberationID].scanAngle / 0.006);
			liberatedPoint->extended_scanner_channel = imprisonedPoints[liberationID].scannerChannel;
			liberatedPoint->extended_classification_flags = ((laszip_U8)imprisonedPoints[liberationID].bIsOverlapPoint) << 3;
			liberatedPoint->extended_classification = imprisonedPoints[liberationID].classCode;
			liberatedPoint->extended_return_number = imprisonedPoints[liberationID].returnNumber;
			liberatedPoint->extended_number_of_returns_of_given_pulse = imprisonedPoints[liberationID].numberOfReturns;
			liberatedPoint->gps_time = imprisonedPoints[liberationID].gpsTime;
			liberatedPoint->rgb[0] = imprisonedPoints[liberationID].red;
			liberatedPoint->rgb[1] = imprisonedPoints[liberationID].green;
			liberatedPoint->rgb[2] = imprisonedPoints[liberationID].blue;
			liberatedPoint->rgb[3] = imprisonedPoints[liberationID].nir;
			if (imprisonedExtraBytes)
			{
				memcpy(liberatedPoint->extra_bytes, &(imprisonedExtraBytes[liberationID*imprisonedExtraByteSize]), imprisonedExtraByteSize);
			}

			if (laszip_write_indexed_point(freedomWriter))
			{
				reportLASzipError(freedomWriter);
				fwprintf(stderr, L"DLL ERROR: writing liberated point %u\n", (laszip_U32)count);
				return 1;
			}

			count++;
		}

		freedomProgress += currentLiberation;
	}

	// close the freedom writer

	if (laszip_close_writer(freedomWriter))
	{
		reportLASzipError(freedomWriter);
		fwprintf(stderr, L"DLL ERROR: closing laszip writer\n");
		return 1;
	}

	// destroy the freedom writer now that liberty of LiDAR was restored

	if (laszip_destroy(freedomWriter))
	{
		fwprintf(stderr, L"DLL ERROR: destroying laszip writer\n");
		return 1;
	}

	// unload the liberating DLL

	if (laszip_unload_dll())
	{
		fwprintf(stderr, L"DLL ERROR: failed to unload liberating DLL\n");
		return 1;
	}

	// Maybe release memory for (now liberated) extra bytes.

	if (imprisonedExtraBytes)
	{
		delete[] imprisonedExtraBytes;
	}

	// Release the "LAZ clone" reader.

	hr = pLAZcloneReader->Release();

	return hr;
};

