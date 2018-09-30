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

    (c) 2015-2018, martin isenburg, rapidlasso - fast tools to catch reality

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

// Includes for the evil and proprietary lock-in "LAZ clone"

#include "EzLasAPIs.h"

// Includes for the liberating open source LASzip compressor

#include "laszip_api.h"

typedef struct laszip_evlr
{
	laszip_U16 reserved;
	laszip_CHAR user_id[16];
	laszip_U16 record_id;
	laszip_I64 record_length_after_header;
	laszip_CHAR description[32];
	laszip_U8* data;
} laszip_evlr_struct;

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
		fprintf(stderr, "Failed to create ESRI's proprietary lock-in format reader.\n");
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
		fprintf(stderr, "Cannot load liberating DLL. is DLL not in path? missing LASzip.dll? wrong DLL?\n");
		pLAZcloneReader->Release();
		return 1;
	}

	// Create the writer that will set the LiDAR free

	laszip_POINTER freedomWriter;
	if (laszip_create(&freedomWriter))
	{
		reportLASzipError(freedomWriter);
		fprintf(stderr, "Cannot create writer to set LiDAR data free\n");
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
	if (liberatedHeader.point_data_format > 5)
	{
		liberatedHeader.number_of_point_records = 0;
		for (i = 0; i < 5; i++) liberatedHeader.number_of_points_by_return[i] = 0;
	}
	else
	{
		liberatedHeader.number_of_point_records = imprisonedHeader.numberOfPointRecords1_3;
		for (i = 0; i < 5; i++) liberatedHeader.number_of_points_by_return[i] = imprisonedHeader.numberOfPointsByReturn1_3[i];
	}
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

	if (imprisonedHeader.startOfWaveformData)
	{
		fwprintf(stderr, L"WARNING: '%s' seems to contain waveform data. ignoring waveforms ...\n", file_name_in);
	}
	liberatedHeader.start_of_waveform_data_packet_record = 0;
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
				fprintf(stderr, "Imprisoned header size is %d instead of 227 for imprisoned LAS %d.%d\n", imprisonedHeader.headerSize, imprisonedHeader.verMajor, imprisonedHeader.verMinor);
				return 1;
			}
		}
		else if (imprisonedHeader.verMinor == 3)
		{
			if (imprisonedHeader.headerSize != 235)
			{
				fprintf(stderr, "Imprisoned header size is %d instead of 235 for imprisoned LAS %d.%d\n", imprisonedHeader.headerSize, imprisonedHeader.verMajor, imprisonedHeader.verMinor);
				return 1;
			}
		}
		else if (imprisonedHeader.verMinor == 4)
		{
			if (imprisonedHeader.headerSize != 375)
			{
				fprintf(stderr, "Imprisoned header size is %d instead of 375 for imprisoned LAS %d.%d\n", imprisonedHeader.headerSize, imprisonedHeader.verMajor, imprisonedHeader.verMinor);
				return 1;
			}
		}
		else
		{
			fprintf(stderr, "Imprisoned minor version of LAS %d.%d not known\n", imprisonedHeader.verMajor, imprisonedHeader.verMinor);
		}
	}
	else
	{
		fprintf(stderr, "Imprisoned major version of LAS %d.%d not known\n", imprisonedHeader.verMajor, imprisonedHeader.verMinor);
		return 1;
	}

	// Create name of temp file to write VLRs, EVLRs and user data after header into

	const int cSize = strlen(file_name_out) + 1;
	wchar_t* file_name_temp = new wchar_t[cSize];
	if (file_name_temp == 0)
	{
		fprintf(stderr, "Failed to allocate wchar_t[%d] for temp file name '%s'\n", cSize, file_name_out);
		return 1;
	}
	mbstowcs(file_name_temp, file_name_out, cSize);

	// Liberate the VLRs and the user data after header

	if (imprisonedHeader.headerSize < imprisonedHeader.offsetToData)
	{
		// Write VLRs and user data after header to temp file (no direct access via "LAZ clone" API)

		unsigned long recordCount = 0;
		UINT64 dataSize = 0;
		hr = pLAZcloneReader->SaveVLRs(file_name_temp, recordCount, dataSize);
		if (FAILED(hr))
		{
			reportLAZcloneError(pLAZcloneReader);
			fprintf(stderr, "Failed to save VLRs and user data after header from \"LAZ clone\" to temp file '%s'.\n", file_name_out);
			return hr;
		}

		// Make sure all the missing data between header and first point record is there

		if (imprisonedHeader.offsetToData != (dataSize + imprisonedHeader.headerSize))
		{
			fprintf(stderr, "Imprisoned header's offset to point data of %u does not match its header size of %u plus its VLRs and user data after header size of %u\n", imprisonedHeader.offsetToData, imprisonedHeader.headerSize, (laszip_U32)dataSize);
			return 1;
		}

		// Open temporary file for read

		FILE* file_temp = fopen(file_name_out, "rb");
		if (file_temp == 0)
		{
			fprintf(stderr, "Cannot open temp file '%s' with VLRs and user data after header liberated from \"LAZ clone\"", file_name_out);
			return 1;
		}

		UINT64 dataTotal = 0;
		if (imprisonedHeader.numberOfVLRs)
		{
			liberatedHeader.vlrs = (laszip_vlr*)malloc(sizeof(laszip_vlr)*imprisonedHeader.numberOfVLRs);
			if (liberatedHeader.vlrs == 0)
			{
				fprintf(stderr, "Failed to allocate laszip_vlr[%d] for liberating VLRs\n", imprisonedHeader.numberOfVLRs);
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
						fprintf(stderr, "Failed to allocate payload of %u bytes for VLR %d while liberating %d VLRs\n", liberatedHeader.vlrs[i].record_length_after_header, i, imprisonedHeader.numberOfVLRs);
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
				fprintf(stderr, "Failed to allocate %u user data after header while liberating the LAS header\n", liberatedHeader.user_data_after_header_size);
				return 1;
			}
			fread(liberatedHeader.user_data_after_header, 1, liberatedHeader.user_data_after_header_size, file_temp);
		}
		else if (dataTotal > dataSize)
		{
			fprintf(stderr, "Found corrupted %I64u instead of %I64u bytes while liberating %d VLRs\n", dataTotal, dataSize, imprisonedHeader.numberOfVLRs);
			return 1;
		}

		fclose(file_temp);
		file_temp = 0;

		// set header

		if (laszip_set_header(freedomWriter, &liberatedHeader))
		{
			reportLASzipError(freedomWriter);
			fprintf(stderr, "DLL ERROR: setting liberated LAS header\n");
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

	// Liberate the EVLRs

	if (imprisonedHeader.numberOfEVLRs)
	{
		// Write EVLRs to a temp file (no direct access via "LAZ clone" API)

		UINT64 dataTotal = 0;

		unsigned long recordCount = 0;
		UINT64 dataSize = 0;
		hr = pLAZcloneReader->SaveExtendedVLRs(file_name_temp, recordCount, dataSize);
		if (FAILED(hr))
		{
			reportLAZcloneError(pLAZcloneReader);
			fprintf(stderr, "Failed to save EVLRs from \"LAZ clone\" to temp file '%s'.\n", file_name_out);
			return hr;
		}

		// Open temporary file for read

		FILE* file_temp = fopen(file_name_out, "rb");
		if (file_temp == 0)
		{
			fprintf(stderr, "Cannot open temp file '%s' with EVLRs liberated from \"LAZ clone\"", file_name_out);
			return 1;
		}

		laszip_evlr evlr;
		for (i = 0; i < imprisonedHeader.numberOfEVLRs; i++)
		{
			fread(&evlr, 60, 1, file_temp);
			if (evlr.record_length_after_header)
			{
				evlr.data = new laszip_U8[(laszip_U32)(evlr.record_length_after_header)];
				if (evlr.data == 0)
				{
					fprintf(stderr, "Failed to allocate payload of %I64d bytes for EVLR %d while liberating %d EVLRs\n", evlr.record_length_after_header, i, imprisonedHeader.numberOfEVLRs);
					return 1;
				}
				fread(evlr.data, 1, (laszip_U32)(evlr.record_length_after_header), file_temp);
			}
			else
			{
				evlr.data = 0;
			}

			if (evlr.record_length_after_header > 65535)
			{
				fprintf(stderr, "WARNING: skipping EVLR '%.16s' with payload of %I64d while liberating %d EVLRs\n", evlr.user_id, evlr.record_length_after_header, imprisonedHeader.numberOfEVLRs);
			}
			else
			{
				if (laszip_add_vlr(freedomWriter, evlr.user_id, evlr.record_id, (laszip_U16)evlr.record_length_after_header, evlr.description, evlr.data))
				{
					reportLASzipError(freedomWriter);
					fprintf(stderr, "DLL ERROR: adding small payload EVLR as VLR\n");
					return 1;
				}
			}
			if (evlr.data)
			{
				delete [] evlr.data;
			}
		}

		fclose(file_temp);
		file_temp = 0;
	}

	// delete the temporary file name

	delete[] file_name_temp;
	file_name_temp = 0;
	
	// Enable the creation of spatial indices with open source LAX

	laszip_BOOL create = 1;
	laszip_BOOL append = 0; /* not supported yet */

	if (laszip_create_spatial_index(freedomWriter, create, append))
	{
		reportLASzipError(freedomWriter);
		fprintf(stderr, "DLL ERROR: signaling laszip writer to create spatial indexing information\n");
		return 1;
	}

	// Tell the freedom writer to preserve the "generating software" string 

	laszip_BOOL preserve = 1;

	if (laszip_preserve_generating_software(freedomWriter, preserve))
	{
		reportLASzipError(freedomWriter);
		fprintf(stderr, "DLL ERROR : preserving generating software string in laszip writer\n");
		return 1;
	}

	// Open the freedom writer that will ultimately set the LAS file free 

	int len = strlen(file_name_out);
	laszip_BOOL compress = ((file_name_out[len - 1] == 'z') || (file_name_out[len - 1] == 'Z'));

	if (laszip_open_writer(freedomWriter, file_name_out, compress))
	{
		reportLASzipError(freedomWriter);
		fwprintf(stderr, L"DLL ERROR : opening laszip writer for '%hs'\n", file_name_out);
		return 1;
	}

	// Get a pointer to a structure that will be populated with liberated LiDAR data

	laszip_point* liberatedPoint;

	if (laszip_get_point_pointer(freedomWriter, &liberatedPoint))
	{
		reportLASzipError(freedomWriter);
		fprintf(stderr, "DLL ERROR: getting point pointer from laszip writer\n");
		return 1;
	}

	// Liberate the LiDAR points in chunks of 8192

	static const long liberationIntervalSize = 8192;
	long currentLiberation = liberationIntervalSize;

	// Special handling for setting free any imprisoned extra bytes

	char* imprisonedExtraBytes = 0;
	long imprisonedExtraByteSize = imprisonedHeader.pointDataExtraBytesSize;

	if (imprisonedExtraByteSize)
	{
		imprisonedExtraBytes = new char[imprisonedExtraByteSize * liberationIntervalSize];
		if (imprisonedExtraBytes)
		{
			fprintf(stderr, "Failed to allocate %u bytes for liberating %u extra bytes per point\n", imprisonedExtraByteSize * liberationIntervalSize, imprisonedExtraByteSize);
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
			liberatedPoint->number_of_returns = (imprisonedPoints[liberationID].numberOfReturns & 7);
			liberatedPoint->scan_direction_flag = imprisonedPoints[liberationID].scanDirectionFlag;
			liberatedPoint->edge_of_flight_line = imprisonedPoints[liberationID].edgeOfFlightLine;
			if (imprisonedPoints[liberationID].classCode > 31)
			{
				liberatedPoint->classification = 0;
			}
			else
			{
				liberatedPoint->classification = imprisonedPoints[liberationID].classCode;
			}

			liberatedPoint->synthetic_flag = imprisonedPoints[liberationID].bIsSyntheticPoint;
			liberatedPoint->keypoint_flag = imprisonedPoints[liberationID].bIsKeyPoint;
			liberatedPoint->withheld_flag = imprisonedPoints[liberationID].bIsWithheld;
			liberatedPoint->scan_angle_rank = laszip_I8_QUANTIZE(imprisonedPoints[liberationID].scanAngle);
			liberatedPoint->user_data = imprisonedPoints[liberationID].userData;
			liberatedPoint->point_source_ID = imprisonedPoints[liberationID].pointSourceID;
			liberatedPoint->extended_scan_angle = laszip_I16_QUANTIZE(imprisonedPoints[liberationID].scanAngle / 0.006);
			liberatedPoint->extended_scanner_channel = imprisonedPoints[liberationID].scannerChannel;
			liberatedPoint->extended_classification_flags = ((((laszip_U8)imprisonedPoints[liberationID].bIsOverlapPoint) << 3) | (((laszip_U8)imprisonedPoints[liberationID].bIsWithheld) << 2) | (((laszip_U8)imprisonedPoints[liberationID].bIsKeyPoint) << 1) | ((laszip_U8)imprisonedPoints[liberationID].bIsSyntheticPoint));
			liberatedPoint->extended_classification = imprisonedPoints[liberationID].classCode;
			liberatedPoint->extended_return_number = imprisonedPoints[liberationID].returnNumber;
			liberatedPoint->extended_number_of_returns = imprisonedPoints[liberationID].numberOfReturns;
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
				fprintf(stderr, "DLL ERROR: writing liberated point %I64d\n", count);
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
		fprintf(stderr, "DLL ERROR: closing laszip writer\n");
		return 1;
	}

	// destroy the freedom writer now that liberty of LiDAR was restored

	if (laszip_destroy(freedomWriter))
	{
		fprintf(stderr, "DLL ERROR: destroying laszip writer\n");
		return 1;
	}

	// unload the liberating DLL

	if (laszip_unload_dll())
	{
		fprintf(stderr, "DLL ERROR: failed to unload liberating DLL\n");
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
