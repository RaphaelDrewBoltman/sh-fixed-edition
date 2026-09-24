#pragma once

#include "renderware.h"
#include "UsercallFunctionHandler.h"

typedef int Angle;

#define NJD_PI  3.141592

#define NJM_DEG_RAD(n)  ((n) * NJD_PI / 180.0)
/*  deg ¨ rad  */
#define NJM_DEG_ANG(n)  ((Angle)(((n) * 65536.0) / 360.0))
															/*  deg ¨ ang  */
#define NJM_RAD_ANG(n)  ((Angle)(((n) * 65536.0) / (2 * NJD_PI)))
															/*  rad ¨ ang  */
#define NJM_RAD_DEG(n)  ((n) * 180.0 / NJD_PI)
															/*  rad ¨ deg  */
#define NJM_ANG_DEG(n)  ((n) * 360.0 / 65536.0)
															/*  ang ¨ deg  */
#define NJM_ANG_RAD(n)  ((n) * (2 * NJD_PI) / 65536.0)
															/*  ang ¨ rad  */
struct ONEFILE
{
	char fileName[64];
	void* exBuffer;
	int fileData_HHC;
	void* fileMemoryBuffer_HHC;
	int errorMode_HHC;
	int initialized;
	void* chunkHeader_HHC;
	int fileDataLength;
};

struct PtclPlayData {
	RwInt8 ptclNum; // offset 0x0, size 0x1
	RwInt8 endtime; // offset 0x1, size 0x1
	RwInt16 timer; // offset 0x2, size 0x2
	sAngle ang; // offset 0x4, size 0xC
	RwV3d pos; // offset 0x10, size 0xC
	RwV3d vec; // offset 0x1C, size 0xC
};

UsercallFunc(int, ONEFILE__CheckFileID, (ONEFILE* _this, const char* fname), (_this, fname), 0x42F280, rEAX, rEAX, rECX);
UserpurgeFunc(void*, ONEFILE__OpenData, (ONEFILE* _this, RwUInt32 id, void* addressToDecompressTo), (_this, id, addressToDecompressTo), 0x42F340, rEAX, rEAX, stack4, stack4);
UserpurgeFunc(RwBool, ONEFILE__LoadOneFile, (ONEFILE* _this, const char* fname), (_this, fname), 0x42F100, rEAX, rEAX, stack4);
UserpurgeFunc(RwBool, ONEFILE__ReleaseOneFile, (ONEFILE* _this), (_this), 0x42F210, rEAX, rEBX);
UserpurgeFunc(RpClump*, ONEFILE__LoadClump, (int id, void* dest, ONEFILE* _this), (id, dest, _this), 0x42F440, rEAX, rEAX, rECX, stack4);
UserpurgeFunc(RwTexDictionary*, ONEFILE__LoadTexDictionary, (int id, void* dest, ONEFILE* _this), (id, dest, _this), 0x42F3C0, rEAX, rEAX, rECX, stack4);
UserpurgeFunc(RtAnimAnimation*, ONEFILE__LoadAnimation, (int id, void* dest, ONEFILE* _this), (id, dest, _this), 0x42F600, rEAX, rEAX, rECX, stack4);
UsercallFunc(RwInt32, LoadFile, (RwChar* fname, void* buff), (fname, buff), 0x41BDB0, rEAX, rEAX, stack4);
FunctionPointer(RpAtomic*, objRpAtomicSetGeometryFlagToModulateMaterialColor, (RpAtomic*, void*), 0x42C0F0);

class OneFileReader
{
private:
	ONEFILE* one = nullptr;
	void* rbuf = nullptr;

	bool CheckID(int id)
	{
		return id >= 2;
	}

	ONEFILE* OpenONEFile(const char* path)
	{
		ONEFILE* file = (ONEFILE*)RwMalloc(sizeof(ONEFILE));
		if (file)
		{
			file->fileName[0] = 0;
			file->exBuffer = 0;
			file->errorMode_HHC = 0;
			file->initialized = 1;
			ONEFILE__LoadOneFile(file, path);
			return file;
		}
		else
		{
			return NULL;
		}
	}

	void CloseONEFile(ONEFILE* file)
	{
		if (file)
		{
			ONEFILE__ReleaseOneFile(file);
			RwFree(file);
		}
	}

public:
	OneFileReader(const char* path)
	{
		one = OpenONEFile(path);
		rbuf = RwMalloc(0x4B000);
	}

	~OneFileReader()
	{
		if (one)
			CloseONEFile(one);
		if (rbuf)
			RwFree(rbuf);
	}

	RpClump* LoadClump(const char* fname)
	{
		int id = ONEFILE__CheckFileID(one, fname);
		return CheckID(id) ? ONEFILE__LoadClump(id, rbuf, one) : nullptr;
	}

	RtAnimAnimation* LoadAnim(const char* fname)
	{
		int id = ONEFILE__CheckFileID(one, fname);
		return CheckID(id) ? ONEFILE__LoadAnimation(id, rbuf, one) : nullptr;
	}

	RwTexDictionary* LoadTexDict(const char* fname)
	{
		int id = ONEFILE__CheckFileID(one, fname);
		return CheckID(id) ? ONEFILE__LoadTexDictionary(id, rbuf, one) : nullptr;
	}

	RwRGBA* LoadChaoPalette(const char* fname);
};