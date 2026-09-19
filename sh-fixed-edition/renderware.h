#pragma once

#include <stdint.h>
#include "UsercallFunctionHandler.h"
#include "MemAccess.h"

//////////////////////////////////// RW ////////////////////////////////////

#define RwMalloc(size) RwEngineInstance->memoryFuncs.rwmalloc(size)
#define RwCalloc(count, size) RwEngineInstance->memoryFuncs.rwcalloc(count, size)
#define RwFree(mem) RwEngineInstance->memoryFuncs.rwfree(mem)

enum RwOpCombineType : __int32
{
	rwCOMBINEREPLACE = 0x0,
	rwCOMBINEPRECONCAT = 0x1,
	rwCOMBINEPOSTCONCAT = 0x2,
};

enum RwPrimitiveType : __int32
{
	rwPRIMTYPENAPRIMTYPE = 0x0,
	rwPRIMTYPELINELIST = 0x1,
	rwPRIMTYPEPOLYLINE = 0x2,
	rwPRIMTYPETRILIST = 0x3,
	rwPRIMTYPETRISTRIP = 0x4,
	rwPRIMTYPETRIFAN = 0x5,
	rwPRIMTYPEPOINTLIST = 0x6,
};

enum RwRasterType
{
	rwRASTERTYPENORMAL = 0u,
	rwRASTERTYPEZBUFFER = 1u,
	rwRASTERTYPECAMERA = 2u,
	rwRASTERTYPETEXTURE = 4u,
	rwRASTERTYPECAMERATEXTURE = 5u,
	rwRASTERTYPEMASK = 7u,
	rwRASTERDONTALLOCATE = 128u,
};

enum rpGEOMETRYLOCK
{
	rpGEOMETRYLOCKVERTICES = 0x01,
	rpGEOMETRYLOCKNORMALS = 0x02,
	rpGEOMETRYLOCKPRELIGHT = 0x04,
	rpGEOMETRYLOCKTEXCOORDS = 0x08,
	rpGEOMETRYLOCKTEXCOORDS1 = 0x10,
	rpGEOMETRYLOCKTEXCOORDS2 = 0x20,
	rpGEOMETRYLOCKTEXCOORDS3 = 0x40,
	rpGEOMETRYLOCKTEXCOORDS4 = 0x80,
	rpGEOMETRYLOCKALL = 0xFF,
};

enum RwCameraProjection
{
	rwNACAMERAPROJECTION = 0x0,
	rwPERSPECTIVE = 0x1,
	rwPARALLEL = 0x2,
};

enum RwRenderState
{
	rwRENDERSTATENARENDERSTATE = 0u,
	rwRENDERSTATETEXTURERASTER = 1u,
	rwRENDERSTATETEXTUREADDRESS = 2u,
	rwRENDERSTATETEXTUREADDRESSU = 3u,
	rwRENDERSTATETEXTUREADDRESSV = 4u,
	rwRENDERSTATETEXTUREPERSPECTIVE = 5u,
	rwRENDERSTATEZTESTENABLE = 6u,
	rwRENDERSTATESHADEMODE = 7u,
	rwRENDERSTATEZWRITEENABLE = 8u,
	rwRENDERSTATETEXTUREFILTER = 9u,
	rwRENDERSTATESRCBLEND = 10u,
	rwRENDERSTATEDESTBLEND = 11u,
	rwRENDERSTATEVERTEXALPHAENABLE = 12u,
	rwRENDERSTATEBORDERCOLOR = 13u,
	rwRENDERSTATEFOGENABLE = 14u,
	rwRENDERSTATEFOGCOLOR = 15u,
	rwRENDERSTATEFOGTYPE = 16u,
	rwRENDERSTATEFOGDENSITY = 17u,
	rwRENDERSTATECULLMODE = 20u,
	rwRENDERSTATESTENCILENABLE = 21u,
	rwRENDERSTATESTENCILFAIL = 22u,
	rwRENDERSTATESTENCILZFAIL = 23u,
	rwRENDERSTATESTENCILPASS = 24u,
	rwRENDERSTATESTENCILFUNCTION = 25u,
	rwRENDERSTATESTENCILFUNCTIONREF = 26u,
	rwRENDERSTATESTENCILFUNCTIONMASK = 27u,
	rwRENDERSTATESTENCILFUNCTIONWRITEMASK = 28u,
};

enum RwBlendFunction
{
	rwBLENDNA,
	rwBLENDZERO,
	rwBLENDONE,
	rwBLENDSRCCOLOR,
	rwBLENDINVSRCCOLOR,
	rwBLENDSRCALPHA,
	rwBLENDINVSRCALPHA,
	rwBLENDDESTALPHA,
	rwBLENDINVDESTALPHA,
	rwBLENDDESTCOLOR,
	rwBLENDINVDESTCOLOR,
	rwBLENDSRCALPHASAT

};

