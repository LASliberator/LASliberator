// EzLasAPIs.h

/**********************************************
Copyright 2015 Esri
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
either express or implied. See the License for the specific
language governing permissions and limitations under the License
**********************************************/

//*******************************************************************//
// IMPORTANT: Do not change anything other than the following #define
// statements to avoid symbol conflict/duplication during compilation.
//*******************************************************************//

#pragma once
//=============================//

#ifdef WIN32
  #define EZLASLIB_EXPORT __declspec(dllexport)
  #define EZLASLIB_IMPORT __declspec(dllimport)
  #define EZLASLIB_METHOD virtual HRESULT __stdcall

  #ifndef SUCCEEDED
    #define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
    #define FAILED(hr) (((HRESULT)(hr)) < 0)
  #endif

  #ifndef HRESULT
    typedef long HRESULT;
  #endif

  #ifndef INT64
    typedef signed char      INT8;
    typedef unsigned char    UINT8;
    typedef signed short     INT16;
    typedef unsigned short   UINT16;
    typedef signed int       INT32;
    typedef unsigned int     UINT32;
    typedef signed __int64   INT64;
    typedef unsigned __int64 UINT64;
  #endif
#else
  #define EZLASLIB_EXPORT __attribute__((visibility("default")))
  #define EZLASLIB_IMPORT
  #define EZLASLIB_METHOD virtual HRESULT

  #define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
  #define FAILED(hr) (((HRESULT)(hr)) < 0)

  typedef signed int64_t    INT64;
  typedef unsigned int64_t UINT64;
#endif

#ifdef EZLASLIB_EXPORTS
  #define EZLASLIB_EXPORTS_API EZLASLIB_EXPORT 
  #define EZLASLIB_INTERFACE struct EZLASLIB_EXPORT
#else
  #define EZLASLIB_EXPORTS_API EZLASLIB_IMPORT 
  #define EZLASLIB_INTERFACE struct EZLASLIB_IMPORT
#endif
//=============================//

// IMPORTANT: Do not change anything below this line.

enum EzLasErrorCodes 
{
  E_EZLAS_OK =                      0,          // Same as S_OK
  E_EZLAS_FALSE =                   1,          // Same as S_FALSE
  E_EZLAS_NO_INTERFACE =           -2147467262, // Same as E_NOINTERFACE (0x80004002L)
  E_EZLAS_POINTER =                -2147467261, // Same as E_POINTER     (0x80004003L)
  E_EZLAS_FAIL =                   -2147467259, // Same as E_FAIL        (0x80004005L)
  E_EZLAS_OUTOFMEMORY =            -2147024882, // Same as E_OUTOFMEMORY (0x8007000EL)
  E_EZLAS_INVALIDARG =             -2147024809, // Same as E_INVALIDARG  (0x80070057L)

  E_EZLAS_FILE_EXISTS =            -2147188736, // (0x80048000L)
  E_EZLAS_FILE_NOT_EXIST =         -2147188735, // (0x80048001L)
  E_EZLAS_FILE_OPEN_ERROR =        -2147188734,
  E_EZLAS_FILE_READ_ERROR =        -2147188733,
  E_EZLAS_FILE_WRITE_ERROR =       -2147188732,
  E_EZLAS_FILE_CREATE_ERROR =      -2147188731,
  E_EZLAS_FILE_DELETE_ERROR =      -2147188730,
  E_EZLAS_FILE_OVERWRITE_ERROR =   -2147188729,
  E_EZLAS_NOT_LAS_FILE =           -2147188728,
  E_EZLAS_BAD_INPUT_FILE =         -2147188727,
  E_EZLAS_BUFFER_OVERFLOW =        -2147188715,
  E_EZLAS_NO_TEMP_PATH =           -2147188714,
  E_EZLAS_INVALID_CS_RECORD =      -2147188713,
  E_EZLAS_PROCESS_CANCELLED =      -2147188712,
  E_EZLAS_VERSION_NOT_SUPPORTED =  -2147188711,
  E_EZLAS_FORMAT_NOT_SUPPORTED =   -2147188710,

