#include "d3dUtility.h"

std::wstring	m_mainWndCaption = L"D3D11";		// 窗口标题
HWND			hWnd = 0;

/*D3D初始化，包括两个过程：创建窗口和初始化D3D*/
bool d3d::InitD3D(
	HINSTANCE hInstance,
	int width, int height,
	ID3D11RenderTargetView ** renderTargetView,
	ID3D11DeviceContext ** immediateContext,
	IDXGISwapChain ** swapChain,
	ID3D11Device ** device)
{
	/*一创建一个窗口*/

	//1.设计窗口
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)d3d::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = L"Direct3D11 App";
	wc.lpszMenuName = 0;
	//2.注册窗口
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed", 0, 0);
		return false;
	}
	//3.创建窗口
	HWND hwnd = 0;
	hwnd = CreateWindow(L"Direct3D11 App",
		m_mainWndCaption.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		0,
		0,
		hInstance,
		0);
	if (!hwnd)
	{
		MessageBox(0, L"CreateWindow Failed", 0, 0);
		return false;
	}
	hWnd = hwnd;
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	/*初始化D3D*/
		//1.描述交换链
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;//多重采样
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

		//2.创建3D设备和交换链以及上下文环境
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevers = ARRAYSIZE(featureLevels);

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featureLevels,
		numFeatureLevers,
		D3D11_SDK_VERSION,
		&sd,
		swapChain,
		device,
		NULL,
		immediateContext
	)))
	{
		MessageBox(0, L"CreateDevice Failed",0,0);
		return false;
	}

		//3.创建并设置渲染视图
	HRESULT hr;
	ID3D11Texture2D *pBackBuffer = NULL;
		//创建后台缓存
	hr = (*swapChain)->GetBuffer(0,
		_uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"GetBuffer Failed", 0, 0);
		return false;
	}
		//创建渲染视图
	hr = (*device)->CreateRenderTargetView(pBackBuffer,
		NULL,
		renderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		MessageBox(0, L"CreateRender Failed", 0, 0);
		return false;
	}
		//渲染试图绑定至渲染管线
	(*immediateContext)->OMSetRenderTargets(1,
		renderTargetView,
		NULL);

	//设置视口
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MaxDepth = 1;
	vp.MinDepth = 0;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	(*immediateContext)->RSSetViewports(1,
		&vp);
	return true;
}

void Get_FPS()
{

	//定义四个静态变量  
	static float  fps = 0; //我们需要计算的FPS值  
	static int    frameCount = 0;//帧数  
	static float  currentTime = 0.0f;//当前时间  
	static float  lastTime = 0.0f;//持续时间  

	frameCount++;//每调用一次Get_FPS()函数，帧数自增1  
	currentTime = timeGetTime()*0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间，所以需要乘以0.001，得到单位为秒的时间  

									   //如果当前时间减去持续时间大于了1秒钟，就进行一次FPS的计算和持续时间的更新，并将帧数值清零  
	if (currentTime - lastTime > 1.0f) //将时间控制在1秒钟  
	{
		fps = (float)frameCount / (currentTime - lastTime);//计算这1秒钟的FPS值  
		lastTime = currentTime; //将当前时间currentTime赋给持续时间lastTime，作为下一秒的基准时间  
		frameCount = 0;//将本次帧数frameCount值清零  
	}
	std::wostringstream outs;
	outs.precision(6);
	outs << m_mainWndCaption << L"    "
		<< L"FPS: " << fps << L"    ";
	SetWindowText(hWnd, outs.str().c_str());
}

/*消息处理*/
int d3d::EnterMsgLoop(bool(*ptrDisplay)(float timeDelta))
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	static float lastTime = (float)timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float currTime = (float)timeGetTime();
			float timeDelta = (lastTime - currTime)*0.001f;
			ptrDisplay(timeDelta);
			Get_FPS();
			lastTime = currTime;
		}
	}
	return msg.wParam;
}