enum RwCullMode
{
	rwCULLMODECULLNONE = 1,
	rwCULLMODECULLBACK,
	rwCULLMODECULLFRONT,
};

enum RwRasterLockMode
{
	rwRASTERLOCKREAD = 0x01,
	rwRASTERLOCKWRITE = 0x02,
	rwRASTERLOCKNOFETCH = 0x04,
	rwRASTERLOCKRAW = 0x08
};

enum RwRasterFormat
{
	rwRASTERFORMATDEFAULT = 0x0000,
	rwRASTERFORMAT1555 = 0x0100,
	rwRASTERFORMAT565 = 0x0200,
	rwRASTERFORMAT4444 = 0x0300,
	rwRASTERFORMATLUM8 = 0x0400,
	rwRASTERFORMAT8888 = 0x0500,
	rwRASTERFORMAT888 = 0x0600,
	rwRASTERFORMAT16 = 0x0700,
	rwRASTERFORMAT24 = 0x0800,
	rwRASTERFORMAT32 = 0x0900,
	rwRASTERFORMATPAL4 = 0x0A00,
	rwRASTERFORMATPAL8 = 0x0B00,
	rwRASTERFORMATP8 = 0x0B00,
	rwRASTERFORMATDITHER = 0x4000,
	rwRASTERFORMATMIPMAP = 0x2000,
	rwRASTERFORMATPIXELFORMATMASK = 0x0F00
};

enum RwIm3DTransformFlags
{
	rwIM3D_VERTEXUV = 0x1,
	rwIM3D_ALLOPAQUE = 0x2,
	rwIM3D_NOCLIP = 0x4,
	rwIM3D_VERTEXXYZ = 0x8,
	rwIM3D_VERTEXRGBA = 0x10,
};

enum RwFrameFlag
{
	rwFRAMEOBJECTTYPEHIERARCHYDIRTY = 0x01,
	rwFRAMEOBJECTTYPEMATRIXDIRTY = 0x02,
	rwFRAMEOBJECTTYPEMATRIXIDENTITY = 0x04,
	rwFRAMEOBJECTTYPEMATRIXORTHONORMAL = 0x08,
	rwFRAMEOBJECTTYPEMATRIXNOTORTHONORMAL = 0x10,
	rwFRAMEOBJECTTYPEMATRIXHASSCALING = 0x20
};

enum RwFrustumTestResult : __int32
{
	rwSPHEREOUTSIDE = 0x0,
	rwSPHEREBOUNDARY = 0x1,
	rwSPHEREINSIDE = 0x2,
};

enum RpMatFXMaterialFlags
{
	rpMATFXEFFECTNULL = 0,
	rpMATFXEFFECTBUMPMAP = 1,
	rpMATFXEFFECTENVMAP = 2,
	rpMATFXEFFECTBUMPENVMAP = 3,
	rpMATFXEFFECTDUAL = 4,
	rpMATFXEFFECTUVTRANSFORM = 5,
	rpMATFXEFFECTDUALUVTRANSFORM = 6,
};

typedef char RwChar;
typedef int8_t RwInt8;
typedef uint8_t RwUInt8;
typedef int32_t RwInt32;
typedef uint16_t RwUInt16;
typedef int16_t RwInt16;
typedef uint32_t RwUInt32;
typedef float_t RwReal;
typedef bool RwBool;

struct RwDevice
{
	RwReal gammaCorrection;
	void* fpSystem;
	RwReal zBufferNear;
	RwReal zBufferFar;
	void* fpRenderStateSet;
	void* fpRenderStateGet;
	void* fpIm2DRenderLine;
	void* fpIm2DRenderTriangle;
	void* fpIm2DRenderPrimitive;
	void* fpIm2DRenderIndexedPrimitive;
	void* fpIm3DRenderLine;
	void* fpIm3DRenderTriangle;
	void* fpIm3DRenderPrimitive;
	void* fpIm3DRenderIndexedPrimitive;
};

typedef RwBool(__cdecl* RwStandardFunc)(void* pOut, void* pInOut, RwInt32 nI);

struct RwLLLink
{
	RwLLLink* next;
	RwLLLink* prev;
};

struct RwLinkList
{
	RwLLLink link;
};

struct RwFileFunctions
{
	void* rwfexist;
	void* rwfopen;
	void* rwfclose;
	void* rwfread;
	void* rwfwrite;
	void* rwfgets;
	void* rwfputs;
	void* rwfeof;
	void* rwfseek;
	void* rwfflush;
	void* rwftell;
};