  E_EZLAS_NOT_ZLAS_FILE =          -2147184640, // (0x80049000L)
  E_EZLAS_INVALID_OBJECT_TYPE =    -2147184639, // (0x80049001L)
  E_EZLAS_OBJECT_NOT_INITIALIZED = -2147184638,
  E_EZLAS_BETA_VERSION =           -2147184637,
};
//-----------------------------//

enum EzLasPointPropertyType
{
  EzLasPointXY =                0x00000001,
  EzLasPointZ =                 0x00000002,
  EzLasPointIntensity =         0x00000004,
  EzLasPointReturnNumber =      0x00000008,
  EzLasPointNumberOfReturns =   0x00000010,
  EzLasPointScannerChannel =    0x00000020,
  EzLasPointScanDirectionFlag = 0x00000040,
  EzLasPointEdgeOfFlightLine =  0x00000080,
  EzLasPointClassCode =         0x00000100,
  EzLasPointFlags =             0x00000200,
  EzLasPointScanAngle =         0x00000400,
  EzLasPointUserData =          0x00000800,
  EzLasPointSourceID =          0x00001000,
  EzLasPointGpsTime =           0x00002000,
  EzLasPointColorRed =          0x00004000,
  EzLasPointColorGreen =        0x00008000,
  EzLasPointColorBlue =         0x00010000,
  EzLasPointColorNIR =          0x00020000,
  EzLasPointExtraBytes =        0x00040000,
  EzLasPointAll =               0xFFFFFFFF,
};
//-----------------------------//

enum EzLasColorChannelType
{
  EzLasColorRed =   0x00000001,
  EzLasColorGreen = 0x00000002,
  EzLasColorBlue =  0x00000004,
  EzLasColorNIR =   0x00000008,
};
//-----------------------------//

enum EzLasPointFlagType
{
  EzLasFlagNone      = 0x00000001,
  EzLasFlagSynthetic = 0x00000002,
  EzLasFlagKey =       0x00000004,
  EzLasFlagWithheld =  0x00000008,
  EzLasFlagOverlap =   0x00000016,
};
//-----------------------------//

enum EzLasPointReturnType
{
  EzLasReturn1 = 1,
  EzLasReturn2 = 2,
  EzLasReturn3 = 3,
  EzLasReturn4 = 4,
  EzLasReturn5 = 5,
  EzLasReturn6 = 6,
  EzLasReturn7 = 7,
  EzLasReturn8 = 8,
  EzLasReturn9 = 9,
  EzLasReturn10 = 10,
  EzLasReturn11 = 11,
  EzLasReturn12 = 12,
  EzLasReturn13 = 13,
  EzLasReturn14 = 14,
  EzLasReturn15 = 15,
  EzLasReturnLast = 256,
  EzLasReturnSingle = 257,
  EzLasReturnFirstOfMany = 258,
  EzLasReturnLastOfMany = 259,
  EzLasReturnAll = -1
};
//=============================//

struct EZLASLIB_EXPORTS_API EzLasHeader
{ 
  unsigned char verMajor, verMinor;
  unsigned char pointDataFormatID;
  unsigned short pointDataRecordLength;
  unsigned short pointDataExtraBytesSize;
  unsigned short headerSize;

  unsigned long numberOfPointRecords1_3;
  unsigned long numberOfPointsByReturn1_3[5];

  UINT64 numberOfPointRecords1_4;
  UINT64 numberOfPointsByReturn1_4[15];

  unsigned long numberOfVLRs;
  unsigned long numberOfEVLRs;

  unsigned long offsetToData;

  UINT64 startOfWaveformData;
  UINT64 startOfFirstEVLR;

  double xOffset, yOffset, zOffset;
  double xScaleFactor, yScaleFactor, zScaleFactor;

  double xMin, yMin, zMin, xMax, yMax, zMax;
  
  char fileSignature[5];
  char systemID[33];
  char generatingSoftware[33];

  unsigned short fileSourceID;
  unsigned short fileCreationDayOfYear;
  unsigned short fileCreationYear;
  unsigned short globalEncoding;

  unsigned long  guid1;
  unsigned short guid2;
  unsigned short guid3;
  unsigned char  guid4[9];
};
//-----------------------------//

