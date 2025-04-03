#include "pch.h"
#include "dx.h"

#if _DEBUG
#   include <imgui.h>
#   include <imgui_impl_dx11.h>
#   include <imgui_impl_win32.h>
#endif

dxinit_t dx_initorig;
simple_fun_t create_annotations_orig;
simple_fun_t dx_create_buffer_orig;
simple_fun_t create_dx_buffers_orig;

DEFINE_VAR(LARGE_INTEGER, g_dx_init_time);
DEFINE_VAR(ppfx_vtbl, g_PPFX_vtable);
DEFINE_VAR(ppfx_vtbl*, g_PPFX);
DEFINE_VAR(ScreenDimensions, OrigScreenDimensions);
DEFINE_VAR(HWND, hWnd);
DEFINE_VAR(DXGI_FORMAT, g_DefaultTextureFormat);
DEFINE_VAR(IDXGISwapChain*, ppSwapChain);
DEFINE_VAR(ID3D11Device*, ppDevice);
DEFINE_VAR(ID3D11DeviceContext*, pDeviceContext);
DEFINE_VAR(ID3D11RenderTargetView*, g_ppRenderTargetView);
DEFINE_VAR(ID3D11ShaderResourceView*, g_ppShaderResourceView);
DEFINE_VAR(ID3D11Texture2D*, g_ppBackBufferTexture2D);
DEFINE_VAR(ID3DUserDefinedAnnotation*, ppUserAnnotation);
DEFINE_VAR(ID3D11Query* [3], ppOcclusionQueries);
DEFINE_VAR(ID3D11Texture2D*, g_ppTexture2D);
DEFINE_VAR(ID3D11Texture2D*, g_pDxMipsAccessView);
DEFINE_VAR(__int64, g_inner_frame_loop_cnt);
DEFINE_VAR(__int64, g_frame_count);
DEFINE_VAR(float, fFrameTimeMs);
DEFINE_VAR(double, dFrameTime);

#if _DEBUG
static WNDPROC oWndProc = nullptr;

void DebugDraw()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Begin("Debug");
    {
        ImGui::Text("Rendering\n");
        ImGui::Separator();
        ImGui::Text("Avg.: %.3f ms/frame\nFPS: %.1f (Real: %f (%f))", 1000.0f / io.Framerate, io.Framerate, fFrameTimeMs(), dFrameTime());
    }
    ImGui::Separator();
    ImGui::End();
}

void HandleDebugDraw()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    DebugDraw();
    ImGui::Render();

    pDeviceContext()->OMSetRenderTargets(1, &g_ppRenderTargetView(), nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
#endif


void __fastcall CreateUserDefinedAnnotations()
{
    create_annotations_orig();
}

void __fastcall DXCreateBuffer()
{
    dx_create_buffer_orig();
}

void __fastcall CreateDXBuffers()
{
    create_dx_buffers_orig();
}

// @confirmed
void __fastcall CreateNewSRVTexture()
{
    D3D11_TEXTURE2D_DESC currDesc;
    g_ppBackBufferTexture2D()->GetDesc(&currDesc);

    D3D11_TEXTURE2D_DESC desc;
    desc.BindFlags = 32;
    desc.CPUAccessFlags = 0;
    desc.Format = currDesc.Format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.Width = currDesc.Width;
    desc.Height = currDesc.Height;
    desc.MipLevels = currDesc.MipLevels;
    desc.ArraySize = currDesc.ArraySize;
    desc.MiscFlags = 0;
    ppDevice()->CreateTexture2D(&desc, 0LL, &g_pDxMipsAccessView());


    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = 0x20000;
    ppDevice()->CreateTexture2D(&desc, 0LL, &g_ppTexture2D());
}


#if _DEBUG
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI HookedWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureKeyboard || io.WantCaptureMouse)
            return true;
    }
    return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}
#endif

