#include "ComponentFactory.h"
#include "DDSTextureLoader.h"
#include "GraphicsDirectX.h"
#include "SceneGraph.h"
#include "ServiceLocator.h"
#include "Window.h"

namespace Indecisive
{
	GraphicsDirectX::~GraphicsDirectX()
	{
		Cleanup();
	}

	bool GraphicsDirectX::CreateTextureFromFile(const wchar_t* file, Texture** ppTexture)
	{
		if (FAILED(CreateDDSTextureFromFile(_pd3dDevice, file, nullptr, (ID3D11ShaderResourceView**)ppTexture)))
		{
			// TODO: Error Handling
			if (ppTexture != nullptr) delete *ppTexture;
			ppTexture = nullptr;
			return false;
		}
		return true;
	}

	bool GraphicsDirectX::Initialise(Window* pWindow)
	{
		_hWnd = pWindow->GetHWND();
		_windowWidth = pWindow->GetWidth();
		_windowHeight = pWindow->GetHeight();

		if (FAILED(InitDevice()))
		{
			Cleanup();
			// TODO: Error Handling
			return false;
		}

		// Initialize the world matrix
		XMStoreFloat4x4(&_world, XMMatrixIdentity());

		_pCamera = static_cast<CameraNode*>(ServiceLocatorInstance()->Get("camera"));
		if (_pCamera == nullptr)
		{
			// TODO: Error Handling
			return false;
		}

		// Initialize the view matrix
		XMStoreFloat4x4(&_view, XMMatrixLookAtLH(_pCamera->eye, _pCamera->center, _pCamera->up));

		// Initialize the projection matrix
		XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _windowWidth / (FLOAT)_windowHeight, _pCamera->nearZ, _pCamera->farZ));

		lightDir = XMFLOAT3(0.0f, 100.0f, -150.0f);
		ambient = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
		diffuse = XMFLOAT4(0.3f, 0.3f, 0.2f, 1.0f);

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
			// TODO: Error Handling
			MessageBox(nullptr,
				L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
			return hr;
		}

		// Create the vertex shader
		hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);

		if (FAILED(hr))
		{
			// TODO: Error Handling
			pVSBlob->Release();
			return hr;
		}

		// Compile the pixel shader
		ID3DBlob* pPSBlob = nullptr;
		hr = CompileShaderFromFile(L"Lighting.fx", "PS", "ps_4_0", &pPSBlob);

		if (FAILED(hr))
		{
			// TODO: Error Handling
			MessageBox(nullptr,
				L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
			return hr;
		}

		// Create the pixel shader
		hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
		pPSBlob->Release();

		if (FAILED(hr))
		{
			// TODO: Error Handling
			return hr;
		}

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
		{
			// TODO: Error Handling
			return hr;
		}

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

		if (FAILED(hr))
		{
			// TODO: Error Handling
			return hr;
		}

		return hr;
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
			// TODO: Error Handling
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
			// TODO: Error Handling
			return nullptr;
		}

		return pBuffer;
	}

	HRESULT GraphicsDirectX::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr;

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
			// TODO: Error Handling
			return hr;
		}

		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}

	HRESULT GraphicsDirectX::InitDevice()
	{
		HRESULT hr;

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
		{
			// TODO: Error Handling
			return hr;
		}

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		if (FAILED(hr))
		{
			// TODO: Error Handling
			return hr;
		}

		hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
		pBackBuffer->Release();

		if (FAILED(hr))
		{
			// TODO: Error Handling
			return hr;
		}

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
		{
			// TODO: Error Handling
			return hr;
		}

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

		hr = _pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
		
		if (FAILED(hr))
		{
			// TODO: Error Handling
			return hr;
		}

		hr = _pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

		if (FAILED(hr))
		{
			// TODO: Error Handling
			return hr;
		}

		_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

		// Rasterizer
		D3D11_RASTERIZER_DESC cmdesc;

		ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
		cmdesc.FillMode = D3D11_FILL_SOLID;
		cmdesc.CullMode = D3D11_CULL_NONE;
		hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &RSCullNone);

		if (FAILED(hr))
		{
			// TODO: Error Handling
			return hr;
		}

		D3D11_DEPTH_STENCIL_DESC dssDesc;
		ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dssDesc.DepthEnable = true;
		dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		hr = _pd3dDevice->CreateDepthStencilState(&dssDesc, &DSLessEqual);

		if (FAILED(hr))
		{
			// TODO: Error Handling
			return hr;
		}

		ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

		cmdesc.FillMode = D3D11_FILL_SOLID;
		cmdesc.CullMode = D3D11_CULL_BACK;

		cmdesc.FrontCounterClockwise = true;
		hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &CCWcullMode);

		if (FAILED(hr))
		{
			// TODO: Error Handling
			return hr;
		}

		cmdesc.FrontCounterClockwise = false;
		hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &CWcullMode);

		if (FAILED(hr))
		{
			// TODO: Error Handling
			return hr;
		}

		return hr;
	}

	void GraphicsDirectX::Cleanup()
	{
		if (_pImmediateContext) _pImmediateContext->ClearState();
		if (_pConstantBuffer) _pConstantBuffer->Release();
		if (_pVertexLayout) _pVertexLayout->Release();
		if (_pVertexShader) _pVertexShader->Release();
		if (_pPixelShader) _pPixelShader->Release();
		if (_pRenderTargetView) _pRenderTargetView->Release();
		if (_pSwapChain) _pSwapChain->Release();
		if (_pImmediateContext) _pImmediateContext->Release();
		if (_pd3dDevice) _pd3dDevice->Release();
		if (_pSamplerLinear) _pSamplerLinear->Release();
	}

	void GraphicsDirectX::Update()
	{
		// Update our time
		static float t = 0.0f;

		//To be removed (placed in game loop)
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

		// Animate the cube
		//XMStoreFloat4x4(&_world, XMMatrixRotationZ(t));

		_pCamera->Update(t);
		UpdateConstantBuffer(*_pCamera);
	}

	void GraphicsDirectX::Draw()
	{
		// Clear the back buffer
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
		_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
		// Clear depth view to depth value 1.0f
		_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// Set shaders, samplers, and constant buffers
		_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
		_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
		_pImmediateContext->PSSetSamplers(0, 1, &_pSamplerLinear);
		_pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
		_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);

		// Draws the scene graph starting from the camera
		_pCamera->Draw();

		// Present our back buffer to our front buffer
		_pSwapChain->Present(0, 0);
	}
	// TODO: Call this for each node and subobject somehow. Possible solution method DrawNode: Updates CB, DrawsMesh. Alt: Method takes subobject, grabs current cb, updates.
	// Alt Alt: Node::Draw doesnt do recursion, DrawNode sets initial cb, calls DrawNode on children, updates world for all, updates lighting for subobjects.
	// Alt^3: UpdateCB(SubObject){ _cb; set lighting from mat; update cb; } SetCB(Node){ _cb->World = world; set lighting; update cb; }
	void GraphicsDirectX::UpdateConstantBuffer(const TreeNode& n)
	{
		// World Matrices
		auto world = n.world * (n.parent != nullptr ? n.parent->world : _world);
		ConstantBuffer cb;
		// Set matrices
		cb.mWorld = XMMatrixTranspose(world);
		cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&_view));
		cb.mProjection = XMMatrixTranspose(XMLoadFloat4x4(&_projection));
		// Set lighting
		cb.diffuseMtrl = diffuse;
		cb.diffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		cb.ambientMtrl = ambient;
		cb.ambientLight = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
		cb.specularMtrl = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		cb.specularLight = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		cb.specularPower = 5.0f;
		cb.lightVecW = lightDir;
		// Set camera position
		cb.eyePos = XMFLOAT3(0.0f, 100.0f, -150.0f);
		cb.hasTexture = 0.0f;
		// Update constant buffer
		_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
		// Call for all children
		for (const auto& pair : n.children) UpdateConstantBuffer(*pair.second);
	}

	void GraphicsDirectX::DrawMesh(Mesh& m, SubObject& s)
	{
		_pImmediateContext->IASetVertexBuffers(0, 1, (ID3D11Buffer**)&m.vertexBuffer, &m.vertexBufferStride, &m.vertexBufferOffset);
		_pImmediateContext->IASetIndexBuffer((ID3D11Buffer*)m.indexBuffer, DXGI_FORMAT_R16_UINT, m.indexBufferOffset);
		_pImmediateContext->PSSetShaderResources(1, 1, &s.specularTexture);
		_pImmediateContext->PSSetShaderResources(0, 1, &s.diffuseTexture);
		_pImmediateContext->DrawIndexed(s.indexSize, s.indexStart, 0);
	}
};