struct EZLASLIB_EXPORTS_API EzLasPointInfo
{ 
  UINT64 recordID; // 1-based point record number.

  unsigned char returnNumber;
  unsigned char numberOfReturns;
  unsigned char scanDirectionFlag;
  unsigned char edgeOfFlightLine;
  unsigned char scannerChannel;
  unsigned char classCode;

  bool bIsWithheld;
  bool bIsKeyPoint;
  bool bIsSyntheticPoint;
  bool bIsOverlapPoint;

  unsigned short intensity;
  unsigned short pointSourceID;
  unsigned char  userData;

  double scanAngle;
  double gpsTime;

  unsigned short red; 
  unsigned short green; 
  unsigned short blue;
  unsigned short nir; // Near infrared.

  double x, y, z;
};
//-----------------------------//

struct EZLASLIB_EXPORTS_API EzLasRGB
{ 
  unsigned short red, green, blue;
};
//-----------------------------//

struct EZLASLIB_EXPORTS_API EzLasPoint2D
{ 
  double x, y;
};
//-----------------------------//

struct EZLASLIB_EXPORTS_API EzLasPoint3D
{ 
  double x, y, z;
};
//-----------------------------//

struct EZLASLIB_EXPORTS_API EzLasExtent3D
{ 
  double xMin, yMin, zMin;
  double xMax, yMax, zMax;
};
//=============================//

enum EzLasObjectType 
{
  EzLasOID_Reader = 1,
  EzLasOID_Writer = 2,
  EzLasOID_QueryFilter = 3,
  EzLasOID_LongArray = 4,
  EzLasOID_Long64Array = 5,
};
//-----------------------------//

enum EzLasInterfaceID 
{
  EzLasIID_EzLasObject = 0,
  EzLasIID_Reader = 1,
  EzLasIID_Writer = 2,
  EzLasIID_QueryFilter = 3,
  EzLasIID_LongArray = 4,
  EzLasIID_Long64Array = 5,
  EzLasIID_MemoryBuffer = 6,
  EzLasIID_EnumPoint = 7,
  EzLasIID_Feedback = 8,
};
//-----------------------------//

enum EzLasPointSpacingOption 
{
  EzLasPointSpacingUserInput = 0,
  EzLasPointSpacingFromHeader = 1,
  EzLasPointSpacingScanFile = 2,
};
//-----------------------------//

// Interfaces.

EZLASLIB_INTERFACE iEzLasObject
{
  // Increments the reference count of an object.
  EZLASLIB_METHOD AddRef() = 0;

  // Decrements the reference count of an object.
  EZLASLIB_METHOD Release() = 0;

  // Retrieves the pointer to the interface of an object.
  EZLASLIB_METHOD QueryInterface(EzLasInterfaceID iid, void** ppv) = 0;

  // Clears the error info of an object.
  EZLASLIB_METHOD ClearError() = 0;

  // Retrieves the pointer to the error info held by an object.
  // The caller must not delete or free the pointer.  
  EZLASLIB_METHOD AccessErrorDescription(wchar_t** ppDescription) = 0;
};
//-----------------------------//

EZLASLIB_INTERFACE iEzLasFeedback : public iEzLasObject
{
  // Indicates if a process should continue to run.
  EZLASLIB_METHOD Continue(bool& bContinue) = 0;

  // Sets the current process's completion percentage. This is a callback function
  // that allows the caller of a long process to provide progress feedback to users.
  // A negative value indicates that the information is not available.
  EZLASLIB_METHOD SetProcessCompletionPercentage(long percentage) = 0;
};
//-----------------------------//

EZLASLIB_INTERFACE iEzLasMemoryBuffer : public iEzLasObject
{
  // Retrieves the pointer to the buffer of a memory buffer object and its size. 
  // The caller must not delete or free the pointer, but is responsible for 
  // calling iEzLasObject::Release to release the object.
  EZLASLIB_METHOD AccessBuffer(char** ppBuffer, long& bufferSize) = 0;
};
//-----------------------------//