// @confirmed
FUNC void __fastcall DX11_D3D11CreateDeviceAndSwapChain(struct_config* config)
{
    Log("Resolution: %dx%d\n", config->width, config->height);


    // [unused]
    QueryPerformanceFrequency(&g_dx_init_time());

    g_PPFX() = &g_PPFX_vtable();
    g_DefaultTextureFormat() = DXGI_FORMAT_R32G32B32A32_UINT;

    DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
    D3D_FEATURE_LEVEL pFeatureLevel;

    // ?
    OrigScreenDimensions().Width = config->height;
    OrigScreenDimensions().Height = config->width;

    pSwapChainDesc.Windowed = TRUE;
    pSwapChainDesc.OutputWindow = hWnd();
    pSwapChainDesc.BufferCount = 1;
    pSwapChainDesc.BufferDesc.Width = config->width;
    pSwapChainDesc.BufferDesc.Height = config->height;
    pSwapChainDesc.BufferDesc.RefreshRate.Numerator = config->Numerator;
    pSwapChainDesc.BufferDesc.RefreshRate.Denominator = config->Denominator;
    pSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    pSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    pSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    pSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    pSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    pSwapChainDesc.SampleDesc.Count = 1;
    pSwapChainDesc.SampleDesc.Quality = 0;
    pSwapChainDesc.Flags = 0;

#   if _DEBUG
#       define D3D_DEBUG_FLAG D3D11_CREATE_DEVICE_DEBUG
#   else
#       define D3D_DEBUG_FLAG 0
#   endif


    D3D_FEATURE_LEVEL pFeatureLevels_1[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    HRESULT   ret = D3D11CreateDeviceAndSwapChain(
        0LL,
        D3D_DRIVER_TYPE_HARDWARE,
        0LL,
        D3D_DEBUG_FLAG,
        pFeatureLevels_1,
        2,
        D3D11_SDK_VERSION,
        &pSwapChainDesc,
        &ppSwapChain(),
        &ppDevice(),
        &pFeatureLevel,
        &pDeviceContext());

    // fallback to DX 11.0
    if (ret == 0x80070057)
    {
        D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
        ret = D3D11CreateDeviceAndSwapChain(
            0LL,
            D3D_DRIVER_TYPE_HARDWARE,
            0LL,
            D3D_DEBUG_FLAG,
            &pFeatureLevels,
            1,
            D3D11_SDK_VERSION,
            &pSwapChainDesc,
            &ppSwapChain(),
            &ppDevice(),
            &pFeatureLevel,
            &pDeviceContext());
    }

    if (ret >= 0)
    {
        // init buffers
        CreateUserDefinedAnnotations();
        DXCreateBuffer();
        CreateDXBuffers();


        // setup backbuffer
        ppSwapChain()->GetBuffer(0, IID_ID3D11Texture2D, (void**)&g_ppBackBufferTexture2D());
        ppDevice()->CreateRenderTargetView(g_ppBackBufferTexture2D(), 0LL, &g_ppRenderTargetView());
        ppDevice()->CreateShaderResourceView(g_ppBackBufferTexture2D(), 0LL, &g_ppShaderResourceView());
        CreateNewSRVTexture();

        // setup viewport
        D3D11_VIEWPORT viewportDesc;
        viewportDesc.MinDepth = 0.0;
        viewportDesc.TopLeftX = 0.0;
        viewportDesc.TopLeftY = 0.0;
        viewportDesc.Width = (float)OrigScreenDimensions().Height;
        viewportDesc.Height = (float)OrigScreenDimensions().Width;
        viewportDesc.MaxDepth = 1.0;
        pDeviceContext()->RSSetViewports(1LL, &viewportDesc);


        // setup depth pass
        ID3D11DepthStencilState* depthStencilState;
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        memset(&depthStencilDesc, 0, 36);
        depthStencilDesc.DepthEnable = TRUE;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        ppDevice()->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
        pDeviceContext()->OMSetDepthStencilState(depthStencilState, 0LL);
        if (depthStencilState)
        {
            depthStencilState->Release();
            depthStencilState = 0LL;
        }

        // setup rasterizer 
        ID3D11RasterizerState* pRasterizerState;
        D3D11_RASTERIZER_DESC pDesc;
        pDesc.ScissorEnable = FALSE;
        pDesc.MultisampleEnable = FALSE;
        pDesc.FrontCounterClockwise = FALSE;
        pDesc.DepthBias = 0;
        pDesc.DepthBiasClamp = 0.0;
        pDesc.SlopeScaledDepthBias = 0.0;
        pDesc.FillMode = D3D11_FILL_SOLID;
        pDesc.CullMode = D3D11_CULL_BACK;
        pDesc.DepthClipEnable = TRUE;
        pDesc.AntialiasedLineEnable = TRUE;
        ppDevice()->CreateRasterizerState(&pDesc, &pRasterizerState);
        pDeviceContext()->RSSetState(pRasterizerState);
        if (pRasterizerState)
        {
            pRasterizerState->Release();
            pRasterizerState = 0LL;
        }

        // setup annotations
        ret = pDeviceContext()->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), (void**)&ppUserAnnotation());
        auto v6 = ppUserAnnotation();
        if (ret < 0)
            v6 = 0LL;
        ppUserAnnotation() = v6;


        // setup occlusion queries
        ID3D11Query** tmp_query; // rdi
        D3D11_QUERY_DESC queryDesc;
        queryDesc.MiscFlags = 0;
        queryDesc.Query = D3D11_QUERY_OCCLUSION;
        tmp_query = ppOcclusionQueries();
        for (int queryIdx = 0; queryIdx < 3; ++queryIdx) {
            if (ppDevice()->CreateQuery(&queryDesc, &ppOcclusionQueries()[queryIdx]) < 0)
                *tmp_query = 0LL;
            ++tmp_query;
        }

#       if _DEBUG
            IMGUI_CHECKVERSION();
            oWndProc = (WNDPROC)SetWindowLongPtr(hWnd(), GWLP_WNDPROC, (LONG_PTR)HookedWndProc);
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            ImGui::StyleColorsDark();
            ImGui_ImplWin32_Init(hWnd());
            ImGui_ImplDX11_Init(ppDevice(), pDeviceContext());
#       endif

        Log("DirectX initialized\n");
    }
}


