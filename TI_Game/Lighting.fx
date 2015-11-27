Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbData
{
	float4x4 World; 
	float4x4 View; 
	float4x4 Projection;

	float4 gDiffuseMtrl; 
	float4 gDiffuseLight; 

	float4 gAmbientMtrl;
	float4 gAmbientLight;

	float4 gSpecularMtrl;
	float4 gSpecularLight;
	float  gSpecPow;

	float4 gEyePosW;
	float3 gLightVecW;

	float HasTexture;


};

struct VS_IN
{
	float4 posL    : POSITION;
	float3 normalL : NORMAL;
	float2 texL    : TEXCOORD;
};

struct VS_OUT
{
	float4 Pos     : SV_POSITION;
	float3 NormW   : NORMAL;
	float3 PosW    : POSITION;
	float2 TexC    : TEXCOORD;	
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT output = (VS_OUT)0;

	output.PosW = mul(vIn.posL, World).xyz;

	output.Pos = mul(vIn.posL, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	// Convert from local to world normal
	float3 normalW = mul(float4(vIn.normalL, 0.0f), World).xyz;
	normalW = normalize(normalW);
	output.NormW = normalW;

	output.TexC = vIn.texL;

	return output;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	float3 normalW = normalize(pIn.NormW);

	float3 toEye = normalize(gEyePosW - pIn.PosW);

	// Get texture data from file
	float4 textureColour = txDiffuse.Sample(samLinear, pIn.TexC);
	
	//clip(textureColour.a - 0.2f);

	float3 ambient = float3(0.0f, 0.0f, 0.0f);
	float3 diffuse = float3(0.0f, 0.0f, 0.0f);
	float3 specular = float3(0.0f, 0.0f, 0.0f);

	float3 lightVecNorm = normalize(gLightVecW.xyz);

	// Compute reflection vector.
	float3 r = reflect(-lightVecNorm, normalW);

	// Determine how much specular light makes it into the eye.
	float specAmount = pow(max(dot(r, toEye), 0.0f), gSpecPow);

	// Determine the diffuse light intensity that strikes the vertex.
	float diffuseAmount = max(dot(lightVecNorm, normalW), 0.0f);

	// Only display specular when there is diffuse
	if (diffuseAmount <= 0.0f)
	{
		specAmount = 0.0f;
	}

	// Compute the ambient, diffuse, and specular terms separately.
	specular += specAmount * (gSpecularMtrl * gSpecularLight).rgb;
	diffuse += diffuseAmount * (gDiffuseMtrl * gDiffuseLight).rgb;
	ambient += (gAmbientMtrl * gAmbientLight).rgb;

	// Sum all the terms together and copy over the diffuse alpha.
	float4 finalColour;

	//if (HasTexture == 1.0f)
	//{
		finalColour.rgb = (textureColour.rgb * (ambient + diffuse)) + specular;
	//}
	//else
	//{
	//	finalColour.rgb = ambient + diffuse + specular;
	//}

	finalColour.a = gDiffuseMtrl.a;

	return finalColour;
}

technique11 Render
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) ); SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}
