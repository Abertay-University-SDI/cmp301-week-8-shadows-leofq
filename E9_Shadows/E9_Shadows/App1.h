// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "TextureShader.h"
#include "ShadowShader.h"
#include "DepthShader.h"
#include "LightShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void depthPass();
	void finalPass();
	void gui();

private:
	TextureShader* textureShader;
	PlaneMesh* mesh;

	Light* light;
	AModel* model;
	ShadowShader* shadowShader;
	DepthShader* depthShader;
	SphereMesh* sphere;
	OrthoMesh* orthoMesh;
	ShadowMap* shadowMap;
	ShadowMap* shadowMap1;

	OrthoMesh* shadowOrtho;

	ID3D11ShaderResourceView* shadowMaps[2];
	CubeMesh* cube;
	float rotate;
	float lightPosX;

	Light* light1;
	Light* light2;
	Light* lights[2];

	XMFLOAT3 lightpos ;
	XMFLOAT3 lightpos2 ;
	XMFLOAT3 lightpos3 ;
	XMFLOAT3 lightrotation;
};

#endif