// @confirmed
FUNC D3D11_COMPARISON_FUNC __fastcall GetStencilPassOp(int type)
{
    D3D11_COMPARISON_FUNC result = D3D11_COMPARISON_ALWAYS; // eax

    switch (type)
    {
        case 1:
            result = D3D11_COMPARISON_NEVER;
            break;
        case 2:
            result = D3D11_COMPARISON_LESS;
            break;
        case 3:
            result = D3D11_COMPARISON_EQUAL;
            break;
        case 4:
            result = D3D11_COMPARISON_LESS_EQUAL;
            break;
        case 5:
            result = D3D11_COMPARISON_GREATER;
            break;
        case 6:
            result = D3D11_COMPARISON_NOT_EQUAL;
            break;
        case 7:
            result = D3D11_COMPARISON_GREATER_EQUAL;
            break;
    }
    return result;
}

// @confirmed
FUNC D3D11_STENCIL_OP __fastcall DecodeStencilOp(int a1)
{
    D3D11_STENCIL_OP result = D3D11_STENCIL_OP_DECR; // eax

    switch (a1)
    {
        case 0:
            result = D3D11_STENCIL_OP_KEEP;
            break;
        case 1:
            result = D3D11_STENCIL_OP_ZERO;
            break;
        case 2:
            result = D3D11_STENCIL_OP_REPLACE;
            break;
        case 3:
            result = D3D11_STENCIL_OP_INCR_SAT;
            break;
        case 4:
            result = D3D11_STENCIL_OP_DECR_SAT;
            break;
        case 5:
            result = D3D11_STENCIL_OP_INVERT;
            break;
        case 6:
            result = D3D11_STENCIL_OP_INCR;
            break;
    }
    return result;
}

// @confirmed
FUNC ID3D11SamplerState* __fastcall CreateSamplerState(const uint64_t* pFlag)
{
    uint64_t flag = *pFlag;
    D3D11_SAMPLER_DESC desc = {};
    ID3D11SamplerState* sampler = nullptr;

    uint32_t filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    uint32_t flagType = (flag >> 4) & 0x3;

    if (flagType == 1)
        filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

    if ((flag & 0x3) == 1)
        filter |= D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

    if (((flag >> 2) & 0x3) == 1)
        filter |= D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

    if (flagType == 2)
        filter = D3D11_FILTER_ANISOTROPIC;

    int type = (int)((flag >> 10) & 0xF);
    if (type != 0)
        filter |= D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

    desc.Filter = (D3D11_FILTER)filter;

    D3D11_TEXTURE_ADDRESS_MODE addressU = D3D11_TEXTURE_ADDRESS_WRAP;
    uint32_t addrUbits = (uint32_t)((flag >> 6) & 0x3);
    if (addrUbits == 1)
        addressU = D3D11_TEXTURE_ADDRESS_MIRROR;
    else if (addrUbits == 2)
        addressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressU = addressU;

    D3D11_TEXTURE_ADDRESS_MODE addressV = D3D11_TEXTURE_ADDRESS_WRAP;
    uint32_t addrVbits = (uint32_t)((flag >> 8) & 0x3);
    if (addrVbits == 1)
        addressV = D3D11_TEXTURE_ADDRESS_MIRROR;
    else if (addrVbits == 2)
        addressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressV = addressV;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;


    desc.MipLODBias = (float)((flag >> 18) & 0xFFFFFFFF);
    desc.MaxAnisotropy = (UINT)(((flag >> 14) & 0xF) + 1);

    desc.ComparisonFunc = GetStencilPassOp(type);
    desc.BorderColor[0] = 0.0f;
    desc.BorderColor[1] = 0.0f;
    desc.BorderColor[2] = 0.0f;
    desc.BorderColor[3] = 0.0f;

    desc.MinLOD = 0.0f;
    desc.MaxLOD = FLT_MAX;

    if (ppDevice()) {
        ppDevice()->CreateSamplerState(&desc, &sampler);
    }
    return sampler;
}