struct RwStringFunctions
{
	void* vecSprintf;
	void* vecVsprintf;
	void* vecStrcpy;
	void* vecStrncpy;
	void* vecStrcat;
	void* vecStrncat;
	void* vecStrrchr;
	void* vecStrchr;
	void* vecStrstr;
	void* vecStrcmp;
	void* vecStrncmp;
	void* vecStricmp;
	void* vecStrlen;
	void* vecStrupr;
	void* vecStrlwr;
	void* vecStrtok;
	void* vecSscanf;
};

struct RwFreeList
{
	RwUInt32 entrySize;
	RwUInt32 entriesPerBlock;
	RwUInt32 heapSize;
	RwUInt32 alignment;
	RwLinkList blockList;
	RwUInt32 flags;
	RwLLLink link;
};

struct RwMemoryFunctions
{
	void* (__cdecl* rwmalloc)(size_t size);
	void(__cdecl* rwfree)(void* mem);
	void* (__cdecl* rwrealloc)(void* mem, size_t newSize);
	void* (__cdecl* rwcalloc)(size_t numObj, size_t sizeObj);
};

struct RwIm2DVertex
{
	RwReal x;
	RwReal y;
	RwReal z;
	RwReal rhw;
	RwUInt32 emissiveColor;
	RwReal u;
	RwReal v;
};

struct RwV2d
{
	RwReal x;
	RwReal y;
};

struct RwV3d
{
	RwReal x;
	RwReal y;
	RwReal z;
};

struct RwRGBA
{
	RwUInt8 red;
	RwUInt8 green;
	RwUInt8 blue;
	RwUInt8 alpha;
};

struct RwRGBAReal
{
	RwReal red;
	RwReal green;
	RwReal blue;
	RwReal alpha;
};

struct rt2dShadeParameters
{
	RwRGBAReal col;
	RwV2d uv;
};

struct Rt2dBrush
{
	rt2dShadeParameters top;
	rt2dShadeParameters dtop;
	rt2dShadeParameters bottom;
	rt2dShadeParameters dbottom;
	RwRGBA colorCache;
	RwInt32 flag;
	void* texture;
	void* material;
	RwReal halfwidth;
	RwInt32 refCount;
};

struct RwSList
{
	RwUInt8* listElements;
	RwInt32 numElementsFilled;
	RwInt32 numElementsAlloced;
	RwInt32 entrySize;
};

struct Rt2dFont
{
	char gap0[4];
	RwReal field_4;
	RwReal yShift__;
	RwReal width__;
	char gap10[280];
	RwSList* field_128;
	Rt2dFont* (*fRenderFunc)();
	char gap130[4];
	int field_134;
};

struct RwMatrix
{
	RwV3d right;
	int flags;
	RwV3d up;
	RwUInt32 pad1;
	RwV3d at;
	RwUInt32 pad2;
	RwV3d pos;
	RwUInt32 pad3;
};

struct _rt2dScene
{
	RwSList* objects;
	RwInt32 objectCount;
	RwSList* depths;
	RwBool isDirtyDepths;
};

union _rt2dObjectdata
{
	//_rt2dShape shape;
	_rt2dScene scene;
	//_rt2dPickRegion pickRegion;
	//_rt2dObjectString objectString;
};

struct Rt2dObject
{
	RwInt32 type;
	RwInt32 depth;
	RwInt32 flag;
	RwMatrix MTM;
	RwMatrix LTM;
	RwRGBAReal colorMult;
	RwRGBAReal colorOffs;
	_rt2dObjectdata data;
};

struct Rt2dBBox
{
	RwReal x;
	RwReal y;
	RwReal w;
	RwReal h;
};

struct Rt2dMaestro
{
	int flagsMaybe;
	char gap4[4];
	RwSList* animations;
	const char** stringLabels;
	void* buttons;
	char field_14;
	void* messageList;
	void* byteBucket__;
	void* messageQueue;
	Rt2dObject* scene;
	int(__cdecl* callbackMaybe)(int, int);
	float float_2C;
	float float_30;
	char gap34[4];
	RwV3d camRelated;
	int field_44;
	Rt2dBBox bbox;
	void* stateTransTable;
};

struct RwObject
{
	RwUInt8 type;
	RwUInt8 subType;
	RwUInt8 flags;
	RwUInt8 privateFlags;
	void* parent;
};

struct RpClump;

typedef RpClump* (__cdecl* RpClumpCallBack)(RpClump* clump, void* data);

