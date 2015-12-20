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
		LIBRARY_API virtual bool CreateTextureFromFile(const wchar_t*, Texture**) override;
		LIBRARY_API virtual bool Initialise(Window*) override;
		LIBRARY_API virtual Buffer* InitVertexBuffer(Vertex[], unsigned) override;
		LIBRARY_API virtual Buffer* InitIndexBuffer(unsigned short[], unsigned) override;
		LIBRARY_API virtual void DrawMesh(Mesh&, SubObject&) override;
		LIBRARY_API virtual void Update() override;
		LIBRARY_API virtual void Draw() override;

	private:
		GraphicsDirectX(GraphicsDirectX const&) = delete;
		void operator=(GraphicsDirectX const&) = delete;

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
		ID3D11Buffer*           _pConstantBuffer = nullptr;
		XMFLOAT4X4              _world;
		XMFLOAT4X4              _view;
		XMFLOAT4X4              _projection;

		XMFLOAT3 lightDir;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;

		ID3D11DepthStencilView* _depthStencilView = nullptr;
		ID3D11Texture2D* _depthStencilBuffer = nullptr;

		ID3D11SamplerState* _pSamplerLinear = nullptr;

		HRESULT InitDevice();
		void Cleanup();
		HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
		HRESULT InitShadersAndInputLayout();
		void UpdateConstantBuffer(const TreeNode&);

		UINT _windowHeight;
		UINT _windowWidth;

		ID3D11DepthStencilState* DSLessEqual;
		ID3D11RasterizerState* RSCullNone;

		ID3D11RasterizerState* CCWcullMode;
		ID3D11RasterizerState* CWcullMode;

		TreeNode* _pRoot = nullptr;
		CameraNode* _pCamera = nullptr;
	};
};