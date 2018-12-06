#include "d3dUtility.h"

#include<time.h>
//声明全局的指针
ID3D11Device* device = NULL;
IDXGISwapChain* swapChain = NULL;
ID3D11DeviceContext* immediateContext = NULL;
ID3D11RenderTargetView* renderTargetView = NULL;

ID3D11VertexShader* m_VertexShader;
ID3D11PixelShader* m_PixelShader;

ID3D11InputLayout* vertexLayout;
ID3DX11Effect* effect;
ID3DX11EffectTechnique* technique;

XMMATRIX world;
XMMATRIX view;
XMMATRIX projection;


ID3D11ShaderResourceView * texture;
ID3D11ShaderResourceView * textureRoad;//路面纹理
ID3D11ShaderResourceView * textureLawn;//草地纹理
//树的纹理
ID3D11ShaderResourceView * textureTree1;
ID3D11ShaderResourceView * textureTree2;
ID3D11ShaderResourceView * textureTree3;
ID3D11ShaderResourceView * textureTree4;
ID3D11ShaderResourceView * textureTree5;
ID3D11ShaderResourceView * textureTree6;
//云的纹理
ID3D11ShaderResourceView * textureCloud1;
ID3D11ShaderResourceView * textureCloud2;
ID3D11ShaderResourceView * textureCloud3;
ID3D11ShaderResourceView * textureCloud4;
//巴士上左右后四个面的纹理
ID3D11ShaderResourceView * textureTop;
ID3D11ShaderResourceView * textureLeft;
ID3D11ShaderResourceView * textureRight;
ID3D11ShaderResourceView * textureBack;
//得分板数字纹理
ID3D11ShaderResourceView * textureScore1;
ID3D11ShaderResourceView * textureScore2;
ID3D11ShaderResourceView * textureScore3;
ID3D11ShaderResourceView * textureScore4;
ID3D11ShaderResourceView * textureScore5;
ID3D11ShaderResourceView * textureScore6;
ID3D11ShaderResourceView * textureScore7;
ID3D11ShaderResourceView * textureScore8;
ID3D11ShaderResourceView * textureScore9;
ID3D11ShaderResourceView * textureScore0;

ID3D11ShaderResourceView * textureCoin;
ID3D11ShaderResourceView * textureBomb;
ID3D11ShaderResourceView * textureStart;
ID3D11ShaderResourceView * textureEnd;

ID3D11BlendState * blendStateAlpha;//混合状态

//该接口表示用于配置管线光栅化阶段的状态组
ID3D11RasterizerState * NoCullRS;  //背面消隐

//声明道具位置暂存数组
static float tempPositionZ[24];
static float tempPositionX[24];


ID3D11DepthStencilView* depthStencilView;  //深度模板视图

//定义一个顶点结构，这个顶点包含坐标和颜色信息
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};