struct RpClump
{
	RwObject object;
	RwLinkList atomicList;
	RwLinkList lightList;
	RwLinkList cameraList;
	RwLLLink inWorldLink;
	RpClumpCallBack callback;
};

struct RwFrame
{
	RwObject object;
	RwLLLink inDirtyListLink;
	RwMatrix modelling;
	RwMatrix ltm;
	RwLinkList objectList;
	struct RwFrame* child;
	struct RwFrame* next;
	struct RwFrame* root;
};

struct RwTexDictionary
{
	RwObject object;
	RwLinkList texturesInDict;
	RwLLLink lInInstance;
};

struct RtAnimInterpolatorInfo
{
	RwInt32 typeID;
	RwInt32 interpKeyFrameSize;
	RwInt32 animKeyFrameSize;
	void* keyFrameApplyCB;
	void* keyFrameBlendCB;
	void* keyFrameInterpolateCB;
	void* keyFrameAddCB;
	void* keyFrameMulRecipCB;
	void* keyFrameStreamReadCB;
	void* keyFrameStreamWriteCB;
	void* keyFrameStreamGetSizeCB;
	RwInt32 customDataSize;
};

struct RtAnimAnimation
{
	RtAnimInterpolatorInfo* interpInfo;
	RwInt32 numFrames;
	RwInt32 flags;
	RwReal duration;
	void* pFrames;
	void* customData;
};

struct RpHAnimNodeInfo
{
	RwInt32 nodeID;
	RwInt32 nodeIndex;
	RwInt32 flags;
	RwFrame* pFrame;
};

struct RtAnimInterpolator;

typedef RtAnimInterpolator* (__cdecl* RtAnimCallBack)(RtAnimInterpolator* animInstance, void* data);
typedef void(__cdecl* RtAnimKeyFrameApplyCallBack)(void* result, void* voidIFrame);
typedef void(__cdecl* RtAnimKeyFrameBlendCallBack)(void* voidOut, void* voidIn1, void* voidIn2, RwReal alpha);
typedef void(__cdecl* RtAnimKeyFrameInterpolateCallBack)(void* voidOut, void* voidIn1, void* voidIn2, RwReal time, void* customData);
typedef void(__cdecl* RtAnimKeyFrameAddCallBack)(void* voidOut, void* voidIn1, void* voidIn2);
typedef void(__cdecl* RtAnimKeyFrameMulRecipCallBack)(void* voidFrame, void* voidStart);

struct RtAnimInterpolator
{
	RtAnimAnimation* pCurrentAnim;
	RwReal currentTime;
	void* pNextFrame;
	RtAnimCallBack pAnimCallBack;
	void* pAnimCallBackData;
	RwReal animCallBackTime;
	RtAnimCallBack pAnimLoopCallBack;
	void* pAnimLoopCallBackData;
	RwInt32 maxInterpKeyFrameSize;
	RwInt32 currentInterpKeyFrameSize;
	RwInt32 currentAnimKeyFrameSize;
	RwInt32 numNodes;
	RwBool isSubInterpolator;
	RwInt32 offsetInParent;
	RtAnimInterpolator* parentAnimation;
	RtAnimKeyFrameApplyCallBack keyFrameApplyCB;
	RtAnimKeyFrameBlendCallBack keyFrameBlendCB;
	RtAnimKeyFrameInterpolateCallBack keyFrameInterpolateCB;
	RtAnimKeyFrameAddCallBack keyFrameAddCB;
};

struct RpHAnimHierarchy
{
	RwInt32 flags;
	RwInt32 numNodes;
	RwMatrix* pMatrixArray;
	void* pMatrixArrayUnaligned;
	RpHAnimNodeInfo* pNodeInfo;
	RwFrame* parentFrame;
	RpHAnimHierarchy* parentHierarchy;
	RwInt32 rootParentOffset;
	RtAnimInterpolator* currentAnim;
};

struct RwObjectHasFrame;

typedef RwObjectHasFrame* (__cdecl* RwObjectHasFrameSyncFunction)(RwObjectHasFrame* object);

struct RwObjectHasFrame
{
	RwObject object;
	RwLLLink lFrame;
	RwObjectHasFrameSyncFunction sync;
};

struct RwResEntry;

typedef void(__cdecl* RwResEntryDestroyNotify)(RwResEntry* resEntry);

struct RwResEntry
{
	RwLLLink link;
	RwInt32 size;
	void* owner;
	RwResEntry** ownerRef;
	RwResEntryDestroyNotify destroyNotify;
};

