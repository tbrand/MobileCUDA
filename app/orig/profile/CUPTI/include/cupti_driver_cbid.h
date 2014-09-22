
// *************************************************************************
//      Definitions of indices for API functions, unique across entire API
// *************************************************************************

// This file is generated.  Any changes you make will be lost during the next clean build.
// CUDA public interface, for type definitions and cu* function prototypes

typedef enum CUpti_driver_api_trace_cbid_enum {
    CUPTI_DRIVER_TRACE_CBID_INVALID                                                        = 0,
    CUPTI_DRIVER_TRACE_CBID_cuInit                                                         = 1,
    CUPTI_DRIVER_TRACE_CBID_cuDriverGetVersion                                             = 2,
    CUPTI_DRIVER_TRACE_CBID_cuDeviceGet                                                    = 3,
    CUPTI_DRIVER_TRACE_CBID_cuDeviceGetCount                                               = 4,
    CUPTI_DRIVER_TRACE_CBID_cuDeviceGetName                                                = 5,
    CUPTI_DRIVER_TRACE_CBID_cuDeviceComputeCapability                                      = 6,
    CUPTI_DRIVER_TRACE_CBID_cuDeviceTotalMem                                               = 7,
    CUPTI_DRIVER_TRACE_CBID_cuDeviceGetProperties                                          = 8,
    CUPTI_DRIVER_TRACE_CBID_cuDeviceGetAttribute                                           = 9,
    CUPTI_DRIVER_TRACE_CBID_cuCtxCreate                                                    = 10,
    CUPTI_DRIVER_TRACE_CBID_cuCtxDestroy                                                   = 11,
    CUPTI_DRIVER_TRACE_CBID_cuCtxAttach                                                    = 12,
    CUPTI_DRIVER_TRACE_CBID_cuCtxDetach                                                    = 13,
    CUPTI_DRIVER_TRACE_CBID_cuCtxPushCurrent                                               = 14,
    CUPTI_DRIVER_TRACE_CBID_cuCtxPopCurrent                                                = 15,
    CUPTI_DRIVER_TRACE_CBID_cuCtxGetDevice                                                 = 16,
    CUPTI_DRIVER_TRACE_CBID_cuCtxSynchronize                                               = 17,
    CUPTI_DRIVER_TRACE_CBID_cuModuleLoad                                                   = 18,
    CUPTI_DRIVER_TRACE_CBID_cuModuleLoadData                                               = 19,
    CUPTI_DRIVER_TRACE_CBID_cuModuleLoadDataEx                                             = 20,
    CUPTI_DRIVER_TRACE_CBID_cuModuleLoadFatBinary                                          = 21,
    CUPTI_DRIVER_TRACE_CBID_cuModuleUnload                                                 = 22,
    CUPTI_DRIVER_TRACE_CBID_cuModuleGetFunction                                            = 23,
    CUPTI_DRIVER_TRACE_CBID_cuModuleGetGlobal                                              = 24,
    CUPTI_DRIVER_TRACE_CBID_cu64ModuleGetGlobal                                            = 25,
    CUPTI_DRIVER_TRACE_CBID_cuModuleGetTexRef                                              = 26,
    CUPTI_DRIVER_TRACE_CBID_cuMemGetInfo                                                   = 27,
    CUPTI_DRIVER_TRACE_CBID_cu64MemGetInfo                                                 = 28,
    CUPTI_DRIVER_TRACE_CBID_cuMemAlloc                                                     = 29,
    CUPTI_DRIVER_TRACE_CBID_cu64MemAlloc                                                   = 30,
    CUPTI_DRIVER_TRACE_CBID_cuMemAllocPitch                                                = 31,
    CUPTI_DRIVER_TRACE_CBID_cu64MemAllocPitch                                              = 32,
    CUPTI_DRIVER_TRACE_CBID_cuMemFree                                                      = 33,
    CUPTI_DRIVER_TRACE_CBID_cu64MemFree                                                    = 34,
    CUPTI_DRIVER_TRACE_CBID_cuMemGetAddressRange                                           = 35,
    CUPTI_DRIVER_TRACE_CBID_cu64MemGetAddressRange                                         = 36,
    CUPTI_DRIVER_TRACE_CBID_cuMemAllocHost                                                 = 37,
    CUPTI_DRIVER_TRACE_CBID_cuMemFreeHost                                                  = 38,
    CUPTI_DRIVER_TRACE_CBID_cuMemHostAlloc                                                 = 39,
    CUPTI_DRIVER_TRACE_CBID_cuMemHostGetDevicePointer                                      = 40,
    CUPTI_DRIVER_TRACE_CBID_cu64MemHostGetDevicePointer                                    = 41,
    CUPTI_DRIVER_TRACE_CBID_cuMemHostGetFlags                                              = 42,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyHtoD                                                   = 43,
    CUPTI_DRIVER_TRACE_CBID_cu64MemcpyHtoD                                                 = 44,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyDtoH                                                   = 45,
    CUPTI_DRIVER_TRACE_CBID_cu64MemcpyDtoH                                                 = 46,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyDtoD                                                   = 47,
    CUPTI_DRIVER_TRACE_CBID_cu64MemcpyDtoD                                                 = 48,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyDtoA                                                   = 49,
    CUPTI_DRIVER_TRACE_CBID_cu64MemcpyDtoA                                                 = 50,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyAtoD                                                   = 51,
    CUPTI_DRIVER_TRACE_CBID_cu64MemcpyAtoD                                                 = 52,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyHtoA                                                   = 53,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyAtoH                                                   = 54,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyAtoA                                                   = 55,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy2D                                                     = 56,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy2DUnaligned                                            = 57,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy3D                                                     = 58,
    CUPTI_DRIVER_TRACE_CBID_cu64Memcpy3D                                                   = 59,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyHtoDAsync                                              = 60,
    CUPTI_DRIVER_TRACE_CBID_cu64MemcpyHtoDAsync                                            = 61,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyDtoHAsync                                              = 62,
    CUPTI_DRIVER_TRACE_CBID_cu64MemcpyDtoHAsync                                            = 63,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyDtoDAsync                                              = 64,
    CUPTI_DRIVER_TRACE_CBID_cu64MemcpyDtoDAsync                                            = 65,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyHtoAAsync                                              = 66,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyAtoHAsync                                              = 67,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy2DAsync                                                = 68,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy3DAsync                                                = 69,
    CUPTI_DRIVER_TRACE_CBID_cu64Memcpy3DAsync                                              = 70,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD8                                                     = 71,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD8                                                   = 72,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD16                                                    = 73,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD16                                                  = 74,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD32                                                    = 75,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD32                                                  = 76,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD2D8                                                   = 77,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD2D8                                                 = 78,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD2D16                                                  = 79,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD2D16                                                = 80,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD2D32                                                  = 81,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD2D32                                                = 82,
    CUPTI_DRIVER_TRACE_CBID_cuFuncSetBlockShape                                            = 83,
    CUPTI_DRIVER_TRACE_CBID_cuFuncSetSharedSize                                            = 84,
    CUPTI_DRIVER_TRACE_CBID_cuFuncGetAttribute                                             = 85,
    CUPTI_DRIVER_TRACE_CBID_cuFuncSetCacheConfig                                           = 86,
    CUPTI_DRIVER_TRACE_CBID_cuArrayCreate                                                  = 87,
    CUPTI_DRIVER_TRACE_CBID_cuArrayGetDescriptor                                           = 88,
    CUPTI_DRIVER_TRACE_CBID_cuArrayDestroy                                                 = 89,
    CUPTI_DRIVER_TRACE_CBID_cuArray3DCreate                                                = 90,
    CUPTI_DRIVER_TRACE_CBID_cuArray3DGetDescriptor                                         = 91,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefCreate                                                 = 92,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefDestroy                                                = 93,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetArray                                               = 94,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetAddress                                             = 95,
    CUPTI_DRIVER_TRACE_CBID_cu64TexRefSetAddress                                           = 96,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetAddress2D                                           = 97,
    CUPTI_DRIVER_TRACE_CBID_cu64TexRefSetAddress2D                                         = 98,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetFormat                                              = 99,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetAddressMode                                         = 100,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetFilterMode                                          = 101,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetFlags                                               = 102,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetAddress                                             = 103,
    CUPTI_DRIVER_TRACE_CBID_cu64TexRefGetAddress                                           = 104,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetArray                                               = 105,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetAddressMode                                         = 106,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetFilterMode                                          = 107,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetFormat                                              = 108,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetFlags                                               = 109,
    CUPTI_DRIVER_TRACE_CBID_cuParamSetSize                                                 = 110,
    CUPTI_DRIVER_TRACE_CBID_cuParamSeti                                                    = 111,
    CUPTI_DRIVER_TRACE_CBID_cuParamSetf                                                    = 112,
    CUPTI_DRIVER_TRACE_CBID_cuParamSetv                                                    = 113,
    CUPTI_DRIVER_TRACE_CBID_cuParamSetTexRef                                               = 114,
    CUPTI_DRIVER_TRACE_CBID_cuLaunch                                                       = 115,
    CUPTI_DRIVER_TRACE_CBID_cuLaunchGrid                                                   = 116,
    CUPTI_DRIVER_TRACE_CBID_cuLaunchGridAsync                                              = 117,
    CUPTI_DRIVER_TRACE_CBID_cuEventCreate                                                  = 118,
    CUPTI_DRIVER_TRACE_CBID_cuEventRecord                                                  = 119,
    CUPTI_DRIVER_TRACE_CBID_cuEventQuery                                                   = 120,
    CUPTI_DRIVER_TRACE_CBID_cuEventSynchronize                                             = 121,
    CUPTI_DRIVER_TRACE_CBID_cuEventDestroy                                                 = 122,
    CUPTI_DRIVER_TRACE_CBID_cuEventElapsedTime                                             = 123,
    CUPTI_DRIVER_TRACE_CBID_cuStreamCreate                                                 = 124,
    CUPTI_DRIVER_TRACE_CBID_cuStreamQuery                                                  = 125,
    CUPTI_DRIVER_TRACE_CBID_cuStreamSynchronize                                            = 126,
    CUPTI_DRIVER_TRACE_CBID_cuStreamDestroy                                                = 127,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsUnregisterResource                                   = 128,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsSubResourceGetMappedArray                            = 129,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsResourceGetMappedPointer                             = 130,
    CUPTI_DRIVER_TRACE_CBID_cu64GraphicsResourceGetMappedPointer                           = 131,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsResourceSetMapFlags                                  = 132,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsMapResources                                         = 133,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsUnmapResources                                       = 134,
    CUPTI_DRIVER_TRACE_CBID_cuGetExportTable                                               = 135,
    CUPTI_DRIVER_TRACE_CBID_cuCtxSetLimit                                                  = 136,
    CUPTI_DRIVER_TRACE_CBID_cuCtxGetLimit                                                  = 137,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10GetDevice                                               = 138,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10CtxCreate                                               = 139,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsD3D10RegisterResource                                = 140,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10RegisterResource                                        = 141,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10UnregisterResource                                      = 142,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10MapResources                                            = 143,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10UnmapResources                                          = 144,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10ResourceSetMapFlags                                     = 145,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10ResourceGetMappedArray                                  = 146,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10ResourceGetMappedPointer                                = 147,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10ResourceGetMappedSize                                   = 148,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10ResourceGetMappedPitch                                  = 149,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10ResourceGetSurfaceDimensions                            = 150,
    CUPTI_DRIVER_TRACE_CBID_cuD3D11GetDevice                                               = 151,
    CUPTI_DRIVER_TRACE_CBID_cuD3D11CtxCreate                                               = 152,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsD3D11RegisterResource                                = 153,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9GetDevice                                                = 154,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9CtxCreate                                                = 155,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsD3D9RegisterResource                                 = 156,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9GetDirect3DDevice                                        = 157,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9RegisterResource                                         = 158,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9UnregisterResource                                       = 159,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9MapResources                                             = 160,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9UnmapResources                                           = 161,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9ResourceSetMapFlags                                      = 162,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9ResourceGetSurfaceDimensions                             = 163,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9ResourceGetMappedArray                                   = 164,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9ResourceGetMappedPointer                                 = 165,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9ResourceGetMappedSize                                    = 166,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9ResourceGetMappedPitch                                   = 167,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9Begin                                                    = 168,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9End                                                      = 169,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9RegisterVertexBuffer                                     = 170,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9MapVertexBuffer                                          = 171,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9UnmapVertexBuffer                                        = 172,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9UnregisterVertexBuffer                                   = 173,
    CUPTI_DRIVER_TRACE_CBID_cuGLCtxCreate                                                  = 174,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsGLRegisterBuffer                                     = 175,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsGLRegisterImage                                      = 176,
    CUPTI_DRIVER_TRACE_CBID_cuWGLGetDevice                                                 = 177,
    CUPTI_DRIVER_TRACE_CBID_cuGLInit                                                       = 178,
    CUPTI_DRIVER_TRACE_CBID_cuGLRegisterBufferObject                                       = 179,
    CUPTI_DRIVER_TRACE_CBID_cuGLMapBufferObject                                            = 180,
    CUPTI_DRIVER_TRACE_CBID_cuGLUnmapBufferObject                                          = 181,
    CUPTI_DRIVER_TRACE_CBID_cuGLUnregisterBufferObject                                     = 182,
    CUPTI_DRIVER_TRACE_CBID_cuGLSetBufferObjectMapFlags                                    = 183,
    CUPTI_DRIVER_TRACE_CBID_cuGLMapBufferObjectAsync                                       = 184,
    CUPTI_DRIVER_TRACE_CBID_cuGLUnmapBufferObjectAsync                                     = 185,
    CUPTI_DRIVER_TRACE_CBID_cuVDPAUGetDevice                                               = 186,
    CUPTI_DRIVER_TRACE_CBID_cuVDPAUCtxCreate                                               = 187,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsVDPAURegisterVideoSurface                            = 188,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsVDPAURegisterOutputSurface                           = 189,
    CUPTI_DRIVER_TRACE_CBID_cuModuleGetSurfRef                                             = 190,
    CUPTI_DRIVER_TRACE_CBID_cuSurfRefCreate                                                = 191,
    CUPTI_DRIVER_TRACE_CBID_cuSurfRefDestroy                                               = 192,
    CUPTI_DRIVER_TRACE_CBID_cuSurfRefSetFormat                                             = 193,
    CUPTI_DRIVER_TRACE_CBID_cuSurfRefSetArray                                              = 194,
    CUPTI_DRIVER_TRACE_CBID_cuSurfRefGetFormat                                             = 195,
    CUPTI_DRIVER_TRACE_CBID_cuSurfRefGetArray                                              = 196,
    CUPTI_DRIVER_TRACE_CBID_cu64DeviceTotalMem                                             = 197,
    CUPTI_DRIVER_TRACE_CBID_cu64D3D10ResourceGetMappedPointer                              = 198,
    CUPTI_DRIVER_TRACE_CBID_cu64D3D10ResourceGetMappedSize                                 = 199,
    CUPTI_DRIVER_TRACE_CBID_cu64D3D10ResourceGetMappedPitch                                = 200,
    CUPTI_DRIVER_TRACE_CBID_cu64D3D10ResourceGetSurfaceDimensions                          = 201,
    CUPTI_DRIVER_TRACE_CBID_cu64D3D9ResourceGetSurfaceDimensions                           = 202,
    CUPTI_DRIVER_TRACE_CBID_cu64D3D9ResourceGetMappedPointer                               = 203,
    CUPTI_DRIVER_TRACE_CBID_cu64D3D9ResourceGetMappedSize                                  = 204,
    CUPTI_DRIVER_TRACE_CBID_cu64D3D9ResourceGetMappedPitch                                 = 205,
    CUPTI_DRIVER_TRACE_CBID_cu64D3D9MapVertexBuffer                                        = 206,
    CUPTI_DRIVER_TRACE_CBID_cu64GLMapBufferObject                                          = 207,
    CUPTI_DRIVER_TRACE_CBID_cu64GLMapBufferObjectAsync                                     = 208,
    CUPTI_DRIVER_TRACE_CBID_cuD3D11GetDevices                                              = 209,
    CUPTI_DRIVER_TRACE_CBID_cuD3D11CtxCreateOnDevice                                       = 210,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10GetDevices                                              = 211,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10CtxCreateOnDevice                                       = 212,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9GetDevices                                               = 213,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9CtxCreateOnDevice                                        = 214,
    CUPTI_DRIVER_TRACE_CBID_cu64MemHostAlloc                                               = 215,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD8Async                                                = 216,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD8Async                                              = 217,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD16Async                                               = 218,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD16Async                                             = 219,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD32Async                                               = 220,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD32Async                                             = 221,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD2D8Async                                              = 222,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD2D8Async                                            = 223,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD2D16Async                                             = 224,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD2D16Async                                           = 225,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD2D32Async                                             = 226,
    CUPTI_DRIVER_TRACE_CBID_cu64MemsetD2D32Async                                           = 227,
    CUPTI_DRIVER_TRACE_CBID_cu64ArrayCreate                                                = 228,
    CUPTI_DRIVER_TRACE_CBID_cu64ArrayGetDescriptor                                         = 229,
    CUPTI_DRIVER_TRACE_CBID_cu64Array3DCreate                                              = 230,
    CUPTI_DRIVER_TRACE_CBID_cu64Array3DGetDescriptor                                       = 231,
    CUPTI_DRIVER_TRACE_CBID_cu64Memcpy2D                                                   = 232,
    CUPTI_DRIVER_TRACE_CBID_cu64Memcpy2DUnaligned                                          = 233,
    CUPTI_DRIVER_TRACE_CBID_cu64Memcpy2DAsync                                              = 234,
    CUPTI_DRIVER_TRACE_CBID_cuCtxCreate_v2                                                 = 235,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10CtxCreate_v2                                            = 236,
    CUPTI_DRIVER_TRACE_CBID_cuD3D11CtxCreate_v2                                            = 237,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9CtxCreate_v2                                             = 238,
    CUPTI_DRIVER_TRACE_CBID_cuGLCtxCreate_v2                                               = 239,
    CUPTI_DRIVER_TRACE_CBID_cuVDPAUCtxCreate_v2                                            = 240,
    CUPTI_DRIVER_TRACE_CBID_cuModuleGetGlobal_v2                                           = 241,
    CUPTI_DRIVER_TRACE_CBID_cuMemGetInfo_v2                                                = 242,
    CUPTI_DRIVER_TRACE_CBID_cuMemAlloc_v2                                                  = 243,
    CUPTI_DRIVER_TRACE_CBID_cuMemAllocPitch_v2                                             = 244,
    CUPTI_DRIVER_TRACE_CBID_cuMemFree_v2                                                   = 245,
    CUPTI_DRIVER_TRACE_CBID_cuMemGetAddressRange_v2                                        = 246,
    CUPTI_DRIVER_TRACE_CBID_cuMemHostGetDevicePointer_v2                                   = 247,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy_v2                                                    = 248,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD8_v2                                                  = 249,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD16_v2                                                 = 250,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD32_v2                                                 = 251,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD2D8_v2                                                = 252,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD2D16_v2                                               = 253,
    CUPTI_DRIVER_TRACE_CBID_cuMemsetD2D32_v2                                               = 254,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetAddress_v2                                          = 255,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetAddress2D_v2                                        = 256,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetAddress_v2                                          = 257,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsResourceGetMappedPointer_v2                          = 258,
    CUPTI_DRIVER_TRACE_CBID_cuDeviceTotalMem_v2                                            = 259,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10ResourceGetMappedPointer_v2                             = 260,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10ResourceGetMappedSize_v2                                = 261,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10ResourceGetMappedPitch_v2                               = 262,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10ResourceGetSurfaceDimensions_v2                         = 263,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9ResourceGetSurfaceDimensions_v2                          = 264,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9ResourceGetMappedPointer_v2                              = 265,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9ResourceGetMappedSize_v2                                 = 266,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9ResourceGetMappedPitch_v2                                = 267,
    CUPTI_DRIVER_TRACE_CBID_cuD3D9MapVertexBuffer_v2                                       = 268,
    CUPTI_DRIVER_TRACE_CBID_cuGLMapBufferObject_v2                                         = 269,
    CUPTI_DRIVER_TRACE_CBID_cuGLMapBufferObjectAsync_v2                                    = 270,
    CUPTI_DRIVER_TRACE_CBID_cuMemHostAlloc_v2                                              = 271,
    CUPTI_DRIVER_TRACE_CBID_cuArrayCreate_v2                                               = 272,
    CUPTI_DRIVER_TRACE_CBID_cuArrayGetDescriptor_v2                                        = 273,
    CUPTI_DRIVER_TRACE_CBID_cuArray3DCreate_v2                                             = 274,
    CUPTI_DRIVER_TRACE_CBID_cuArray3DGetDescriptor_v2                                      = 275,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyHtoD_v2                                                = 276,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyHtoDAsync_v2                                           = 277,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyDtoH_v2                                                = 278,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyDtoHAsync_v2                                           = 279,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyDtoD_v2                                                = 280,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyDtoDAsync_v2                                           = 281,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyAtoH_v2                                                = 282,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyAtoHAsync_v2                                           = 283,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyAtoD_v2                                                = 284,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyDtoA_v2                                                = 285,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyAtoA_v2                                                = 286,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy2D_v2                                                  = 287,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy2DUnaligned_v2                                         = 288,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy2DAsync_v2                                             = 289,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy3D_v2                                                  = 290,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy3DAsync_v2                                             = 291,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyHtoA_v2                                                = 292,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyHtoAAsync_v2                                           = 293,
    CUPTI_DRIVER_TRACE_CBID_cuMemAllocHost_v2                                              = 294,
    CUPTI_DRIVER_TRACE_CBID_cuStreamWaitEvent                                              = 295,
    CUPTI_DRIVER_TRACE_CBID_cuCtxGetApiVersion                                             = 296,
    CUPTI_DRIVER_TRACE_CBID_cuD3D10GetDirect3DDevice                                       = 297,
    CUPTI_DRIVER_TRACE_CBID_cuD3D11GetDirect3DDevice                                       = 298,
    CUPTI_DRIVER_TRACE_CBID_cuCtxGetCacheConfig                                            = 299,
    CUPTI_DRIVER_TRACE_CBID_cuCtxSetCacheConfig                                            = 300,
    CUPTI_DRIVER_TRACE_CBID_cuMemHostRegister                                              = 301,
    CUPTI_DRIVER_TRACE_CBID_cuMemHostUnregister                                            = 302,
    CUPTI_DRIVER_TRACE_CBID_cuCtxSetCurrent                                                = 303,
    CUPTI_DRIVER_TRACE_CBID_cuCtxGetCurrent                                                = 304,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy                                                       = 305,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyAsync                                                  = 306,
    CUPTI_DRIVER_TRACE_CBID_cuLaunchKernel                                                 = 307,
    CUPTI_DRIVER_TRACE_CBID_cuProfilerStart                                                = 308,
    CUPTI_DRIVER_TRACE_CBID_cuProfilerStop                                                 = 309,
    CUPTI_DRIVER_TRACE_CBID_cuPointerGetAttribute                                          = 310,
    CUPTI_DRIVER_TRACE_CBID_cuProfilerInitialize                                           = 311,
    CUPTI_DRIVER_TRACE_CBID_cuDeviceCanAccessPeer                                          = 312,
    CUPTI_DRIVER_TRACE_CBID_cuCtxEnablePeerAccess                                          = 313,
    CUPTI_DRIVER_TRACE_CBID_cuCtxDisablePeerAccess                                         = 314,
    CUPTI_DRIVER_TRACE_CBID_cuMemPeerRegister                                              = 315,
    CUPTI_DRIVER_TRACE_CBID_cuMemPeerUnregister                                            = 316,
    CUPTI_DRIVER_TRACE_CBID_cuMemPeerGetDevicePointer                                      = 317,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyPeer                                                   = 318,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpyPeerAsync                                              = 319,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy3DPeer                                                 = 320,
    CUPTI_DRIVER_TRACE_CBID_cuMemcpy3DPeerAsync                                            = 321,
    CUPTI_DRIVER_TRACE_CBID_cuCtxDestroy_v2                                                = 322,
    CUPTI_DRIVER_TRACE_CBID_cuCtxPushCurrent_v2                                            = 323,
    CUPTI_DRIVER_TRACE_CBID_cuCtxPopCurrent_v2                                             = 324,
    CUPTI_DRIVER_TRACE_CBID_cuEventDestroy_v2                                              = 325,
    CUPTI_DRIVER_TRACE_CBID_cuStreamDestroy_v2                                             = 326,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetAddress2D_v3                                        = 327,
    CUPTI_DRIVER_TRACE_CBID_cuIpcGetMemHandle                                              = 328,
    CUPTI_DRIVER_TRACE_CBID_cuIpcOpenMemHandle                                             = 329,
    CUPTI_DRIVER_TRACE_CBID_cuIpcCloseMemHandle                                            = 330,
    CUPTI_DRIVER_TRACE_CBID_cuDeviceGetByPCIBusId                                          = 331,
    CUPTI_DRIVER_TRACE_CBID_cuDeviceGetPCIBusId                                            = 332,
    CUPTI_DRIVER_TRACE_CBID_cuGLGetDevices                                                 = 333,
    CUPTI_DRIVER_TRACE_CBID_cuIpcGetEventHandle                                            = 334,
    CUPTI_DRIVER_TRACE_CBID_cuIpcOpenEventHandle                                           = 335,
    CUPTI_DRIVER_TRACE_CBID_cuCtxSetSharedMemConfig                                        = 336,
    CUPTI_DRIVER_TRACE_CBID_cuCtxGetSharedMemConfig                                        = 337,
    CUPTI_DRIVER_TRACE_CBID_cuFuncSetSharedMemConfig                                       = 338,
    CUPTI_DRIVER_TRACE_CBID_cuTexObjectCreate                                              = 339,
    CUPTI_DRIVER_TRACE_CBID_cuTexObjectDestroy                                             = 340,
    CUPTI_DRIVER_TRACE_CBID_cuTexObjectGetResourceDesc                                     = 341,
    CUPTI_DRIVER_TRACE_CBID_cuTexObjectGetTextureDesc                                      = 342,
    CUPTI_DRIVER_TRACE_CBID_cuSurfObjectCreate                                             = 343,
    CUPTI_DRIVER_TRACE_CBID_cuSurfObjectDestroy                                            = 344,
    CUPTI_DRIVER_TRACE_CBID_cuSurfObjectGetResourceDesc                                    = 345,
    CUPTI_DRIVER_TRACE_CBID_cuStreamAddCallback                                            = 346,
    CUPTI_DRIVER_TRACE_CBID_cuMipmappedArrayCreate                                         = 347,
    CUPTI_DRIVER_TRACE_CBID_cuMipmappedArrayGetLevel                                       = 348,
    CUPTI_DRIVER_TRACE_CBID_cuMipmappedArrayDestroy                                        = 349,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetMipmappedArray                                      = 350,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetMipmapFilterMode                                    = 351,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetMipmapLevelBias                                     = 352,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetMipmapLevelClamp                                    = 353,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefSetMaxAnisotropy                                       = 354,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetMipmappedArray                                      = 355,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetMipmapFilterMode                                    = 356,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetMipmapLevelBias                                     = 357,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetMipmapLevelClamp                                    = 358,
    CUPTI_DRIVER_TRACE_CBID_cuTexRefGetMaxAnisotropy                                       = 359,
    CUPTI_DRIVER_TRACE_CBID_cuGraphicsResourceGetMappedMipmappedArray                      = 360,
    CUPTI_DRIVER_TRACE_CBID_cuTexObjectGetResourceViewDesc                                 = 361,
    CUPTI_DRIVER_TRACE_CBID_cuLinkCreate                                                   = 362,
    CUPTI_DRIVER_TRACE_CBID_cuLinkAddData                                                  = 363,
    CUPTI_DRIVER_TRACE_CBID_cuLinkAddFile                                                  = 364,
    CUPTI_DRIVER_TRACE_CBID_cuLinkComplete                                                 = 365,
    CUPTI_DRIVER_TRACE_CBID_cuLinkDestroy                                                  = 366,
    CUPTI_DRIVER_TRACE_CBID_SIZE                                                           = 367,
    CUPTI_DRIVER_TRACE_CBID_FORCE_INT                                                      = 0x7fffffff
} CUpti_driver_api_trace_cbid;