//**************以下为框架函数******************
bool Setup()
{
	//*************第一步初始化金币和炸弹的随机分布数组*************************
	srand((unsigned)time(NULL));
	float Position[4] = { 0, 2.5, 5, 7.5 };
	//XZ随机平移距离
	for (int i = 0; i < 24; i++)
	{
		tempPositionX[i] = Position[rand() % 4];
		tempPositionZ[i] = (float)10 + rand() % 391;
	}

	//*************第二步从.fx文件创建ID3DEffect对象****************************
	HRESULT hr = S_OK;
	ID3DBlob* pTechBlob = NULL;
	//从我们之前建立的.fx文件读取着色器相关信息
	hr = D3DCompileFromFile(L"Shader.fx", NULL, NULL, NULL, "fx_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS, NULL, &pTechBlob, NULL);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"fx文件载入失败", L"Error", MB_OK);
		return hr;
	}
	//创建ID3DEffect对象
	hr = D3DX11CreateEffectFromMemory(
		pTechBlob->GetBufferPointer(),
		pTechBlob->GetBufferSize(),
		0,
		device,
		&effect);

	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建Effect失败", L"Error", MB_OK);  //创建失败，弹出错误信息
		return hr;
	}
	//从素材创建纹理
	
	CreateWICTextureFromFile(device, L"img/road.bmp", NULL, &textureRoad);
	CreateWICTextureFromFile(device, L"img/lawn.png", NULL, &textureLawn);
	CreateWICTextureFromFile(device, L"img/tree1.png", NULL, &textureTree1);
	CreateWICTextureFromFile(device, L"img/tree2.png", NULL, &textureTree2);
	CreateWICTextureFromFile(device, L"img/tree3.png", NULL, &textureTree3);
	CreateWICTextureFromFile(device, L"img/tree4.png", NULL, &textureTree4);
	CreateWICTextureFromFile(device, L"img/tree5.png", NULL, &textureTree5);
	CreateWICTextureFromFile(device, L"img/tree6.png", NULL, &textureTree6);
	CreateWICTextureFromFile(device, L"img/cloud1.png", NULL, &textureCloud1);
	CreateWICTextureFromFile(device, L"img/cloud1.png", NULL, &textureCloud2);
	CreateWICTextureFromFile(device, L"img/cloud1.png", NULL, &textureCloud3);
	CreateWICTextureFromFile(device, L"img/cloud1.png", NULL, &textureCloud4);
	CreateWICTextureFromFile(device, L"img/bus-top.png", NULL, &textureTop);
	CreateWICTextureFromFile(device, L"img/bus-left.png", NULL, &textureLeft);
	CreateWICTextureFromFile(device, L"img/bus-right.png", NULL, &textureRight);
	CreateWICTextureFromFile(device, L"img/bus-back.png", NULL, &textureBack);
	CreateWICTextureFromFile(device, L"img/start.png", NULL, &textureStart);
	CreateWICTextureFromFile(device, L"img/end.png", NULL, &textureEnd);
	CreateWICTextureFromFile(device, L"img/1.png", NULL, &textureScore1);
	CreateWICTextureFromFile(device, L"img/2.png", NULL, &textureScore2);
	CreateWICTextureFromFile(device, L"img/3.png", NULL, &textureScore3);
	CreateWICTextureFromFile(device, L"img/4.png", NULL, &textureScore4);
	CreateWICTextureFromFile(device, L"img/5.png", NULL, &textureScore5);
	CreateWICTextureFromFile(device, L"img/6.png", NULL, &textureScore6);
	CreateWICTextureFromFile(device, L"img/7.png", NULL, &textureScore7);
	CreateWICTextureFromFile(device, L"img/8.png", NULL, &textureScore8);
	CreateWICTextureFromFile(device, L"img/9.png", NULL, &textureScore9);
	CreateWICTextureFromFile(device, L"img/0.png", NULL, &textureScore0);
	CreateWICTextureFromFile(device, L"img/coin.png", NULL, &textureCoin);
	CreateWICTextureFromFile(device, L"img/bomb.png", NULL, &textureBomb);

	//*************第三步设置各种渲染效果****************************
	//半透明效果
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	//关闭AlphaToConverage多重采样
	blendDesc.AlphaToCoverageEnable = false;
	//不针对多个render target采用不同的混合效果
	blendDesc.IndependentBlendEnable = false;
	//只针对redertarget[0]设置混合绘制状态
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;//设置源因子
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//设置目标因子
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;//混合操作
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;//源混合百分比
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;//目标混合百分比因子
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;//混合百分比
	//写掩码
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//创建Id3d11blendstate接口
	device->CreateBlendState(&blendDesc, &blendStateAlpha);
	//关闭背面消隐
	D3D11_RASTERIZER_DESC ncDesc;//光栅器描述
	ZeroMemory(&ncDesc, sizeof(ncDesc));
	ncDesc.CullMode = D3D11_CULL_NONE;//不剔除特定三角形
	ncDesc.FillMode = D3D11_FILL_SOLID;//填充模式
	ncDesc.FrontCounterClockwise = false;//是否逆时针为正面
	ncDesc.DepthClipEnable = true;//开启深度裁剪
	//创建一个关闭背面消隐的状态在需要用的时候才设置给执行上下文
	if (FAILED(device->CreateRasterizerState(&ncDesc, &NoCullRS)))
	{
		MessageBox(NULL, L"Create ' NoCull' rasterizer state failed!", L"Error",MB_OK);
		return false;
	}
	//纹理设置到texture
	effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texture);
	

	//*************第四步创建输入布局****************************
	//设置默认的technique到effect
	technique = effect->GetTechniqueByName("TexTech");
	D3DX11_PASS_DESC PassDesc;
	//利用GetPassByIndex获取Effect Pass
	//再利用GetDesc获取Effect Pass的描述，并存如PassDesc对象中
	technique->GetPassByIndex(0)->GetDesc(&PassDesc);

	//创建并设置输入布局
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	//创建输入布局
	hr = device->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &vertexLayout);
	//设置生成的输入布局到设备上下文中
	immediateContext->IASetInputLayout(vertexLayout);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建Input Layout失败", L"Error", MB_OK);
		return hr;
	}

	//*************第五步创建顶点缓存以及顶点索引缓存***************************
	//和实验4一样，创建顶点数组，由于每个顶点有包含了坐标和颜色
	Vertex vertices[] =
	{
		//车尾
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -0.7f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -0.7f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -0.7f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		//车右侧
		{ XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, 5.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 2.0f, 5.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 5.0f), XMFLOAT2(1.0f, 1.0f) },
		//车左侧
		{ XMFLOAT3(-1.0f, 2.0f, 5.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		//车顶
		{ XMFLOAT3(-1.0f, 2.0f, 4.2f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, 4.2f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 2.0f, 4.2f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		//车头
		{ XMFLOAT3(1.0f, 2.0f, 5.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, 5.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, 5.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 5.0f), XMFLOAT2(1.0f, 1.0f) },
		//车底
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 5.0f), XMFLOAT2(1.0f, 1.0f) },
		//道路顶点
		{ XMFLOAT3(-5.0f, -1.0f, 500.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 500.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 50.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 50.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 500.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f, -1.0f, -10.0f), XMFLOAT2(1.0f, 50.0f) },
		//草地顶点
		//右草地
		{ XMFLOAT3(5.0f, -1.0f, 500.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(505.0f, -1.0f, 500.0f), XMFLOAT2(50.0f, 0.0f) },
		{ XMFLOAT3(505.0f, -1.0f, -10.0f), XMFLOAT2(50.0f, 51.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 500.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(505.0f, -1.0f, -10.0f), XMFLOAT2(50.0f, 51.0f) },
		{ XMFLOAT3(5.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 51.0f) },
		//左草地
		{ XMFLOAT3(-505.0f, -1.0f, 500.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, 500.0f), XMFLOAT2(50.0f, 0.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, -10.0f), XMFLOAT2(50.0f, 51.0f) },
		{ XMFLOAT3(-505.0f, -1.0f, 500.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, -10.0f), XMFLOAT2(50.0f, 51.0f) },
		{ XMFLOAT3(-505.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 51.0f) },
		//内侧树
		//左侧
		{ XMFLOAT3(-10.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-10.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(-10.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-10.0f, 7.0f, -10.0f),  XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-10.0f, 7.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(-10.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//右侧
		{ XMFLOAT3(10.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(10.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(10.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f, 7.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(10.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//外侧树
		//左侧
		{ XMFLOAT3(-15.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-15.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(-15.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-15.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-15.0f, 7.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(-15.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//右侧
		{ XMFLOAT3(15.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(15.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(15.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(15.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(15.0f, 7.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(15.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//云
		{ XMFLOAT3(-50.0f, 50.0f, 100.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(50.0f, 0.0f, 100.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-50.0f, 0.0f, 100.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-50.0f, 50.0f, 100.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(50.0f, 50.0f, 100.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(50.0f, 0.0f, 100.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-10.0f, 20.0f, 100.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(20.0f, 0.0f, 100.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-10.0f, 0.0f, 100.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-10.0f, 20.0f, 100.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(20.0f, 20.0f, 100.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(20.0f, 0.0f, 100.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-100.0f, 40.0f, 100.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-50.0f, 0.0f, 100.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-100.0f, 0.0f, 100.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-100.0f, 40.0f, 100.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-50.0f, 40.0f, 100.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-50.0f, 0.0f, 100.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(50.0f, 40.0f, 100.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(100.0f, 0.0f, 100.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(50.0f, 0.0f, 100.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(50.0f, 40.0f, 100.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(100.0f, 40.0f, 100.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(100.0f, 0.0f, 100.0f), XMFLOAT2(1.0f, 1.0f) },

		//横向树
		{ XMFLOAT3(-13.644f, 7.0f, -6.356f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-6.356f, -1.0f, -6.356f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-13.644f, -1.0f, -6.356f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-13.644f, 7.0f, -6.356f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-6.356f, 7.0f, -6.356f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-6.356f, -1.0f, -6.356f), XMFLOAT2(1.0f, 1.0f) },

		//左最外侧树
		{ XMFLOAT3(-20.0f, 10.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-20.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(-20.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-20.0f, 10.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-20.0f, 10.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(-20.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//右最外侧树
		{ XMFLOAT3(20.0f, 10.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(20.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(20.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(20.0f, 10.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(20.0f, 10.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(20.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//start图标
		{ XMFLOAT3(-2.0f, 4.5f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f, 4.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-2.0f, 4.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-2.0f, 4.5f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f, 4.5f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f, 4.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		//终点
		{ XMFLOAT3(-5.0f, -1.0f, 406.25f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 406.25f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, 405.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, 405.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 406.25f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 405.0f), XMFLOAT2(1.0f, 1.0f) },
		//得分板
		{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.3f, 5.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.55f, 5.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.3f, 5.5f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-0.3f, 5.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		//金币和炸弹
		{ XMFLOAT3(-4.75f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-2.75f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-4.75f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-4.75f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-2.75f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-2.75f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
	};
	UINT vertexCount = ARRAYSIZE(vertices);
	//创建顶点缓存，方法同实验4一样
	//首先声明一个D3D11_BUFFER_DESC的对象bd
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  //注意：这里表示创建的是顶点缓存
	bd.CPUAccessFlags = 0;

	//声明一个D3D11_SUBRESOURCE_DATA数据用于初始化子资源
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;         //设置需要初始化的数据，这里的数据就是顶点数组

	//声明一个ID3D11Buffer对象作为顶点缓存
	ID3D11Buffer* vertexBuffer;
	//调用CreateBuffer创建顶点缓存
	hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建VertexBuffer失败", L"Error", MB_OK);
		return hr;
	}

	UINT stride = sizeof(Vertex);                 //获取Vertex的大小作为跨度
	UINT offset = 0;                              //设置偏移量为0
												  //设置顶点缓存
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
void Cleanup()
{
	//释放全局指针
	if (renderTargetView) renderTargetView->Release();
	if (immediateContext) immediateContext->Release();
	if (swapChain) swapChain->Release();
	if (device) device->Release();

	if (vertexLayout) vertexLayout->Release();
	if (effect) effect->Release();

	if (depthStencilView) depthStencilView->Release();

	if (texture) texture->Release();
	if (textureLawn) textureLawn->Release();
	if (textureRoad) textureRoad->Release();
	if (textureTree1) textureTree1->Release();
	if (textureTree2) textureTree2->Release();
	if (textureTree3) textureTree3->Release();
	if (textureTree4) textureTree4->Release();
	if (textureTree5) textureTree5->Release();
	if (textureTree6) textureTree6->Release();
	if (textureCloud1) textureCloud1->Release();
	if (textureCloud2) textureCloud2->Release();
	if (textureCloud3) textureCloud3->Release();
	if (textureCloud4) textureCloud4->Release();
	if (textureTop) textureTop->Release();
	if (textureLeft) textureLeft->Release();
	if (textureRight) textureRight->Release();
	if (textureBack) textureBack ->Release();
	if (textureScore1) textureScore1->Release();
	if (textureScore2) textureScore2->Release();
	if (textureScore3) textureScore3->Release();
	if (textureScore4) textureScore4->Release();
	if (textureScore5) textureScore5->Release();
	if (textureScore6) textureScore6->Release();
	if (textureScore7) textureScore7->Release();
	if (textureScore8) textureScore8->Release();
	if (textureScore9) textureScore9->Release();
	if (textureScore0) textureScore0->Release();

	if (textureCoin) textureCoin->Release();
	if (textureBomb) textureBomb->Release();
	if (textureStart) textureStart->Release();
	if (textureEnd) textureEnd->Release();

	if (blendStateAlpha) blendStateAlpha->Release();
	if (NoCullRS) NoCullRS->Release();
}

bool Display(float timeDelta)
{
	if (device)
	{
		float ClearColor[4] = { 0.0f, 0.5f, 1.0f, 1.0f };
		immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
		//清除深度模板视图
		immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		//得分纹理数组
		ID3D11ShaderResourceView * textureS[] = { textureScore0,textureScore1,textureScore2,
			textureScore3,textureScore4,textureScore5,textureScore6,textureScore7,textureScore8,textureScore9 };
		float BlendFactor[] = { 0,0,0,0 };

		static float tranX = 0;						//初始化X方向移动距离
		static float zpos = 0;						//初始化z方向移动距离
		static bool start = false;					//游戏开始标志
		static int scores = 0;						//初始化游戏得分
		static int scoreIterms = 0;					//初始化道具得分
		static int scoreboard[4] = { 0,0,0,0 };		//初始化得分版数组

		//道具碰撞算法
		for (int i = 0; i < 24; i++)
		{	//获取车与道具的距离
			float distance = (tranX - tempPositionX[i] + 3.75f) *  (tranX - tempPositionX[i] + 3.75f) +
				(tempPositionZ[i] - zpos - 5.0f) * (tempPositionZ[i] - zpos - 5.0f);
			if (distance < 4)
			{
				tempPositionX[i] = 0;
				tempPositionZ[i] = 0;
				if (i % 4 == 0) scoreIterms -= 200;
				else scoreIterms += 100;
			}
		}
		//游戏开始后，车以15单位/timedelta的速度前行
		if (start)
		{
			zpos -= timeDelta * 15;
			scores = scoreIterms + (int)(2 * zpos);
			//终点检测
			if (zpos > 400)
			{
				zpos = 400;
			}
			//左右方向
			if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
				tranX += 3.0f * timeDelta;
			if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
				tranX -= 3.0f * timeDelta;
			//边界检测
			if (tranX >= 3.5) tranX = 3.5;
			if (tranX <= -3.5)tranX = -3.5;
		}
		if (::GetAsyncKeyState(13) & 0x8000f)
		{
			start = true;
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000f)
		{
			start = false;
		}
		//设置变换坐标系
		//初始化世界矩阵 生成投影矩阵 获取观察坐标系
		world = XMMatrixIdentity();
		//调整视点 和 视线方向
		XMVECTOR Eye = XMVectorSet(0.0f, 4.5f, zpos - 4, 0.0f);
		XMVECTOR At = XMVectorSet(0.0f, 0.0f, 5.0f + zpos, 0.0f);

		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		view = XMMatrixLookAtLH(Eye, At, Up);

		projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);

		//将几个坐标系变换矩阵设置到Effect框架中
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);				//设置世界坐标系
		effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);				//设置观察坐标系
		effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection);    //设置投影坐标系

		//描述technique
		D3DX11_TECHNIQUE_DESC techDesc;
		technique->GetDesc(&techDesc);

		//画车道
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRoad);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 36);
		//画道路终点
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureEnd);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 126);
		//画道路两旁的绿地
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureLawn);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(12, 42);

		immediateContext->OMSetBlendState(blendStateAlpha, BlendFactor, 0xffffffff);
		immediateContext->RSSetState(NoCullRS);

		//画云
		world = world * XMMatrixTranslation(0.0f, 0.0f, 0.92f * zpos);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCloud1);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 78);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCloud2);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 84);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCloud3);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 90);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCloud4);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 96);

		//画外侧树
		world = XMMatrixIdentity();
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTree2);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(12, 108);

		//画内侧树
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTree3);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(12, 66);
		world = XMMatrixIdentity();
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);

		//第一棵内侧树向x正方向平移500单位
		world = world * XMMatrixTranslation(0.0f, 0.0f, 500.0f);
		for (int i = 0; i < 70; i++)
		{
			world = world * XMMatrixTranslation(0.0f, 0.0f, -7.288f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTree6);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 102);
		}

		//循环平移画右内侧树
		world = XMMatrixIdentity();
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
		//第一棵内侧树向x正方向平移500单位
		world = world * XMMatrixTranslation(20.0f, 0.0f, 500.0f);
		for (int i = 0; i < 70; i++)
		{
			world = world * XMMatrixTranslation(0.0f, 0.0f, -7.288f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTree6);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 102);
		}

		
		//画道具
		for (int i = 0; i < 20; i = i + 4)
		{
			world = XMMatrixIdentity();
			world = world * XMMatrixTranslation(tempPositionX[i], 0.0f, tempPositionZ[i]);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureBomb);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 138);

			world = XMMatrixIdentity();
			world = world * XMMatrixTranslation(tempPositionX[i+1], 0.0f, tempPositionZ[i+1]);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCoin);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 138);

			world = XMMatrixIdentity();
			world = world * XMMatrixTranslation(tempPositionX[i+3], 0.0f, tempPositionZ[i+2]);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCoin);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 138);

			world = XMMatrixIdentity();
			world = world * XMMatrixTranslation(tempPositionX[i+3], 0.0f, tempPositionZ[i+3]);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCoin);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 138);

		}
		immediateContext->OMSetBlendState(0, 0, 0xffffffff);
		immediateContext->RSSetState(0);

		//画车
		//起始位置设为(tranX, 0.0f, zpos)
		world = XMMatrixIdentity();
		world = world * XMMatrixTranslation(tranX, 0.0f, zpos);

		immediateContext->OMSetBlendState(blendStateAlpha, BlendFactor, 0xffffffff);
		immediateContext->RSSetState(NoCullRS);

		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRight);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 6);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureLeft);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 12);
		if (tranX <= -2.5)
		{
			//画车左侧
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureLeft);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 12);
			//画车右侧
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRight);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 6);
		
		}
		//画车头
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTop);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 18);
		//画车尾
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureBack);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 0);

		//画开始游戏
		world = XMMatrixIdentity();
		//起始位置作为车的当前位置
		world = world * XMMatrixTranslation(0.0f, 0.0f, zpos);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
		if (!start)
		{
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureStart);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 120);
		}
		else
		{
			if (scores < 0) scores = 0;
			for (int i = 0; i < 4; i++)
			{
				scoreboard[i] = scores % 10;
				scores /= 10;
			}
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureS[scoreboard[3]]);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 132);

			world = world * XMMatrixTranslation(0.3f, 0.0f, 0.0f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureS[scoreboard[2]]);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 132);

			world = world * XMMatrixTranslation(0.3f, 0.0f, 0.0f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureS[scoreboard[1]]);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 132);

			world = world * XMMatrixTranslation(0.3f, 0.0f, 0.0f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureS[scoreboard[0]]);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 132);
		}
		//呈现
		immediateContext->OMSetBlendState(0, 0, 0xffffffff);
		immediateContext->RSSetState(0);

		swapChain->Present(0, 0);

	}
	return true;
}

//**************框架函数******************

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{

	//初始化
	if (!d3d::InitD3D(hinstance,
		800,
		600,
		&renderTargetView,
		&immediateContext,
		&swapChain,
		&device))// [out]The created device.
	{
		MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	return 0;
}