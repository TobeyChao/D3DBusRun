#include "d3dUtility.h"

#include<time.h>
//����ȫ�ֵ�ָ��
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
ID3D11ShaderResourceView * textureRoad;//·������
ID3D11ShaderResourceView * textureLawn;//�ݵ�����
//��������
ID3D11ShaderResourceView * textureTree1;
ID3D11ShaderResourceView * textureTree2;
ID3D11ShaderResourceView * textureTree3;
ID3D11ShaderResourceView * textureTree4;
ID3D11ShaderResourceView * textureTree5;
ID3D11ShaderResourceView * textureTree6;
//�Ƶ�����
ID3D11ShaderResourceView * textureCloud1;
ID3D11ShaderResourceView * textureCloud2;
ID3D11ShaderResourceView * textureCloud3;
ID3D11ShaderResourceView * textureCloud4;
//��ʿ�����Һ��ĸ��������
ID3D11ShaderResourceView * textureTop;
ID3D11ShaderResourceView * textureLeft;
ID3D11ShaderResourceView * textureRight;
ID3D11ShaderResourceView * textureBack;
//�÷ְ���������
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

ID3D11BlendState * blendStateAlpha;//���״̬

//�ýӿڱ�ʾ�������ù��߹�դ���׶ε�״̬��
ID3D11RasterizerState * NoCullRS;  //��������

//��������λ���ݴ�����
static float tempPositionZ[24];
static float tempPositionX[24];


ID3D11DepthStencilView* depthStencilView;  //���ģ����ͼ

//����һ������ṹ�������������������ɫ��Ϣ
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};