struct RwRaster
{
	RwRaster* parent;
	RwUInt8* cpPixels;
	RwUInt8* palette;
	RwInt32 width;
	RwInt32 height;
	RwInt32 depth;
	RwInt32 stride;
	RwInt16 nOffsetX;
	RwInt16 nOffsetY;
	RwUInt8 cType;
	RwRasterType cFlags;
	RwUInt8 privateFlags;
	RwUInt8 cFormat;
	RwUInt8* originalPixels;
	RwInt32 originalWidth;
	RwInt32 originalHeight;
	int originalStride;
};


struct RwTexture
{
	RwRaster* raster;
	RwTexDictionary* dict;
	RwLLLink lInDictionary;
	RwChar name[32];
	RwChar mask[32];
	RwUInt32 filterAddressing;
	RwInt32 refCount;
};

struct RwSurfaceProperties
{
	RwReal ambient;
	RwReal specular;
	RwReal diffuse;
};

struct RpMaterial
{
	RwTexture* texture;
	RwRGBA color;
	void* pipeline; // RxPipeline* pipeline;
	RwSurfaceProperties surfaceProps;
	RwInt16 refCount;
	RwInt16 pad;
};

struct RpMaterialList
{
	RpMaterial** materials;
	RwInt32 numMaterials;
	RwInt32 space;
};

struct RpTriangle
{
	RwUInt16 vertIndex[3];
	RwInt16 matIndex;
};

struct RwTexCoords
{
	RwReal u;
	RwReal v;
};

struct RpMeshHeader
{
	RwUInt32 flags;
	RwUInt16 numMeshes;
	RwUInt16 serialNum;
	RwUInt32 totalIndicesInMesh;
	RwUInt32 firstMeshOffset;
};

struct RwSphere
{
	RwV3d center;
	RwReal radius;
};

struct RwPlane
{
	RwV3d normal;
	RwReal distance;
};

struct RwFrustumPlane
{
	RwPlane plane;
	RwUInt8 closestX;
	RwUInt8 closestY;
	RwUInt8 closestZ;
	RwUInt8 pad;
};

struct RwBBox
{
	RwV3d sup;
	RwV3d inf;
};

struct RwCamera;

typedef RwCamera* (__cdecl* RwCameraBeginUpdateFunc)(RwCamera* camera);
typedef RwCamera* (__cdecl* RwCameraEndUpdateFunc)(RwCamera* camera);

struct RwCamera
{
	RwObjectHasFrame object;
	RwCameraProjection projectionType;
	RwCameraBeginUpdateFunc beginUpdate;
	RwCameraEndUpdateFunc endUpdate;
	RwMatrix viewMatrix;
	RwRaster* frameBuffer;
	RwRaster* zBuffer;
	RwV2d viewWindow;
	RwV2d recipViewWindow;
	RwV2d viewOffset;
	RwReal nearPlane;
	RwReal farPlane;
	RwReal fogPlane;
	RwReal zScale;
	RwReal zShift;
	RwFrustumPlane frustumPlanes[6];
	RwBBox frustumBoundBox;
	RwV3d frustumCorners[8];
};

struct RpGeometry;

typedef RpMaterial* (*RpMaterialCallBack)(RpMaterial* material, void* data);

struct RpMorphTarget
{
	RpGeometry* parentGeom;
	RwSphere boundingSphere;
	RwV3d* verts;
	RwV3d* normals;
};

struct RpGeometry
{
	RwObject object;
	RwUInt32 flags;
	RwUInt16 lockedSinceLastInst;
	RwInt16 refCount;
	RwInt32 numTriangles;
	RwInt32 numVertices;
	RwInt32 numMorphTargets;
	RwInt32 numTexCoordSets;
	RpMaterialList matList;
	RpTriangle* triangles;
	RwRGBA* preLitLum;
	RwTexCoords* texCoords[8];
	RpMeshHeader* mesh;
	RwResEntry* repEntry;
	RpMorphTarget* morphTarget;
};

struct RpInterpolator
{
	RwInt32 flags;
	RwInt16 startMorphTarget;
	RwInt16 endMorphTarget;
	RwReal time;
	RwReal recipTime;
	RwReal position;
};

struct RpAtomic;

typedef RpAtomic* (__cdecl* RpAtomicCallBackRender)(RpAtomic* atomic);

struct RpAtomic
{
	RwObjectHasFrame object;
	RwResEntry* repEntry;
	RpGeometry* geometry;
	RwSphere boundingSphere;
	RwSphere worldBoundingSphere;
	RpClump* clump;
	RwLLLink inClumpLink;
	RpAtomicCallBackRender renderCallBack;
	RpInterpolator interpolator;
	RwUInt16 renderFrame;
	RwUInt16 pad;
	RwLinkList llWorldSectorsInAtomic;
	void* pipeline; // RxPipeline* pipeline;
};