// @confirmed
void ApplyFramepacing(int renderMode)
{
    // ---------------------
    // now they do some frame pacing, as the game runs much faster
    // on modern platforms, so we'll do that here too.

    if (g_inner_frame_loop_cnt() == 0)
        g_frame_count() = GetTimeNs();

    if (renderMode && g_inner_frame_loop_cnt() > 0)
    {
        uint64_t frame_duration_ns = 16'666'666ULL; // ~60 FPS
        if (renderMode == 1)
            frame_duration_ns = 33'333'333ULL;       // ~30 FPS

        uint64_t full_target_ns = g_frame_count() + frame_duration_ns;

        // Original implementation
#if 1
        // ... :(
        mtx_t mutex;
        cnd_t cond;
        mtx_init(&mutex, mtx_timed);
        cnd_init(&cond);
        mtx_lock(&mutex);

        double ugly_ugly_approx_frame_time = (double)(int)g_frame_count() + (double)(int)frame_duration_ns * 1.0;

        auto perf_frequency = Query_perf_frequency();
        auto perf_counter = Query_perf_counter();
        double* p_d = &ugly_ugly_approx_frame_time;

        double fudge_time = *p_d - (double)(int)(1000000000 * (perf_counter % perf_frequency) / perf_frequency
            + 1000000000 * (perf_counter / perf_frequency));
        timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = 0;
        if (fudge_time > 0.0)
        {
            __int64 v9 = 100 * GetTickCount64() + (unsigned int)(int)fudge_time;
            ts.tv_sec = v9 / 1000000000;
            ts.tv_nsec = v9 % 1000000000;
        }

        // wait
        cnd_timedwait(&cond, &mutex, &ts);
        
        // cpu lock till full
        uint64_t time = GetTimeNs();
        while (time < full_target_ns && time > 0) {
            time = GetTimeNs();
            _mm_pause();
        }
#else
        while (true)
        {
            uint64_t now = GetTimeNs();
            int64_t remaining = full_target_ns - now;
            if (remaining <= 0)
                break;
            if (remaining > 2'000'000) // >2ms
                Sleep(1);
            else
                _mm_pause();
        }
#endif
        mtx_unlock(&mutex);
        cnd_destroy(&cond);
        mtx_destroy(&mutex);

        g_frame_count() = full_target_ns;
    }
    ++g_inner_frame_loop_cnt();
}


// @confirmed
void __fastcall d3tPresent(int renderMode)
{
    D3D11_TEXTURE2D_DESC v16;
    g_ppBackBufferTexture2D()->GetDesc(&v16);

    if (v16.SampleDesc.Count == 1)
        pDeviceContext()->CopyResource(g_pDxMipsAccessView(), g_ppBackBufferTexture2D());
    else
        pDeviceContext()->ResolveSubresource(g_pDxMipsAccessView(), 0LL, g_ppBackBufferTexture2D(), 0, v16.Format);

    pDeviceContext()->CopyResource(g_ppTexture2D(), g_pDxMipsAccessView());


#   if _DEBUG
        HandleDebugDraw();
#   endif

    unsigned int syncInterval = 0;
    if (renderMode && (renderMode - 1) <= 1)
        syncInterval = 1;
    ppSwapChain()->Present(syncInterval, 0LL);

    ApplyFramepacing(renderMode);
}