//**************����Ϊ��ܺ���******************
bool Setup()
{
	//*************��һ����ʼ����Һ�ը��������ֲ�����*************************
	srand((unsigned)time(NULL));
	float Position[4] = { 0, 2.5, 5, 7.5 };
	//XZ���ƽ�ƾ���
	for (int i = 0; i < 24; i++)
	{
		tempPositionX[i] = Position[rand() % 4];
		tempPositionZ[i] = (float)10 + rand() % 391;
	}

	//*************�ڶ�����.fx�ļ�����ID3DEffect����****************************
	HRESULT hr = S_OK;
	ID3DBlob* pTechBlob = NULL;
	//������֮ǰ������.fx�ļ���ȡ��ɫ�������Ϣ
	hr = D3DCompileFromFile(L"Shader.fx", NULL, NULL, NULL, "fx_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS, NULL, &pTechBlob, NULL);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"fx�ļ�����ʧ��", L"Error", MB_OK);
		return hr;
	}
	//����ID3DEffect����
	hr = D3DX11CreateEffectFromMemory(
		pTechBlob->GetBufferPointer(),
		pTechBlob->GetBufferSize(),
		0,
		device,
		&effect);

	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����Effectʧ��", L"Error", MB_OK);  //����ʧ�ܣ�����������Ϣ
		return hr;
	}
	//���زĴ�������
	
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

	//*************���������ø�����ȾЧ��****************************
	//��͸��Ч��
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	//�ر�AlphaToConverage���ز���
	blendDesc.AlphaToCoverageEnable = false;
	//����Զ��render target���ò�ͬ�Ļ��Ч��
	blendDesc.IndependentBlendEnable = false;
	//ֻ���redertarget[0]���û�ϻ���״̬
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;//����Դ����
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//����Ŀ������
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;//��ϲ���
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;//Դ��ϰٷֱ�
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;//Ŀ���ϰٷֱ�����
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;//��ϰٷֱ�
	//д����
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//����Id3d11blendstate�ӿ�
	device->CreateBlendState(&blendDesc, &blendStateAlpha);
	//�رձ�������
	D3D11_RASTERIZER_DESC ncDesc;//��դ������
	ZeroMemory(&ncDesc, sizeof(ncDesc));
	ncDesc.CullMode = D3D11_CULL_NONE;//���޳��ض�������
	ncDesc.FillMode = D3D11_FILL_SOLID;//���ģʽ
	ncDesc.FrontCounterClockwise = false;//�Ƿ���ʱ��Ϊ����
	ncDesc.DepthClipEnable = true;//������Ȳü�
	//����һ���رձ���������״̬����Ҫ�õ�ʱ������ø�ִ��������
	if (FAILED(device->CreateRasterizerState(&ncDesc, &NoCullRS)))
	{
		MessageBox(NULL, L"Create ' NoCull' rasterizer state failed!", L"Error",MB_OK);
		return false;
	}
	//�������õ�texture
	effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texture);
	

	//*************���Ĳ��������벼��****************************
	//����Ĭ�ϵ�technique��effect
	technique = effect->GetTechniqueByName("TexTech");
	D3DX11_PASS_DESC PassDesc;
	//����GetPassByIndex��ȡEffect Pass
	//������GetDesc��ȡEffect Pass��������������PassDesc������
	technique->GetPassByIndex(0)->GetDesc(&PassDesc);

	//�������������벼��
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	//�������벼��
	hr = device->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &vertexLayout);
	//�������ɵ����벼�ֵ��豸��������
	immediateContext->IASetInputLayout(vertexLayout);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����Input Layoutʧ��", L"Error", MB_OK);
		return hr;
	}

	//*************���岽�������㻺���Լ�������������***************************
	//��ʵ��4һ���������������飬����ÿ�������а������������ɫ
	Vertex vertices[] =
	{
		//��β
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -0.7f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -0.7f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -0.7f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		//���Ҳ�
		{ XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, 5.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 2.0f, 5.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 5.0f), XMFLOAT2(1.0f, 1.0f) },
		//�����
		{ XMFLOAT3(-1.0f, 2.0f, 5.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		//����
		{ XMFLOAT3(-1.0f, 2.0f, 4.2f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, 4.2f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 2.0f, 4.2f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		//��ͷ
		{ XMFLOAT3(1.0f, 2.0f, 5.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, 5.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, 5.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 5.0f), XMFLOAT2(1.0f, 1.0f) },
		//����
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 5.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 5.0f), XMFLOAT2(1.0f, 1.0f) },
		//��·����
		{ XMFLOAT3(-5.0f, -1.0f, 500.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 500.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 50.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 50.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 500.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f, -1.0f, -10.0f), XMFLOAT2(1.0f, 50.0f) },
		//�ݵض���
		//�Ҳݵ�
		{ XMFLOAT3(5.0f, -1.0f, 500.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(505.0f, -1.0f, 500.0f), XMFLOAT2(50.0f, 0.0f) },
		{ XMFLOAT3(505.0f, -1.0f, -10.0f), XMFLOAT2(50.0f, 51.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 500.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(505.0f, -1.0f, -10.0f), XMFLOAT2(50.0f, 51.0f) },
		{ XMFLOAT3(5.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 51.0f) },
		//��ݵ�
		{ XMFLOAT3(-505.0f, -1.0f, 500.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, 500.0f), XMFLOAT2(50.0f, 0.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, -10.0f), XMFLOAT2(50.0f, 51.0f) },
		{ XMFLOAT3(-505.0f, -1.0f, 500.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, -10.0f), XMFLOAT2(50.0f, 51.0f) },
		{ XMFLOAT3(-505.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 51.0f) },
		//�ڲ���
		//���
		{ XMFLOAT3(-10.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-10.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(-10.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-10.0f, 7.0f, -10.0f),  XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-10.0f, 7.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(-10.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//�Ҳ�
		{ XMFLOAT3(10.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(10.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(10.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f, 7.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(10.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//�����
		//���
		{ XMFLOAT3(-15.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-15.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(-15.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-15.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-15.0f, 7.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(-15.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//�Ҳ�
		{ XMFLOAT3(15.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(15.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(15.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(15.0f, 7.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(15.0f, 7.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(15.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//��
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

		//������
		{ XMFLOAT3(-13.644f, 7.0f, -6.356f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-6.356f, -1.0f, -6.356f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-13.644f, -1.0f, -6.356f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-13.644f, 7.0f, -6.356f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-6.356f, 7.0f, -6.356f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-6.356f, -1.0f, -6.356f), XMFLOAT2(1.0f, 1.0f) },

		//���������
		{ XMFLOAT3(-20.0f, 10.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-20.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(-20.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-20.0f, 10.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-20.0f, 10.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(-20.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//���������
		{ XMFLOAT3(20.0f, 10.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(20.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		{ XMFLOAT3(20.0f, -1.0f, -10.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(20.0f, 10.0f, -10.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(20.0f, 10.0f, 500.0f), XMFLOAT2(70.0f, 0.0f) },
		{ XMFLOAT3(20.0f, -1.0f, 500.0f), XMFLOAT2(70.0f, 1.0f) },
		//startͼ��
		{ XMFLOAT3(-2.0f, 4.5f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f, 4.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-2.0f, 4.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-2.0f, 4.5f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f, 4.5f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f, 4.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		//�յ�
		{ XMFLOAT3(-5.0f, -1.0f, 406.25f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 406.25f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, 405.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-5.0f, -1.0f, 405.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 406.25f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f, -1.0f, 405.0f), XMFLOAT2(1.0f, 1.0f) },
		//�÷ְ�
		{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.3f, 5.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.55f, 5.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-0.55f, 5.5f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.3f, 5.5f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-0.3f, 5.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		//��Һ�ը��
		{ XMFLOAT3(-4.75f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-2.75f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-4.75f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-4.75f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-2.75f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-2.75f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
	};
	UINT vertexCount = ARRAYSIZE(vertices);
	//�������㻺�棬����ͬʵ��4һ��
	//��������һ��D3D11_BUFFER_DESC�Ķ���bd
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  //ע�⣺�����ʾ�������Ƕ��㻺��
	bd.CPUAccessFlags = 0;

	//����һ��D3D11_SUBRESOURCE_DATA�������ڳ�ʼ������Դ
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;         //������Ҫ��ʼ�������ݣ���������ݾ��Ƕ�������

	//����һ��ID3D11Buffer������Ϊ���㻺��
	ID3D11Buffer* vertexBuffer;
	//����CreateBuffer�������㻺��
	hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����VertexBufferʧ��", L"Error", MB_OK);
		return hr;
	}

	UINT stride = sizeof(Vertex);                 //��ȡVertex�Ĵ�С��Ϊ���
	UINT offset = 0;                              //����ƫ����Ϊ0
												  //���ö��㻺��
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
void Cleanup()
{
	//�ͷ�ȫ��ָ��
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
		//������ģ����ͼ
		immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		//�÷���������
		ID3D11ShaderResourceView * textureS[] = { textureScore0,textureScore1,textureScore2,
			textureScore3,textureScore4,textureScore5,textureScore6,textureScore7,textureScore8,textureScore9 };
		float BlendFactor[] = { 0,0,0,0 };

		static float tranX = 0;						//��ʼ��X�����ƶ�����
		static float zpos = 0;						//��ʼ��z�����ƶ�����
		static bool start = false;					//��Ϸ��ʼ��־
		static int scores = 0;						//��ʼ����Ϸ�÷�
		static int scoreIterms = 0;					//��ʼ�����ߵ÷�
		static int scoreboard[4] = { 0,0,0,0 };		//��ʼ���÷ְ�����

		//������ײ�㷨
		for (int i = 0; i < 24; i++)
		{	//��ȡ������ߵľ���
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
		//��Ϸ��ʼ�󣬳���15��λ/timedelta���ٶ�ǰ��
		if (start)
		{
			zpos -= timeDelta * 15;
			scores = scoreIterms + (int)(2 * zpos);
			//�յ���
			if (zpos > 400)
			{
				zpos = 400;
			}
			//���ҷ���
			if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
				tranX += 3.0f * timeDelta;
			if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
				tranX -= 3.0f * timeDelta;
			//�߽���
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
		//���ñ任����ϵ
		//��ʼ��������� ����ͶӰ���� ��ȡ�۲�����ϵ
		world = XMMatrixIdentity();
		//�����ӵ� �� ���߷���
		XMVECTOR Eye = XMVectorSet(0.0f, 4.5f, zpos - 4, 0.0f);
		XMVECTOR At = XMVectorSet(0.0f, 0.0f, 5.0f + zpos, 0.0f);

		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		view = XMMatrixLookAtLH(Eye, At, Up);

		projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);

		//����������ϵ�任�������õ�Effect�����
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);				//������������ϵ
		effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);				//���ù۲�����ϵ
		effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection);    //����ͶӰ����ϵ

		//����technique
		D3DX11_TECHNIQUE_DESC techDesc;
		technique->GetDesc(&techDesc);

		//������
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRoad);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 36);
		//����·�յ�
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureEnd);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 126);
		//����·���Ե��̵�
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureLawn);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(12, 42);

		immediateContext->OMSetBlendState(blendStateAlpha, BlendFactor, 0xffffffff);
		immediateContext->RSSetState(NoCullRS);

		//����
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

		//�������
		world = XMMatrixIdentity();
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTree2);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(12, 108);

		//���ڲ���
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTree3);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(12, 66);
		world = XMMatrixIdentity();
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);

		//��һ���ڲ�����x������ƽ��500��λ
		world = world * XMMatrixTranslation(0.0f, 0.0f, 500.0f);
		for (int i = 0; i < 70; i++)
		{
			world = world * XMMatrixTranslation(0.0f, 0.0f, -7.288f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTree6);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 102);
		}

		//ѭ��ƽ�ƻ����ڲ���
		world = XMMatrixIdentity();
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
		//��һ���ڲ�����x������ƽ��500��λ
		world = world * XMMatrixTranslation(20.0f, 0.0f, 500.0f);
		for (int i = 0; i < 70; i++)
		{
			world = world * XMMatrixTranslation(0.0f, 0.0f, -7.288f);
			effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTree6);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 102);
		}

		
		//������
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

		//����
		//��ʼλ����Ϊ(tranX, 0.0f, zpos)
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
			//�������
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureLeft);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 12);
			//�����Ҳ�
			effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRight);
			technique->GetPassByIndex(0)->Apply(0, immediateContext);
			immediateContext->Draw(6, 6);
		
		}
		//����ͷ
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureTop);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 18);
		//����β
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureBack);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 0);

		//����ʼ��Ϸ
		world = XMMatrixIdentity();
		//��ʼλ����Ϊ���ĵ�ǰλ��
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
		//����
		immediateContext->OMSetBlendState(0, 0, 0xffffffff);
		immediateContext->RSSetState(0);

		swapChain->Present(0, 0);

	}
	return true;
}

//**************��ܺ���******************

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

	//��ʼ��
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