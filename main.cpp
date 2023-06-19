#include <Windows.h>
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Model.h"
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "externals/imgui/imgui_impl_DX12.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {

	CoInitializeEx(0, COINIT_MULTITHREADED);

	//WindowsAPI
	WinApp* winApp = new WinApp;
	winApp->CreateGameWindow(L"CG2WindowClass", winApp->kClientWidth, winApp->kClientHeight);

	//DirectX
	DirectXCommon* directX = new DirectXCommon;
	directX->Initialize(winApp);

	//Model
	Model* model = new Model;
	model->Initialize(directX);

	//頂点データ
	ID3D12Resource* resource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//球
	resource = model->CreateBufferResource(directX->GetDevice(), sizeof(VertexData) * 1536);
	VertexData sphereVertexData[1536];
	const float pi = 3.14f;
	const uint32_t kSubdivision = 16;
	uint32_t latIndex = 0;
	uint32_t lonIndex = 0;
	//経度分割一つ分の角度φd
	const float kLonEvery = pi * 2.0f / float(kSubdivision);
	//緯度分割一つ分の角度θd
	const float kLatEvery = pi / float(kSubdivision);
	//緯度の方向に分割
	for (latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;//θ
		//経度の方向に分割しながら線を描く
		for (lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;//φ
			//頂点にデータを入力する。基準点a
			sphereVertexData[start].position.x = std::cos(lat) * std::cos(lon);
			sphereVertexData[start].position.y = std::sin(lat);
			sphereVertexData[start].position.z = std::cos(lat) * std::sin(lon);
			sphereVertexData[start].position.w = 1.0f;
			sphereVertexData[start].texcoord.x = float(lonIndex) / float(kSubdivision);
			sphereVertexData[start].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
			//残りの５頂点も順番に計算して入力していく
			sphereVertexData[start + 1].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			sphereVertexData[start + 1].position.y = std::sin(lat + kLatEvery);
			sphereVertexData[start + 1].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			sphereVertexData[start + 1].position.w = 1.0f;
			sphereVertexData[start + 1].texcoord.x = float(lonIndex) / float(kSubdivision);
			sphereVertexData[start + 1].texcoord.y = 1.0f - float(latIndex + kLatEvery) / float(kSubdivision);
			sphereVertexData[start + 2].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			sphereVertexData[start + 2].position.y = std::sin(lat);
			sphereVertexData[start + 2].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			sphereVertexData[start + 2].position.w = 1.0f;
			sphereVertexData[start + 2].texcoord.x = float(lonIndex + kLonEvery) / float(kSubdivision);
			sphereVertexData[start + 2].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
			sphereVertexData[start + 3].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			sphereVertexData[start + 3].position.y = std::sin(lat);
			sphereVertexData[start + 3].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			sphereVertexData[start + 3].position.w = 1.0f;
			sphereVertexData[start + 3].texcoord.x = float(lonIndex + kLonEvery) / float(kSubdivision);
			sphereVertexData[start + 3].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
			sphereVertexData[start + 4].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			sphereVertexData[start + 4].position.y = std::sin(lat + kLatEvery);
			sphereVertexData[start + 4].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			sphereVertexData[start + 4].position.w = 1.0f;
			sphereVertexData[start + 4].texcoord.x = float(lonIndex) / float(kSubdivision);
			sphereVertexData[start + 4].texcoord.y = 1.0f - float(latIndex + kLatEvery) / float(kSubdivision);
			sphereVertexData[start + 5].position.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			sphereVertexData[start + 5].position.y = std::sin(lat + kLatEvery);
			sphereVertexData[start + 5].position.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			sphereVertexData[start + 5].position.w = 1.0f;
			sphereVertexData[start + 5].texcoord.x = float(lonIndex + kLonEvery) / float(kSubdivision);
			sphereVertexData[start + 5].texcoord.y = 1.0f - float(latIndex + kLatEvery) / float(kSubdivision);
		}
	}


	//WVPリソース
	ID3D12Resource* transformationMatrixData = nullptr;
	Transform transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform cameraTransform = { { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,-10.0f } };
	transformationMatrixData = model->CreateBufferResource(directX->GetDevice(), sizeof(Matrix4x4));


	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	ImGui_ImplDX12_Init(directX->GetDevice(),
		directX->GetSwapChainDesc().BufferCount,
		directX->GetRenderTargetViewDesc().Format,
		directX->GetSRVDescriptorHeap(),
		directX->GetSRVDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		directX->GetSRVDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		//メインループを抜ける
		if (winApp->MessageRoop() == true) {
			break;
		}

		//ゲーム処理
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(winApp->kClientWidth) / float(winApp->kClientHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		model->UpdateMatrix(transformationMatrixData, worldViewProjectionMatrix);
		ImGui::Begin("Window");
		ImGui::End();


		//描画処理
		ImGui::Render();
		directX->PreDraw();
		model->Draw(resource, vertexBufferView, sphereVertexData, 1536, transformationMatrixData);
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directX->GetCommandList());
		directX->PostDraw();
	}

	//解放処理
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	transformationMatrixData->Release();
	resource->Release();
	delete model;
	delete directX;
	delete winApp;

	CoUninitialize();

	return 0;
}