EZLASLIB_INTERFACE iEzLasLongArray : public iEzLasObject
{
  // Removes all the elements in the array.
  EZLASLIB_METHOD SetEmpty() = 0;

  // Adds an element to the array. 
  EZLASLIB_METHOD Add(long value) = 0;

  // Gets the element count in the array.
  EZLASLIB_METHOD GetElementCount(long& count) = 0;

  // Retrieves the element specified by the 0-based array index.
  EZLASLIB_METHOD GetElement(long index, long& element) = 0;
};
//-----------------------------//

EZLASLIB_INTERFACE iEzLasLong64Array : public iEzLasObject
{
  // Removes all the elements in the array.
  EZLASLIB_METHOD SetEmpty() = 0;

  // Adds an element to the array. 
  EZLASLIB_METHOD Add(INT64 value) = 0;

  // Gets the element count in the array.
  EZLASLIB_METHOD GetElementCount(long& count) = 0;

  // Retrieves the element specified by the 0-based array index.
  EZLASLIB_METHOD GetElement(long index, INT64& element) = 0;
};
//-----------------------------//

EZLASLIB_INTERFACE iEzLasQueryFilter : public iEzLasObject
{
  // Clears the filter object and set its status to default.
  EZLASLIB_METHOD SetEmpty() = 0;

  // Sets area of interest.
  EZLASLIB_METHOD SetAOI(EzLasExtent3D* pAOI) = 0;

  // Adds return number. Multiple return numbers can be added to the filter.
  EZLASLIB_METHOD AddReturn(EzLasPointReturnType ret) = 0;

  // Adds class code. Multiple codes can be added.
  EZLASLIB_METHOD AddClassCode(unsigned char code) = 0;

  // Adds classification flag. Multiple flags can be added.
  EZLASLIB_METHOD AddFlag(EzLasPointFlagType flag) = 0;

  // Removes all the return numbers added using AddReturn. 
  EZLASLIB_METHOD RemoveReturns() = 0;

  // Removes all the class codes added using AddClassCode. 
  EZLASLIB_METHOD RemoveClassCodes() = 0;

  // Removes all the classification flags added using AddFlag. 
  EZLASLIB_METHOD RemoveFlags() = 0;

  // Checks if the filter has an area of interest.
  EZLASLIB_METHOD HasAOI(bool& bHasAOI) = 0;

  // Retrieves the area of interest.
  // Note that its value is valid only if the filter has an area of interest.
  EZLASLIB_METHOD QueryAOI(EzLasExtent3D& aoi) = 0;

  // Populates the array with all the return numbers in the filter. The caller
  // can call EzLasObjectFactory::CreateObject to create the array object.
  // The caller should call iEzLasObject::Release to release the array object.
  EZLASLIB_METHOD QueryReturns(iEzLasLongArray* pReturns) = 0;

  // Populates the array with all the class codes in the filter. The caller
  // can call EzLasObjectFactory::CreateObject to create the array object.
  // The caller should call iEzLasObject::Release to release the array object.
  EZLASLIB_METHOD QueryClassCodes(iEzLasLongArray* pCodes) = 0;

  // Populates the array with all the classification flags in the filter. The
  // caller can call EzLasObjectFactory::CreateObject to create the array object.
  // The caller should call iEzLasObject::Release to release the array object.
  EZLASLIB_METHOD QueryFlags(iEzLasLongArray* pFlags) = 0;
};
//-----------------------------//

EZLASLIB_INTERFACE iEzLasEnumPoint : public iEzLasObject
{
  // Resets the enumerator.
  EZLASLIB_METHOD Reset() = 0;

  // Retrieves point records as a memory buffer. The size of the buffer 'records' must
  // be at least equal to the product of 'arraySize' and point-record-length. The
  // caller is responsible for allocating and freeing the buffer. Output argument
  // 'pointCount' indicates the number of point records actually retrieved, and zero
  // means that all the records have been returned. Output argument 'pPointIDs' is
  // optional. It can be NULL or a pointer to a UINT64 array.
  EZLASLIB_METHOD QueryNext(long arraySize, long& pointCount, char* records, UINT64* pPointIDs) = 0;

  // Retrieves point records as an array of EzLasPointInfo.
  // Argument 'extraBytes' is optional and can be set to NULL.
  EZLASLIB_METHOD QueryNext(long arraySize, long& pointCount, EzLasPointInfo points[], char* extraBytes) = 0;

  // Retrieves point records as an array of EzLasPoint3D.
  // Argument 'extraBytes' is optional and can be set to NULL.
  EZLASLIB_METHOD QueryNext(long arraySize, long& pointCount, EzLasPoint3D points[], char* extraBytes) = 0;

  // Retrieves point records as a set of arrays for different point properties.
  // Pass NULL for those properties not interested.
  EZLASLIB_METHOD QueryNext(long arraySize, long& pointCount, EzLasPoint3D* pPoints,
                            unsigned char* pReturns, unsigned char* pNumberOfReturns,
                            unsigned char* pClassCodes, unsigned char* pFlags,
                            unsigned short* pIntensities, EzLasRGB* pRGBs,
                            unsigned short* pNIRs, char* extraBytes, UINT64* pPointIDs) = 0;
};
//-----------------------------//

