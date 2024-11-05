#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

const int NUM_LIGHTS = 3;

class LightShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient[NUM_LIGHTS];
		XMFLOAT4 diffuse[NUM_LIGHTS];
		XMFLOAT4 position[NUM_LIGHTS];
		XMFLOAT4 direction[NUM_LIGHTS];
		XMFLOAT4 Colour[NUM_LIGHTS];

		float spotAngle;
		float constantFactor;
		float linearFactor;
		float quadraticFactor;

		int LightType;
		bool Enabled;
		float padding;
		float padding2;
	};

public:
	LightShader(ID3D11Device* device, HWND hwnd);
	~LightShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, Light* lights[]);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
};

