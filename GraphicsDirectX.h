#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include "IGraphics.h"
#include "Structures.h"

namespace Indecisive
{
	using namespace DirectX;

	class GameObject;

	struct ConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;
	};

	class GraphicsDirectX : public IGraphics
	{
	public:
		GraphicsDirectX(): IGraphics() {};
		~GraphicsDirectX();
		virtual HRESULT Initialise(HINSTANCE hInstance, int nCmdShow) override;
		virtual Buffer* InitVertexBuffer(SimpleVertex vertices[], unsigned arraySize) override;
		virtual Buffer* InitVertexBuffer(Vertex vertices[], unsigned arraySize) override;
		virtual Buffer* InitIndexBuffer(unsigned short indices[], unsigned arraySize) override;
		virtual void DrawGeometry(Geometry* g) override;

		virtual void Update() override;
		virtual void Draw() override;

	private:
		GraphicsDirectX(GraphicsDirectX const&) = delete;
		void operator=(GraphicsDirectX const&) = delete;

		HINSTANCE               _hInst = nullptr;
		HWND                    _hWnd = nullptr;
		D3D_DRIVER_TYPE         _driverType = D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL       _featureLevel = D3D_FEATURE_LEVEL_11_0;
		ID3D11Device*           _pd3dDevice = nullptr;
		ID3D11DeviceContext*    _pImmediateContext = nullptr;
		IDXGISwapChain*         _pSwapChain = nullptr;
		ID3D11RenderTargetView* _pRenderTargetView = nullptr;
		ID3D11VertexShader*     _pVertexShader = nullptr;
		ID3D11PixelShader*      _pPixelShader = nullptr;
		ID3D11InputLayout*      _pVertexLayout = nullptr;
		ID3D11Buffer*           _pVertexBuffer = nullptr;
		ID3D11Buffer*           _pIndexBuffer = nullptr;
		ID3D11Buffer*           _pConstantBuffer = nullptr;
		XMFLOAT4X4              _world;
		XMFLOAT4X4              _view;
		XMFLOAT4X4              _projection;

		ID3D11DepthStencilView* _depthStencilView = nullptr;
		ID3D11Texture2D* _depthStencilBuffer = nullptr;

		ID3D11ShaderResourceView* _pTextureRV = nullptr;
		ID3D11SamplerState* _pSamplerLinear = nullptr;

		HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
		HRESULT InitDevice();
		void Cleanup();
		HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
		HRESULT InitShadersAndInputLayout();
		HRESULT InitVertexBuffer();
		HRESULT InitIndexBuffer();

		UINT _windowHeight;
		UINT _windowWidth;

		UINT _renderHeight = 1080;
		UINT _renderWidth = 1920;

		ID3D11DepthStencilState* DSLessEqual;
		ID3D11RasterizerState* RSCullNone;

		ID3D11RasterizerState* CCWcullMode;
		ID3D11RasterizerState* CWcullMode;

		GameObject* _pGameObject = nullptr;
	};
};