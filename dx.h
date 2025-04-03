#pragma once
#include "shenmue.h"
#include "s1/dx_defines.h"

struct /*VFT*/ ppfx_vtbl
{
    __int64(__fastcall* Initialize)();
    __int64(__fastcall* Uninitialize)();
};

struct ScreenDimensions
{
    struct
    {
        int Width;
        int Height;
    };
};

struct struct_config
{
    int32_t width;
    int32_t height;
    int32_t Numerator;
    int32_t Denominator;
};

typedef void(__fastcall* dxinit_t)(struct_config* config);
extern dxinit_t dx_initorig;
extern simple_fun_t create_annotations_orig;
extern simple_fun_t dx_create_buffer_orig;
extern simple_fun_t create_dx_buffers_orig;

void __fastcall DX11_D3D11CreateDeviceAndSwapChain(struct_config* config);

D3D11_COMPARISON_FUNC __fastcall GetStencilPassOp(int type);
D3D11_STENCIL_OP __fastcall DecodeStencilOp(int a1);
ID3D11SamplerState* __fastcall CreateSamplerState(const uint64_t* pFlag);

void __fastcall CreateUserDefinedAnnotations();
void __fastcall DXCreateBuffer();
void __fastcall CreateDXBuffers();
void __fastcall CreateNewSRVTexture();

void ApplyFramepacing(int renderMode);
void __fastcall d3tPresent(int renderMode);


#if _DEBUG
    void HandleDebugDraw();
    void DebugDraw();
#endif





DECLARE_VAR(LARGE_INTEGER, g_dx_init_time);
DECLARE_VAR(ppfx_vtbl, g_PPFX_vtable);
DECLARE_VAR(ppfx_vtbl*, g_PPFX);
DECLARE_VAR(ScreenDimensions, OrigScreenDimensions);
DECLARE_VAR(HWND, hWnd);
DECLARE_VAR(DXGI_FORMAT, g_DefaultTextureFormat);
DECLARE_VAR(IDXGISwapChain*, ppSwapChain);
DECLARE_VAR(ID3D11Device*, ppDevice);
DECLARE_VAR(ID3D11DeviceContext*, pDeviceContext);
DECLARE_VAR(ID3D11RenderTargetView*, g_ppRenderTargetView);
DECLARE_VAR(ID3D11ShaderResourceView*, g_ppShaderResourceView);
DECLARE_VAR(ID3D11Texture2D*, g_ppBackBufferTexture2D);
DECLARE_VAR(ID3DUserDefinedAnnotation*, ppUserAnnotation);
DECLARE_VAR(ID3D11Query* [3], ppOcclusionQueries);
DECLARE_VAR(ID3D11Texture2D*, g_ppTexture2D);
DECLARE_VAR(ID3D11Texture2D*, g_pDxMipsAccessView);
DECLARE_VAR(__int64, g_inner_frame_loop_cnt);
DECLARE_VAR(__int64, g_frame_count);
DECLARE_VAR(float, fFrameTimeMs);
DECLARE_VAR(double, dFrameTime);