typedef RwFrame* (__cdecl* RwFrameCallBack)(RwFrame* frame, void* data);
typedef RpAtomic* (__cdecl* RpAtomicCallBack)(RpAtomic* atomic, void* data);

struct RpLight
{
	RwObjectHasFrame object;
	RwReal radius;
	RwRGBAReal color;
	RwReal minusCosAngle;
	RwLinkList WorldSectorsInLight;
	RwLLLink inWorld;
	RwUInt16 lightFrame;
	RwUInt16 pad;
};

enum RpWorldRenderOrder
{
	rpWORLDRENDERNARENDERORDER = 0x0,
	rpWORLDRENDERFRONT2BACK = 0x1,
	rpWORLDRENDERBACK2FRONT = 0x2,
};

struct RpSector
{
	RwInt32 type;
};

struct RpWorld
{
	RwObject object;
	RwUInt32 flags;
	RpWorldRenderOrder renderOrder;
	RpMaterialList matList;
	RpSector* rootSector;
	RwInt32 numTexCoordSets;
	RwInt32 numClumpsInWorld;
	RwLLLink* currentClumpLink;
	RwLinkList clumpList;
	RwLinkList lightList;
	RwLinkList directionalLightList;
	RwV3d worldOrigin;
	RwBBox boundingBox;
	/*RpWorldSectorCallBackRender renderCallBack;
	RxPipeline* pipeline;*/
};

struct RwIm3DVertex
{
	RwV3d objVertex;
	RwV3d objNormal;
	RwRGBA color;
	RwTexCoords uv;
};

struct RwGlobals
{
	RwCamera* curCamera;
	void* curWorld;
	RwUInt16 renderFrame;
	RwUInt16 lightFrame;
	RwUInt16 pad[2];
	RwDevice dOpenDevice;
	RwStandardFunc stdFunc[29];
	RwLinkList dirtyFrameList;
	RwFileFunctions fileFuncs;
	RwStringFunctions stringFuncs;
	RwMemoryFunctions memoryFuncs;
	//RwMemoryAllocFn memoryAlloc;
	//RwMemoryFreeFn memoryFree;
	//RwMetrics* metrics;
	//RwEngineStatus engineStatus;
	//RwUInt32 resArenaInitSize;
};

struct sAngle
{
	RwInt32 x;
	RwInt32 y;
	RwInt32 z;
};

#define RpAtomicGetGeometry(atomic) (atomic->geometry)

#define RpGeometryGetMorphTarget(geometry, index) (&geometry->morphTarget[index])
#define RpGeometryGetNumVertices(geometry) (geometry->numVertices)

#define RpMorphTargetGetVertices(morphTarget) (morphTarget->verts)
#define RpMorphTargetGetNormals(morphTarget) (morphTarget->normals)
#define RpMorphTargetGetBoundingSphere(morphTarget) (&morphTarget->boundingSphere)

#define RwRasterGetWidth(raster) (raster->width)
#define RwRasterGetHeight(raster) (raster->height)
#define RwRasterGetDepth(raster) (raster->depth)
#define RwRasterGetFormat(raster) (raster->cType | raster->cFlags)

#define RwTextureGetRaster(texture) (texture->raster)

#define RwCameraGetFrame(camera) ((RwFrame*)camera->object.object.parent)

#define rwLLLinkGetData(link, type, member) ((type *)((char *)(link) - offsetof(type, member)))

