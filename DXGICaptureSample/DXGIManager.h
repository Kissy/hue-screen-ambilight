#pragma once

#include <windows.h>
#include <wincodec.h>
#include <atlbase.h>
#include <dxgitype.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <vector>
#include "DXGIOutputDuplication.h"

using namespace std;

class DXGIPointerInfo;

enum CaptureSource {
    CSUndefined,
    CSMonitor1,
    CSMonitor2,
    CSDesktop
};


class DXGIManager {
public:
    DXGIManager();
    ~DXGIManager();
    HRESULT SetCaptureSource (CaptureSource type);
    CaptureSource GetCaptureSource();

    HRESULT GetOutputRect (RECT& rc);
    HRESULT GetOutputBits (BYTE* pBits, RECT& rcDest);
private:
    HRESULT Init();
    int GetMonitorCount();
    vector<DXGIOutputDuplication> GetOutputDuplication();
    void DrawMousePointer (BYTE* pDesktopBits, RECT rcDesktop, RECT rcDest);
private:
    CComPtr<IDXGIFactory1> m_spDXGIFactory1;
    vector<DXGIOutputDuplication> m_vOutputs;
    bool m_bInitialized;
    CaptureSource m_CaptureSource;
    RECT m_rcCurrentOutput;
    BYTE* m_pBuf;

    CComPtr<IWICImagingFactory> m_spWICFactory;
    ULONG_PTR m_gdiplusToken;
    DXGIPointerInfo* m_pDXGIPointer;
};