EZLASLIB_INTERFACE iEzLasReader : public iEzLasObject
{
  // Initializes the reader with a zLas file name and specify the CPU power usage.
  // The CPU power usage is between 1 - 100. 
  EZLASLIB_METHOD Init(const wchar_t* zLasFileName, long cpuUsage) = 0;

  // Retrieves LAS header info.
  EZLASLIB_METHOD GetLasHeader(EzLasHeader& header) = 0;

  // Retrieves LAS header info as a memory buffer. The caller is responsible for
  // releasing the memory buffer object.
  EZLASLIB_METHOD GetLasHeaderBuffer(iEzLasMemoryBuffer** ppHeader) = 0;

  // Gets the uncompressed LAS file size and compressed zLas file size.
  EZLASLIB_METHOD GetFileSizes(UINT64& uncompressedSize, UINT64& compressedSize) = 0;

  // Checks if the zLas file has been rearranged.
  EZLASLIB_METHOD GetIsRearranged(bool& bRearranged) = 0;

  // Gets the unique return numbers and their point counts.
  EZLASLIB_METHOD GetUniqueReturns(iEzLasLongArray** ppReturns, iEzLasLong64Array** ppPointCounts) = 0;

  // Gets the unique return class codes and their point counts.
  EZLASLIB_METHOD GetUniqueClassCodes(iEzLasLongArray** ppCodes, iEzLasLong64Array** ppPointCounts) = 0;

  // Gets the unique classification flags and their point counts.
  EZLASLIB_METHOD GetUniqueFlags(iEzLasLongArray** ppFlags, iEzLasLong64Array** ppPointCounts) = 0;

  // Gets the statistics of a specific point property.
  EZLASLIB_METHOD GetStats(EzLasPointPropertyType type, UINT64& count,
                           double& min, double& max, double& mean, double& stdDev) = 0;

  // Saves VLRs into a file and return the record count and size in bytes.
  EZLASLIB_METHOD SaveVLRs(const wchar_t* fileName, unsigned long& recordCount, UINT64& dataSize) = 0;

  // Saves EVLRs into a file and return the record count and size in bytes.
  EZLASLIB_METHOD SaveExtendedVLRs(const wchar_t* fileName, unsigned long& recordCount, UINT64& dataSize) = 0;

  // Retrieves the coordonate system info. Output argument 'bEVLR' indicates if
  // the info is located in the EVLRs. The caller is responsible for releasing the
  // memory buffer object. Note that 'ppRecords' will be NULL if no such records exist.
  EZLASLIB_METHOD GetCoordSystemRecords(iEzLasMemoryBuffer** ppRecords, long& recordCount, bool& bEVLR, bool& bWKT) = 0;

  // Retrieves point records as a memory buffer for those points specified in 'pointIDs'.
  // Input argument 'pointIDs' is an array of 1-based point record numbers. The buffer size must
  // be big enough to accommodate all the point records asked. Input argument 'pointDataMask'
  // is a bits union of all the point properties to be decompressed and returned. E.g., to
  // retrieve all the properties, set it to EzLasPointAll; to retrieve just xyz, set
  // it to EzLasPointXY | EzLasPointZ.
  EZLASLIB_METHOD QueryPoints(long arraySize, UINT64 pointIDs[], UINT32 pointDataMask, char* records) = 0;

  // Retrieves 'pointCount' number of point records as a memory buffer, starting at point
  // record number 'beginID'.
  EZLASLIB_METHOD QueryPoints(long pointCount, UINT64 beginID, UINT32 pointDataMask, char* records) = 0;
  
  // Retrieves point records as an array of EzLasPointInfo for those points specified in 'pointIDs'.
  // Argument 'extraBytes' is optional and can be set to NULL.
  EZLASLIB_METHOD QueryPoints(long arraySize, UINT64 pointIDs[], UINT32 pointDataMask, EzLasPointInfo pInfo[], char* extraBytes) = 0;

  // Retrieves 'pointCount' number of point records as an array of EzLasPointInfo,
  // starting at point record number 'beginID'.
  EZLASLIB_METHOD QueryPoints(long pointCount, UINT64 beginID, UINT32 pointDataMask, EzLasPointInfo pInfo[], char* extraBytes) = 0;

  // Retrieves point records as a point enumerator. Input argument 'pFilter' specifies
  // what points to be retrieved and can be set to NULL to get all the points. The
  // caller can call EzLasObjectFactory::CreateObject to create the filter object. 
  EZLASLIB_METHOD GetPoints(UINT32 pointDataMask, iEzLasQueryFilter* pFilter, iEzLasEnumPoint** ppPoints) = 0;

  // Checks the integrity of a zLas file.
  // The input argument 'pFeedback' provides the caller a mechanism to cancel the
  // process and report process progress. The caller will need to implement interface
  // iEzLasFeedback. If NULL is passed for this argument, users will not be able to
  // cancel the process.
  EZLASLIB_METHOD ValidateZlasFile(const wchar_t* zLasFileName, long cpuUsage, iEzLasFeedback* pFeedback) = 0;
};
//-----------------------------//

