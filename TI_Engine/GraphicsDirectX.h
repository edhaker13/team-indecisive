#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include "IGraphics.h"
#include "Logger.h"
#include "Structures.h"
#include "..\TI_Input\Input.h"

namespace Indecisive
{
	using namespace DirectX;

	struct TreeNode;
	struct CameraNode;

	struct ConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;

		XMFLOAT4 diffuseMtrl;
		XMFLOAT4 diffuseLight;

		XMFLOAT4 ambientMtrl;
		XMFLOAT4 ambientLight;

		XMFLOAT4 specularMtrl;
		XMFLOAT4 specularLight;
		float	 specularPower;

		XMFLOAT3 eyePos;

		XMFLOAT3 lightVecW;

		float hasTexture;
	};

	class GraphicsDirectX : public IGraphics
	{
	public:
		LIBRARY_API GraphicsDirectX() : IGraphics() {};
		~GraphicsDirectX();
		LIBRARY_API virtual bool CreateTextureFromFile(const wchar_t*, Texture**) const override;
		LIBRARY_API virtual bool Initialise(const Window*) override;
		LIBRARY_API virtual Buffer* InitVertexBuffer(const Vertex[], unsigned) const override;
		LIBRARY_API virtual Buffer* InitIndexBuffer(const unsigned short[], unsigned) const override;
		LIBRARY_API virtual void DrawMesh(const Mesh&, const SubObject&) const override;
		LIBRARY_API virtual void Update() override;
		LIBRARY_API virtual void Draw() const override;

	private:
		GraphicsDirectX(GraphicsDirectX const&) = delete;
		void operator=(GraphicsDirectX const&) = delete;

		D3D_DRIVER_TYPE          _driverType = D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL        _featureLevel = D3D_FEATURE_LEVEL_11_0;

		// DirectX API pointers

		HWND                     _hWnd = nullptr;
		ID3D11Device*            _pd3dDevice = nullptr;
		ID3D11RasterizerState*   _pCCWcullMode = nullptr;
		ID3D11Buffer*            _pConstantBuffer = nullptr;
		ID3D11RasterizerState*   _pCWcullMode = nullptr;
		ID3D11Texture2D*         _pDepthStencilBuffer = nullptr;
		ID3D11DepthStencilView*  _pDepthStencilView = nullptr;
		ID3D11DepthStencilState* _pDSLessEqual = nullptr;
		ID3D11DeviceContext*     _pImmediateContext = nullptr;
		ID3D11PixelShader*       _pPixelShader = nullptr;
		ID3D11RenderTargetView*  _pRenderTargetView = nullptr;
		ID3D11RasterizerState*   _pRSCullNone = nullptr;
		ID3D11SamplerState*      _pSamplerLinear = nullptr;
		IDXGISwapChain*          _pSwapChain = nullptr;
		ID3D11InputLayout*       _pVertexLayout = nullptr;
		ID3D11VertexShader*      _pVertexShader = nullptr;

		// Scene graph pointers

		TreeNode* _pRoot = nullptr;
		CameraNode* _pCamera = nullptr;

		Input* _pInput = nullptr;

		// Constants in Constant Buffer. Well they don't change much
		XMFLOAT4X4 _world;
		XMFLOAT4X4 _view;
		XMFLOAT4X4 _projection;
		XMFLOAT3 lightDir;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;

		UINT _windowHeight;
		UINT _windowWidth;

		HRESULT InitDevice();
		void Cleanup();
		HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
		HRESULT InitShadersAndInputLayout();
		void UpdateConstantBuffer(const TreeNode&, const XMMATRIX&);
	};
};