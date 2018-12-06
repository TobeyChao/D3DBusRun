#include "d3dUtility.h"

std::wstring	m_mainWndCaption = L"D3D11";		// ���ڱ���
HWND			hWnd = 0;

/*D3D��ʼ���������������̣��������ںͳ�ʼ��D3D*/
bool d3d::InitD3D(
	HINSTANCE hInstance,
	int width, int height,
	ID3D11RenderTargetView ** renderTargetView,
	ID3D11DeviceContext ** immediateContext,
	IDXGISwapChain ** swapChain,
	ID3D11Device ** device)
{
	/*һ����һ������*/

	//1.��ƴ���
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
	//2.ע�ᴰ��
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed", 0, 0);
		return false;
	}
	//3.��������
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
	/*��ʼ��D3D*/
		//1.����������
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
	sd.SampleDesc.Count = 1;//���ز���
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

		//2.����3D�豸�ͽ������Լ������Ļ���
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

		//3.������������Ⱦ��ͼ
	HRESULT hr;
	ID3D11Texture2D *pBackBuffer = NULL;
		//������̨����
	hr = (*swapChain)->GetBuffer(0,
		_uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"GetBuffer Failed", 0, 0);
		return false;
	}
		//������Ⱦ��ͼ
	hr = (*device)->CreateRenderTargetView(pBackBuffer,
		NULL,
		renderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		MessageBox(0, L"CreateRender Failed", 0, 0);
		return false;
	}
		//��Ⱦ��ͼ������Ⱦ����
	(*immediateContext)->OMSetRenderTargets(1,
		renderTargetView,
		NULL);

	//�����ӿ�
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

	//�����ĸ���̬����  
	static float  fps = 0; //������Ҫ�����FPSֵ  
	static int    frameCount = 0;//֡��  
	static float  currentTime = 0.0f;//��ǰʱ��  
	static float  lastTime = 0.0f;//����ʱ��  

	frameCount++;//ÿ����һ��Get_FPS()������֡������1  
	currentTime = timeGetTime()*0.001f;//��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ�䣬������Ҫ����0.001���õ���λΪ���ʱ��  

									   //�����ǰʱ���ȥ����ʱ�������1���ӣ��ͽ���һ��FPS�ļ���ͳ���ʱ��ĸ��£�����֡��ֵ����  
	if (currentTime - lastTime > 1.0f) //��ʱ�������1����  
	{
		fps = (float)frameCount / (currentTime - lastTime);//������1���ӵ�FPSֵ  
		lastTime = currentTime; //����ǰʱ��currentTime��������ʱ��lastTime����Ϊ��һ��Ļ�׼ʱ��  
		frameCount = 0;//������֡��frameCountֵ����  
	}
	std::wostringstream outs;
	outs.precision(6);
	outs << m_mainWndCaption << L"    "
		<< L"FPS: " << fps << L"    ";
	SetWindowText(hWnd, outs.str().c_str());
}

/*��Ϣ����*/
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