FunctionPointer(Rt2dFont*, rt2dFontShowBitmap, (Rt2dFont* a1, const RwChar* a2, RwReal scl, RwReal* anchor, Rt2dBrush* brush), 0x6AA640);
FunctionPointer(Rt2dObject*, Rt2dObjectMTMTranslate, (Rt2dObject* object, RwReal x, RwReal y), 0x6ACE10);
FunctionPointer(Rt2dObject*, Rt2dObjectMTMScale, (Rt2dObject* object, RwReal x, RwReal y), 0x6ACDC0);
FunctionPointer(RwBool, Rt2dCTMTranslate, (RwReal x, RwReal y), 0x6AA330);
FunctionPointer(RwBool, Rt2dCTMSetIdentity, (), 0x6A9B60);
FunctionPointer(RwBool, Rt2dCTMPush, (), 0x6AA150);
FunctionPointer(RwBool, Rt2dCTMPop, (), 0x6AA2B0);
FunctionPointer(Rt2dObject*, Rt2dSceneUpdateLTM, (Rt2dObject* object), 0x6AD8B0);
FunctionPointer(Rt2dObject*, Rt2dObjectMTMChanged, (Rt2dObject* object), 0x06ACDB0);
FunctionPointer(Rt2dMaestro*, Rt2dMaestroUpdateAnimations, (Rt2dMaestro* maestro), 0x6B3FF0);
FunctionPointer(Rt2dMaestro*, Rt2dMaestroProcessMessages, (Rt2dMaestro* maestro), 0x6B4660);
FunctionPointer(RwBool, RwIm2DRenderPrimitive, (RwPrimitiveType primType, RwIm2DVertex* vertices, RwInt32 numVertices), 0x64CA10);
FunctionPointer(RwMatrix*, RwMatrixTranslate, (RwMatrix* matrix, RwV3d* translation, RwOpCombineType combineOp), 0x650E00);
FunctionPointer(RwMatrix*, RwMatrixScale, (RwMatrix* matrix, RwV3d* scale, RwOpCombineType combineOp), 0x650C70);
FunctionPointer(RpClump*, RpClumpRender, (RpClump* pClump), 0x66B4F0);
FunctionPointer(RwFrame*, RwFrameTranslate, (RwFrame* frame, const RwV3d* translation, RwOpCombineType combineOp), 0x64C620);
FunctionPointer(RwTexDictionary*, RwTexDictionarySetCurrent, (RwTexDictionary* dict), 0x64E430);
FunctionPointer(RwTexDictionary*, RwTexDictionaryGetCurrent, (), 0x64E450);
FunctionPointer(RpHAnimHierarchy*, RpHAnimFrameGetHierarchy, (RwFrame* frame), 0x6A8640);
FunctionPointer(RwFrame*, RwFrameForAllChildren, (RwFrame* frame, RwFrameCallBack callBack, void* data), 0x64C5E0);
FunctionPointer(RwFrame*, GetChildFrameHierarchy, (RwFrame* frame, void* data), 0x4A21F0);
FunctionPointer(RpClump*, RpClumpForAllAtomics, (RpClump* clump, RpAtomicCallBack callback, void* pData), 0x66B540);
FunctionPointer(RpAtomic*, nHierarchy__SetHierarchyForSkinAtomic, (RpAtomic* atomic, void* data), 0x5A22A0);
FunctionPointer(RpHAnimHierarchy*, RpHAnimHierarchyAttach, (RpHAnimHierarchy* hierarchy), 0x6A8560);
FunctionPointer(void, RtAnimInterpolatorSetCurrentAnim, (void*, void*), 0x69DFC0);
FunctionPointer(RwBool, RtAnimInterpolatorSetCurrentTime, (RtAnimInterpolator* anim, RwReal time), 0x69E490);
FunctionPointer(RwBool, RpHAnimHierarchyUpdateMatrices, (RpHAnimHierarchy* hierarchy), 0x6A8660);
FunctionPointer(int, Rt2dDeviceGetStep, (RwV2d* xstep, RwV2d* ystep, RwV2d* origin), 0x6A9C50);
DataPointer(RwGlobals*, RwEngineInstance, 0x8E0A4C);
UserpurgeFunc(RwTexDictionary*, texLoadTexDictionaryFile, (const char* fname), (fname), 0x444090, rEAX, rEAX);
FastcallFunctionPointer(RpAtomic*, objRpClumpGetAtomic, (RpClump* clump, RpAtomic* atomic), 0x42BB30);
FunctionPointer(RpGeometry*, RpGeometryLock, (RpGeometry*, __int16 flag), 0x66EC50);
FunctionPointer(RpGeometry*, RpGeometryUnlock, (RpGeometry*), 0x66EC80);
FunctionPointer(RpClump*, RpClumpClone, (RpClump* pClump), 0x66B920);
FunctionPointer(RwBool, RpClumpDestroy, (RpClump* pClump), 0x66BC60);
FunctionPointer(RwV3d*, RwV3dTransformPoints, (RwV3d* pointsOut, const RwV3d* pointsIn, RwInt32 numPoints, const RwMatrix* matrix), 0x651A50);
FunctionPointer(RwV3d*, RwV3dTransformVectors, (RwV3d* vectorsOut, const RwV3d* vectorsIn, RwInt32 numPoints, const RwMatrix* matrix), 0x651A80);
FunctionPointer(void*, RwIm3DTransform, (RwIm3DVertex* pVerts, RwUInt32 numVerts, RwMatrix* ltm, RwUInt32 flags), 0x652380);
FunctionPointer(RwBool, RwIm3DRenderPrimitive, (RwPrimitiveType primType), 0x6525E0);
VoidFunc(RwIm3DEnd, 0x652450);
FunctionPointer(int, RwRenderStateSet, (RwRenderState state, void* value), 0x64C9B0);
FunctionPointer(int, RwRenderStateGet, (RwRenderState state, void* value), 0x64CA00);
FunctionPointer(RwMatrix*, RwMatrixCreate, (), 0x651400);
FunctionPointer(RwBool, RwMatrixDestroy, (RwMatrix*), 0x6513D0);
FunctionPointer(RwRaster*, RwRasterCreate, (int width, int height, int depth, int flags), 0x6520E0);
FunctionPointer(RwBool, RwRasterDestroy, (RwRaster* raster), 0x651ED0);
FunctionPointer(RwUInt8*, RwRasterLock, (RwRaster* raster, int level, RwInt32 flag), 0x652180);
FunctionPointer(RwRaster*, RwRasterUnlock, (RwRaster* raster), 0x651E40);
FunctionPointer(RwRGBA*, RwRasterLockPalette, (RwRaster* raster, RwInt32 flag), 0x651F90);
FunctionPointer(RwRaster*, RwRasterUnlockPalette, (RwRaster* raster), 0x651EA0);
FunctionPointer(RwTexture*, RwTextureCreate, (RwRaster* raster), 0x64E180);
FunctionPointer(RwBool, RwTextureDestroy, (RwTexture* texture), 0x64E1E0);
FunctionPointer(RwMatrix*, RwMatrixInvert, (RwMatrix* dst, RwMatrix* src), 0x650A20);
FunctionPointer(RwMatrix*, RwMatrixMultiply, (RwMatrix* dst, RwMatrix* src1, RwMatrix* src2), 0x650260);
FunctionPointer(RwFrame*, RwFrameUpdateObjects, (RwFrame* frame), 0x64C280);
FunctionPointer(RwMatrix*, RwFrameGetLTM, (RwFrame* frame), 0x64C300);
FunctionPointer(RpWorld*, RpWorldAddClump, (RpWorld* world, RpClump* clump), 0x669840);
FunctionPointer(RpWorld*, RpWorldRemoveClump, (RpWorld* world, RpClump* clump), 0x6699A0);
FunctionPointer(void, RwD3D8SetRenderState, (int state, int value), 0x647980);
FunctionPointer(void, RwD3D8GetRenderState, (int state, int* value), 0x6479D0);
FunctionPointer(RwFrustumTestResult, RwCameraFrustumTestSphere, (const RwCamera* camera, const RwSphere* sphere), 0x64AB40);
FunctionPointer(RwTexture*, RwTexDictionaryFindNamedTexture, (RwTexDictionary* dict, const RwChar* name), 0x64E3B0);
FunctionPointer(RpAtomic*, RpMatFXAtomicEnableEffects, (RpAtomic* atomic), 0x677310);
FunctionPointer(RpMaterial*, RpMatFXMaterialSetEffects, (RpMaterial* material, RpMatFXMaterialFlags flags), 0x677340);
FunctionPointer(RpMaterial*, RpMatFXMaterialSetEnvMapTexture, (RpMaterial* material, RwTexture* texture), 0x6776F0);
FunctionPointer(RpMaterial*, RpMatFXMaterialSetBumpMapTexture, (RpMaterial* material, RwTexture* texture), 0x6774E0);
DataPointer(int, MatFXMaterialDataOffset, 0x8E187C);
FunctionPointer(RpGeometry*, RpGeometryForAllMaterials, (RpGeometry* geometry, RpMaterialCallBack callback, void* data), 0x66EC10);
FunctionPointer(RwReal, RwV3dLength, (const RwV3d* in), 0x6517B0);
FunctionPointer(RwReal, RwV3dNormalize, (RwV3d* out, const RwV3d* in), 0x6516A0);
FunctionPointer(void, RtAnimAnimationDestroy, (RtAnimAnimation* pAnim), 0x69DB00);

inline RpMaterial* RpMatFXMaterialSetEnvMapCoefficient(RpMaterial* material, RwReal coef)
{
	int* matfx = *(int**)(MatFXMaterialDataOffset + (char*)&material->texture);
	if (matfx)
		*(RwReal*)(matfx + 2) = coef;
	return material;
}

inline RwFrame* RwFrameSetMatrix(RwFrame* frame, const RwMatrix* modelling)
{
	frame->modelling = *modelling;
	frame->object.flags |= rwFRAMEOBJECTTYPEHIERARCHYDIRTY;
	RwFrameUpdateObjects(frame);
	return frame;
}

inline RwV3d* RwV3dAdd(RwV3d* out, const RwV3d* a, const RwV3d* b)
{
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
	return out;
}

inline RwFrame* RpClumpGetFrame(const RpClump* clump)
{
	return (RwFrame*)clump->object.parent;
}