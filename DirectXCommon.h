#pragma once
#include "WinApp.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxgidebug.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

class DirectXCommon {
public:
	~DirectXCommon();
	void Initialize(WinApp* winApp);
	void CreateDXGIDevice();
	void CreateCommand();
	void CreateSwapChain();
	ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	void CreateRenderTargetView();
	void CreateShaderResourceView();
	void CreateFence();
	void PreDraw();
	void PostDraw();
	ID3D12Device* GetDevice() { return device_; };
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc_; };
	D3D12_RENDER_TARGET_VIEW_DESC GetRenderTargetViewDesc() { return rtvDesc_; };
	ID3D12DescriptorHeap* GetSRVDescriptorHeap() { return srvDescriptorHeap_; };
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_; };
	WinApp* GetWinApp() { return winApp_; };

	void CreateMultiPassResource();
	void CreateMultiPassRTV();
	void CreateMultiPassSRV();
	void FirstPassPreDraw();
	void FirstPassPostDraw();
	void SecondPassPreDraw();
	void SecondPassPostDraw();
	void PreHorizontalBlur();
	void PostHorizontalBlur();
	void PreVerticalBlur();
	void PostVerticalBlur();

private:
	//WinApp
	WinApp* winApp_ = nullptr;
	//デバッグレイヤー
	ID3D12Debug1* debugController_ = nullptr;
	//DXGIファクトリー
	IDXGIFactory7* dxgiFactory_ = nullptr;
	//アダプタ
	IDXGIAdapter4* useAdapter_ = nullptr;
	//デバイス
	ID3D12Device* device_ = nullptr;
	//コマンドキュー
	ID3D12CommandQueue* commandQueue_ = nullptr;
	//コマンドアロケータ
	ID3D12CommandAllocator* commandAllocator_ = nullptr;
	//コマンドリストを生成
	ID3D12GraphicsCommandList* commandList_ = nullptr;
	//スワップチェーン
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	IDXGISwapChain4* swapChain_ = nullptr;
	//ディスクリプタヒープ
	ID3D12DescriptorHeap* rtvDescriptorHeap_ = nullptr;
	ID3D12DescriptorHeap* srvDescriptorHeap_ = nullptr;
	//RenderTargetView
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	ID3D12Resource* swapChainResources_[2] = { nullptr };
	//フェンス
	ID3D12Fence* fence_ = nullptr;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_ = nullptr;

	//一パス目用
	ID3D12DescriptorHeap* multiPassRTVDescriptorHeap_ = nullptr;
	ID3D12Resource* firstPassResource_ = nullptr;

	//二パス目用
	ID3D12Resource* secondPassResources_[2];

	//縮小バッファ
	ID3D12Resource* shrinkBuffer_[2];
};