#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include "IGraphics.h"
#include "DirectXMath.h"

namespace Indecisive
{
	using namespace DirectX;

	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	struct ConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;
	};

	class GraphicsDirectX : public IGraphics
	{
	public:
		GraphicsDirectX();
		~GraphicsDirectX();

		virtual HRESULT Initialise(HINSTANCE hInstance, int nCmdShow) override;

		Buffer* InitVertexBuffer(SimpleVertex** vertices, int arraySize);
		Buffer* InitIndexBuffer(unsigned short** indices, int arraySize);

		void Update();
		void Draw();

	private:
		HINSTANCE               _hInst;
		HWND                    _hWnd;
		D3D_DRIVER_TYPE         _driverType;
		D3D_FEATURE_LEVEL       _featureLevel;
		ID3D11Device*           _pd3dDevice;
		ID3D11DeviceContext*    _pImmediateContext;
		IDXGISwapChain*         _pSwapChain;
		ID3D11RenderTargetView* _pRenderTargetView;
		ID3D11VertexShader*     _pVertexShader;
		ID3D11PixelShader*      _pPixelShader;
		ID3D11InputLayout*      _pVertexLayout;
		ID3D11Buffer*           _pVertexBuffer;
		ID3D11Buffer*           _pIndexBuffer;
		ID3D11Buffer*           _pConstantBuffer;
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
	};
};