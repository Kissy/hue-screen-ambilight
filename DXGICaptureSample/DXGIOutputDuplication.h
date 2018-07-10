#pragma once

#include <d3d11.h>
#include <atlbase.h>
#include "DXGIPointerInfo.h"

class DXGIOutputDuplication {
public:
	DXGIOutputDuplication(IDXGIAdapter1* pAdapter,
		ID3D11Device* pD3DDevice,
		ID3D11DeviceContext* pD3DDeviceContext,
		IDXGIOutput1* pDXGIOutput1,
		IDXGIOutputDuplication* pDXGIOutputDuplication);

	HRESULT GetDesc(DXGI_OUTPUT_DESC& desc);
	HRESULT AcquireNextFrame(IDXGISurface1** pD3D11Texture2D, DXGIPointerInfo*& pDXGIPointer);
	HRESULT ReleaseFrame();

	bool IsPrimary();

private:
	CComPtr<IDXGIAdapter1> m_Adapter;
	CComPtr<ID3D11Device> m_D3DDevice;
	CComPtr<ID3D11DeviceContext> m_D3DDeviceContext;
	CComPtr<IDXGIOutput1> m_DXGIOutput1;
	CComPtr<IDXGIOutputDuplication> m_DXGIOutputDuplication;
};