#ifndef D3DUTILITY_H_
#define D3DUTILITY_H_

#include <Windows.h>
#include <windowsx.h>
#include "wincodec.h"
#include <iostream>
#include <sstream>
#include <string>

#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx11effect.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "../Effects11/Debug/Effects11d.lib")
#pragma comment(lib, "../Effects11/Release/Effects11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "xaudio2.lib")
#pragma comment(lib, "mfcore.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "xinput.lib")

using namespace DirectX;


namespace d3d
{
	//初始化D3D
	bool InitD3D(
		HINSTANCE hInstance,
		int width, int height,
		ID3D11RenderTargetView ** renderTargetView,
		ID3D11DeviceContext ** immediateContext,
		IDXGISwapChain ** swapChain,
		ID3D11Device ** device);

	//消息循环
	int EnterMsgLoop(bool(*ptr_display)(float timeDelta));

	//回调函数
	LRESULT CALLBACK WndProc(
		HWND,
		UINT msg,
		WPARAM,
		LPARAM lParam);
}

#endif 