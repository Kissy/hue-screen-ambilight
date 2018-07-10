#include "stdafx.h"
#include "DXGIManager.h"
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <algorithm>
#include <cmath>

#include "RgbColor.h"
#include "PixelData.h"
#include "CLocalMaximum.h"
#include "ColorCube.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

DXGIManager g_DXGIManager;

LocalMaximum averageLocalMaxima(std::vector<LocalMaximum> lastLocalMaximum) {
	LocalMaximum localMaximum;
	for (auto maximum : lastLocalMaximum) {
		localMaximum.red += maximum.red;
		localMaximum.green += maximum.green;
		localMaximum.blue += maximum.blue;
	}
	localMaximum.red /= lastLocalMaximum.size();
	localMaximum.green /= lastLocalMaximum.size();
	localMaximum.blue /= lastLocalMaximum.size();
	return localMaximum;
}

void updateLoop() {
	CoInitialize(NULL);
	g_DXGIManager.SetCaptureSource(CSDesktop);
	RECT rcDim;
	g_DXGIManager.GetOutputRect(rcDim);
	rcDim.top = rcDim.top / 4;
	rcDim.right = rcDim.right / 4;
	rcDim.bottom = rcDim.bottom / 4;
	rcDim.left = rcDim.left / 4;
	int dwWidth = rcDim.right - rcDim.left;
	int dwHeight = rcDim.bottom - rcDim.top;
	printf("dwWidth=%d dwHeight=%d\n", dwWidth, dwHeight);
	int stride = dwWidth * 4;
	int dwBufSize = dwHeight * stride;
	int pixelCount = dwWidth * dwHeight;
	BYTE* pBuf = new BYTE[dwBufSize];
	CComPtr<IWICImagingFactory> spWICFactory = NULL;
	HRESULT hr = spWICFactory.CoCreateInstance(CLSID_WICImagingFactory);

	if (FAILED(hr)) {
		return;
	}

	web::http::client::http_client client(U("http://192.168.1.16/"));
	ColorCube colorCube;
	std::vector<std::vector<LocalMaximum>> lastLocalMaxima(3, std::vector<LocalMaximum>(5));
	int lastIndex = 0;

	int counter = 0;
	while (true) {
		Sleep(500);

		int i = 0;

		do {
			hr = g_DXGIManager.GetOutputBits(pBuf, rcDim);
			i++;
		} while (hr == DXGI_ERROR_WAIT_TIMEOUT || i < 2);

		if (FAILED(hr)) {
			printf("GetOutputBits failed with hr=0x%08x\n", hr);
			return;
		}

		colorCube.quantizePixels(pBuf, pixelCount);
		colorCube.updateLocalMaxima();

		lastLocalMaxima[0][lastIndex] = colorCube.getMaximum(0);
		lastLocalMaxima[1][lastIndex] = colorCube.getMaximum(1);
		lastLocalMaxima[2][lastIndex] = colorCube.getMaximum(2);
		lastIndex = (lastIndex + 1) % 5;

		LocalMaximum firstLocalMaxima = averageLocalMaxima(lastLocalMaxima[0]);
		LocalMaximum secondLocalMaxima = averageLocalMaxima(lastLocalMaxima[1]);
		LocalMaximum thirdLocalMaxima = averageLocalMaxima(lastLocalMaxima[2]);

		web::json::value object = firstLocalMaxima.getHueXYState();
		web::http::http_response response = client.request(web::http::methods::PUT, U("/api/administrator/lights/1/state"), object).get();
		web::json::value object2 = secondLocalMaxima.getHueXYState();
		web::http::http_response response2 = client.request(web::http::methods::PUT, U("/api/administrator/lights/2/state"), object2).get();
		web::json::value object3 = thirdLocalMaxima.getHueXYState();
		web::http::http_response response3 = client.request(web::http::methods::PUT, U("/api/administrator/lights/3/state"), object3).get();

		counter++;
		if (counter == 100) {
			break;
		}
	}
	
	delete[] pBuf;
}

int _tmain(int argc, _TCHAR* argv[]) {
	updateLoop();
	_CrtDumpMemoryLeaks();
    return 0;
}