EZLASLIB_INTERFACE iEzLasWriter : public iEzLasObject
{
  // Sets the path of the directory designated for temporary files.
  // The default path is the same as the output file location. The path
  // must exist and must not contain a trailing separator. Passing NULL
  // as the argument value will restore the path to the default location.
  // Setting the path to a fast drive can increase performance.
  EZLASLIB_METHOD SetTempPath(const wchar_t* path) = 0;

  // Converts a zLas file to a LAS file.
  // The CPU power usage is between 1 - 100.
  // The input argument 'pFeedback' provides the caller a mechanism to cancel the
  // process and report process progress. The caller will need to implement interface
  // iEzLasFeedback. If NULL is passed for this argument, users will not be able to
  // cancel the process.
  EZLASLIB_METHOD ConvertToLas(const wchar_t* zLasFileName, const wchar_t* lasFileName,
                               long cpuUsage, iEzLasFeedback* pFeedback) = 0;

  // Converts a LAS file to a zLas file.
  // Argument 'bRearrange' indicates if point data should be spatially rearranged.
  // Argument 'spacingOption' specifies how to obtain point spacing if up-to-date
  // statistics is not available (through file .lasx). Argument 'pointSpacing' is used
  // only if 'spacingOption' is set to EzLasPointSpacingUserInput. It should be set to
  // zero if other options are used. Arguments 'csRecords', 'csRecordCount',
  // 'csRecordSize', and 'bEVLR' are used to inject/override the coordinate system info
  // when it is missing or incorrect. 'csRecords' should be in the form of VLRs or EVLRs. 
  EZLASLIB_METHOD ConvertToZlas(const wchar_t* lasFileName, const wchar_t* zLasFileName,
                                bool bRearrange, EzLasPointSpacingOption spacingOption,
                                double pointSpacing, char* csRecords, long csRecordCount,
                                long csRecordSize, bool bEVLR, long cpuUsage,
                                iEzLasFeedback* pFeedback) = 0;
};
//=============================//

// Classes.

class EZLASLIB_EXPORTS_API EzLasObjectFactory
{
public:
  // Create an EzLas object specified by the object type ID.
  static HRESULT CreateObject(EzLasObjectType objType, iEzLasObject** ppv);
  static HRESULT CreateObject(EzLasObjectType objType, EzLasInterfaceID iid, void** ppv);
};
