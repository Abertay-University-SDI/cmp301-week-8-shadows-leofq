// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Create Mesh object and shader object
	mesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	model = new AModel(renderer->getDevice(), "res/teapot.obj");
	sphere = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture(L"brick", L"res/brick1.dds");

	cube = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());

	// initial shaders
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);

	// Variables for defining shadow map
	int shadowmapWidth = 1024 ;
	int shadowmapHeight = 1024 ;
	int sceneWidth = 100;
	int sceneHeight = 100;

	// This is your shadow map
	shadowMap = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	shadowMap1 = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth/2, screenHeight/2);

	lightpos = XMFLOAT3(10, 5, 2);
	lightpos2 = XMFLOAT3(8.5, 5, 2);
	lightpos3 = XMFLOAT3(0.f, 0.f, -10.f);
	lightrotation = XMFLOAT3(-707, -707, 0);

	// Configure directional light
	light = new Light();
	light->setPosition(lightpos.x, lightpos.y, lightpos.z);
	light->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	light->setDirection(lightrotation.x, lightrotation.y, lightrotation.z);
	light->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);

	// Configure directional light
	light1 = new Light();
	light1->setPosition(lightpos2.x, lightpos2.y, lightpos2.z);
	light1->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light1->setDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	light1->setDirection(-707, -707, -1);
	light1->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);

	lights[0] = light;
	lights[1] = light1;
	shadowMaps[0] = shadowMap->getDepthMapSRV();
	shadowMaps[1] = shadowMap1->getDepthMapSRV();

}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}

bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{

	// Perform depth pass
	depthPass();
	// Render scene
	finalPass();

	return true;
}

void App1::depthPass()
{
	// Set the render target to be the render to texture.
	shadowMap->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	// get the world, view, and projection matrices from the camera and d3d objects.
	light->generateViewMatrix();
	XMMATRIX lightViewMatrix = light->getViewMatrix();
	XMMATRIX lightProjectionMatrix = light->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 1.f, 5.f);

	// Render sphere
	sphere->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixRotationRollPitchYaw(0, rotate, 0), lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), sphere->getIndexCount());

	// Render shape with simple lighting shader set.
	worldMatrix += XMMatrixTranslation(15, 0, 0.f);
	cube->sendData(renderer->getDeviceContext()); //render a cube mesh
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix ); // apply lighting to cube
	depthShader->render(renderer->getDeviceContext(), cube->getIndexCount()); // excute draw call

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();

	//// ---------------------------------------------------------------------------------------------------------------------------------

	// Set the render target to be the render to texture.
	shadowMap1->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	// get the world, view, and projection matrices from the camera and d3d objects.
	light1->generateViewMatrix();
	XMMATRIX lightViewMatrix1 = light1->getViewMatrix();
	XMMATRIX lightProjectionMatrix1 = light1->getOrthoMatrix();
	XMMATRIX worldMatrix1 = renderer->getWorldMatrix();

	worldMatrix1 = XMMatrixTranslation(-50.f, 0.f, -10.f);
	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix1, lightViewMatrix1, lightProjectionMatrix1);
	depthShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	worldMatrix1 = renderer->getWorldMatrix();
	worldMatrix1 = XMMatrixTranslation(0.f, 1.f, 5.f);

	// Render sphere
	sphere->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix1 * XMMatrixRotationRollPitchYaw(0, rotate, 0), lightViewMatrix1, lightProjectionMatrix1);
	depthShader->render(renderer->getDeviceContext(), sphere->getIndexCount());

	// Render shape with simple lighting shader set.
	worldMatrix1 += XMMatrixTranslation(15, 0, 0.f);
	cube->sendData(renderer->getDeviceContext()); //render a cube mesh
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix1, lightViewMatrix1, lightProjectionMatrix1); // apply lighting to cube
	depthShader->render(renderer->getDeviceContext(), cube->getIndexCount()); // excute draw call

	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::finalPass()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
	camera->update();

	light->setPosition(lightpos.x, lightpos.y, lightpos.z);
	light1->setPosition(lightpos2.x, lightpos2.y, lightpos2.z);
	//light->setDirection(lightrotation.x, lightrotation.y, lightrotation.z);

	// get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	// Render floor
	mesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, 
		textureMgr->getTexture(L"brick"), shadowMaps, lights);
	shadowShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	// Render sphere
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 1.f, 5.f);
	sphere->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixRotationRollPitchYaw(0, rotate, 0), viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), shadowMaps, lights);
	shadowShader->render(renderer->getDeviceContext(), sphere->getIndexCount());

	worldMatrix += XMMatrixTranslation(15, 0, 0.f);
	cube->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix , viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), shadowMaps, lights);
	shadowShader->render(renderer->getDeviceContext(), cube->getIndexCount());

	sphere->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixTranslation(lightpos.x, lightpos.y, lightpos.z), viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"));
	textureShader->render(renderer->getDeviceContext(), cube->getIndexCount());

	sphere->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix * XMMatrixTranslation(lightpos2.x, lightpos2.y, lightpos2.z), viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"));
	textureShader->render(renderer->getDeviceContext(), cube->getIndexCount());

	gui();
	renderer->endScene();
}



void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	ImGui::SliderFloat("teapot rotation", &rotate, 0, 40);
	ImGui::SliderFloat("light 1 x", &lightpos.x, 0, 40);
	ImGui::SliderFloat("light 1 y", &lightpos.y, 0, 40);
	ImGui::SliderFloat("light 1 z", &lightpos.z, 0, 40);
	ImGui::SliderFloat("light 2 x", &lightpos2.x, -40, 40);
	ImGui::SliderFloat("light 2 y", &lightpos2.y, -40, 40);
	ImGui::SliderFloat("light 2 z", &lightpos2.z, -40, 40);


	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

