#include "ComponentFactory.h"
#include "DDSTextureLoader.h"
#include "GraphicsDirectX.h"
#include "ServiceLocator.h"
#include "Window.h"

namespace Indecisive
{
	GraphicsDirectX::~GraphicsDirectX()
	{
		Cleanup();
	}

	bool GraphicsDirectX::Initialise(Window* pWindow)
	{
		_hWnd = pWindow->GetHWND();
		_windowWidth = pWindow->GetWidth();
		_windowHeight = pWindow->GetHeight();

		if (FAILED(InitDevice()))
		{
			Cleanup();

			return false;
		}

		// Initialize the world matrix
		XMStoreFloat4x4(&_world, XMMatrixIdentity());

		// Initialize the view matrix
		XMVECTOR Eye = XMVectorSet(0.0f, 100.0f, -150.0f, 0.0f);
		XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		XMStoreFloat4x4(&_view, XMMatrixLookAtLH(Eye, At, Up));

		// Initialize the projection matrix
		XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _windowWidth / (FLOAT)_windowHeight, 0.01f, 1000.0f));

		lightDir = XMFLOAT3(0.0f, 100.0f, -150.0f);
		ambient = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
		diffuse = XMFLOAT4(0.3f, 0.3f, 0.2f, 1.0f);
		
		CreateDDSTextureFromFile(_pd3dDevice, L"carTex.dds", nullptr, &_pTextureRV);
		
		// Create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		_pd3dDevice->CreateSamplerState(&sampDesc, &_pSamplerLinear);

		return true;
	}

	HRESULT GraphicsDirectX::InitShadersAndInputLayout()
	{
		HRESULT hr;

		// Compile the vertex shader
		ID3DBlob* pVSBlob = nullptr;
		hr = CompileShaderFromFile(L"Lighting.fx", "VS", "vs_4_0", &pVSBlob);

		if (FAILED(hr))
		{
			MessageBox(nullptr,
				L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
			return hr;
		}

		// Create the vertex shader
		hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);

		if (FAILED(hr))
		{
			pVSBlob->Release();
			return hr;
		}

		// Compile the pixel shader
		ID3DBlob* pPSBlob = nullptr;
		hr = CompileShaderFromFile(L"Lighting.fx", "PS", "ps_4_0", &pPSBlob);

		if (FAILED(hr))
		{
			MessageBox(nullptr,
				L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
			return hr;
		}

		// Create the pixel shader
		hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
		pPSBlob->Release();

		if (FAILED(hr))
			return hr;

		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT numElements = ARRAYSIZE(layout);

		// Create the input layout
		hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(), &_pVertexLayout);
		pVSBlob->Release();

		if (FAILED(hr))
			return hr;

		// Set the input layout
		_pImmediateContext->IASetInputLayout(_pVertexLayout);

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = _pd3dDevice->CreateSamplerState(&sampDesc, &_pSamplerLinear);

		return hr;
	}

	Buffer* GraphicsDirectX::InitVertexBuffer(SimpleVertex vertices[], unsigned arraySize)
	{
		HRESULT hr;

		Buffer* pBuffer;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * arraySize;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;

		hr = _pd3dDevice->CreateBuffer(&bd, &InitData, (ID3D11Buffer**)&pBuffer);

		if (FAILED(hr))
		{
			return nullptr;
		}

		return pBuffer;
	}

	Buffer* GraphicsDirectX::InitVertexBuffer(Vertex vertices[], unsigned arraySize)
	{
		HRESULT hr;

		Buffer* pBuffer;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * arraySize;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;

		hr = _pd3dDevice->CreateBuffer(&bd, &InitData, (ID3D11Buffer**)&pBuffer);

		if (FAILED(hr))
		{
			return nullptr;
		}

		return pBuffer;
	}

	Buffer* GraphicsDirectX::InitIndexBuffer(unsigned short indices[], unsigned arraySize)
	{
		HRESULT hr;

		Buffer* pBuffer;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * arraySize;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = indices;
		hr = _pd3dDevice->CreateBuffer(&bd, &InitData, (ID3D11Buffer**)&pBuffer);

		if (FAILED(hr))
		{
			return nullptr;
		}

		return pBuffer;
	}

	HRESULT GraphicsDirectX::InitVertexBuffer()
	{
		HRESULT hr;
	
		// Create vertex buffer
		SimpleVertex vertices[] =
		{
			{ XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0, 0) },
			{ XMFLOAT3( 1.0f,  1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1, 0) },
			{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2(1, 1) },
			{ XMFLOAT3( 1.0f, -1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0, 1) },
		};
	
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
	
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;
	
		hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pVertexBuffer);
	
		if (FAILED(hr))
			return hr;
	
		return S_OK;
	}
	
	HRESULT GraphicsDirectX::InitIndexBuffer()
	{
		HRESULT hr;
	
		// Create index buffer
		WORD indices[] =
		{
			0, 1, 2,
			2, 1, 3,
		};
	
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
	
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * 6;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
	
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = indices;
		hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);
	
		if (FAILED(hr))
			return hr;
	
		return S_OK;
	}

	HRESULT GraphicsDirectX::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* pErrorBlob;
		hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

		if (FAILED(hr))
		{
			if (pErrorBlob != nullptr)
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

			if (pErrorBlob) pErrorBlob->Release();

			return hr;
		}

		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;
	}

	HRESULT GraphicsDirectX::InitDevice()
	{
		HRESULT hr = S_OK;

		UINT createDeviceFlags = 0;

#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};

		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		UINT sampleCount = 4;

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = _windowWidth;
		sd.BufferDesc.Height = _windowHeight;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = _hWnd;
		sd.SampleDesc.Count = sampleCount;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			_driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
			if (SUCCEEDED(hr))
				break;
		}

		if (FAILED(hr))
			return hr;

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		if (FAILED(hr))
			return hr;

		hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
		pBackBuffer->Release();

		if (FAILED(hr))
			return hr;

		_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, nullptr);

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)_windowWidth;
		vp.Height = (FLOAT)_windowHeight;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		_pImmediateContext->RSSetViewports(1, &vp);

		InitShadersAndInputLayout();

		/*InitVertexBuffer();

		// Set vertex buffer
		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;
		_pImmediateContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);

		InitIndexBuffer();

		// Set index buffer
		_pImmediateContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		*/

		// TODO: ASSET MANAGER
		_pGameObject = ComponentFactory::MakeTestObjectFromObj("carBody.obj");
		//_pGameObject = ComponentFactory::MakeTestObject();


		// Set primitive topology
		_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Create the constant buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ConstantBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);

		if (FAILED(hr))
			return hr;

		// Depth Stencil Stuff
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = _windowWidth;
		depthStencilDesc.Height = _windowHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = sampleCount;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
		_pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

		_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

		// Rasterizer
		D3D11_RASTERIZER_DESC cmdesc;

		ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
		cmdesc.FillMode = D3D11_FILL_SOLID;
		cmdesc.CullMode = D3D11_CULL_NONE;
		hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &RSCullNone);

		D3D11_DEPTH_STENCIL_DESC dssDesc;
		ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dssDesc.DepthEnable = true;
		dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		_pd3dDevice->CreateDepthStencilState(&dssDesc, &DSLessEqual);

		ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

		cmdesc.FillMode = D3D11_FILL_SOLID;
		cmdesc.CullMode = D3D11_CULL_BACK;

		cmdesc.FrontCounterClockwise = true;
		hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &CCWcullMode);

		cmdesc.FrontCounterClockwise = false;
		hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &CWcullMode);

		return S_OK;
	}

	void GraphicsDirectX::Cleanup()
	{
		if (_pImmediateContext) _pImmediateContext->ClearState();
		if (_pConstantBuffer) _pConstantBuffer->Release();
		if (_pVertexBuffer) _pVertexBuffer->Release();
		if (_pIndexBuffer) _pIndexBuffer->Release();
		if (_pVertexLayout) _pVertexLayout->Release();
		if (_pVertexShader) _pVertexShader->Release();
		if (_pPixelShader) _pPixelShader->Release();
		if (_pRenderTargetView) _pRenderTargetView->Release();
		if (_pSwapChain) _pSwapChain->Release();
		if (_pImmediateContext) _pImmediateContext->Release();
		if (_pd3dDevice) _pd3dDevice->Release();

		if (_pTextureRV) _pTextureRV->Release();

		if (_pSamplerLinear) _pSamplerLinear->Release();
	}

	void GraphicsDirectX::Update()
	{
		// Update our time
		static float t = 0.0f;

		//
		//To be removed (placed in game loop)
		//
		if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
		{
			t += (float)XM_PI * 0.0125f;
		}
		else
		{
			static DWORD dwTimeStart = 0;
			DWORD dwTimeCur = GetTickCount();

			if (dwTimeStart == 0)
				dwTimeStart = dwTimeCur;

			t = (dwTimeCur - dwTimeStart) / 1000.0f;
		}
		//------

		//
		// Animate the cube
		//
		XMStoreFloat4x4(&_world, XMMatrixRotationZ(t));
	}

	void GraphicsDirectX::Draw()
	{
		//
		// Clear the back buffer
		//
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
		_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
		_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);//clear depth view to depth value 1.0F

		//

		XMMATRIX world = XMLoadFloat4x4(&_world);
		XMMATRIX view = XMLoadFloat4x4(&_view);
		XMMATRIX projection = XMLoadFloat4x4(&_projection);
		
		//
		// Renders a triangle
		//
		_pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
		_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
		_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
		_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);

		_pImmediateContext->PSSetSamplers(0, 1, &_pSamplerLinear);

		_pImmediateContext->PSSetShaderResources(0, 1, &_pTextureRV);

		ConstantBuffer cb;
		cb.mWorld = XMMatrixTranspose(world);
		cb.mView = XMMatrixTranspose(view);
		cb.mProjection = XMMatrixTranspose(projection);
		
		cb.diffuseMtrl = diffuse;
		cb.diffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		cb.ambientMtrl = ambient;
		cb.ambientLight = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		cb.specularMtrl = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		cb.specularLight = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		cb.specularPower = 5.0f;

		cb.eyePos = XMFLOAT3(0.0f, 100.0f, -150.0f);
		cb.lightVecW = lightDir;

		_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

		//_pImmediateContext->DrawIndexed(6, 0, 0);
		
		// TODO: USE SCENE GRAPH
		_pGameObject->Draw();
		
		//
		// Present our back buffer to our front buffer
		//
		_pSwapChain->Present(0, 0);
	}

	void GraphicsDirectX::DrawGeometry(Geometry* g)
	{
		_pImmediateContext->IASetVertexBuffers(0, 1, (ID3D11Buffer**)&g->vertexBuffer, &g->vertexBufferStride, &g->vertexBufferOffset);
		_pImmediateContext->IASetIndexBuffer((ID3D11Buffer*)g->indexBuffer, DXGI_FORMAT_R16_UINT, g->indexBufferOffset);
		_pImmediateContext->DrawIndexed(g->indexBufferSize, 0, 0);
	}
};