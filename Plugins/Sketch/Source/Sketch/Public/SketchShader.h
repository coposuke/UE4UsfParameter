#pragma once

#include "GlobalShader.h"
#include "UniformBuffer.h"

BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT(FConstantParameters, )
 SHADER_PARAMETER(int, actorsNum)
 SHADER_PARAMETER(FVector4, resolution)
 SHADER_PARAMETER(FVector4, viewOrigin)
 SHADER_PARAMETER(FMatrix, viewMatrix)
 SHADER_PARAMETER(FMatrix, projMatrix)
END_GLOBAL_SHADER_PARAMETER_STRUCT()

BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT(FVariableParameters, )
 SHADER_PARAMETER(float, time)
END_GLOBAL_SHADER_PARAMETER_STRUCT()

typedef TUniformBufferRef<FConstantParameters> FConstantParametersRef;
typedef TUniformBufferRef<FVariableParameters> FVariableParametersRef;

class FSketchShaderVS : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FSketchShaderVS, Global);
	
public:
	FSketchShaderVS() {}
	explicit FSketchShaderVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	static bool ShouldCache(EShaderPlatform Platform) { return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::ES2); }
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& PermutationParams) { return true; }
	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment);
	virtual bool Serialize(FArchive& Ar) override;

	void SetUniformBuffers(FRHICommandList& commandList, FConstantParameters& constants, FVariableParameters& variables);
	void SetStructuredBuffers(FRHICommandList& commandList, FShaderResourceViewRHIRef& structuredBuffer);

private:
	FShaderResourceParameter srpBuffer;
};


class FSketchShaderPS : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FSketchShaderPS, Global);
	
public:
	FSketchShaderPS() {}
	explicit FSketchShaderPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	static bool ShouldCache(EShaderPlatform Platform) { return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::ES2); }
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& PermutationParams) { return true; }
	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment);
	virtual bool Serialize(FArchive& Ar) override;
    
	void SetUniformBuffers(FRHICommandList& commandList, FConstantParameters& constants, FVariableParameters& variables);
	void SetTexture(FRHICommandList& commandList, FTextureRHIParamRef& texture);
	void SetStructuredBuffers(FRHICommandList& commandList, FShaderResourceViewRHIRef& structuredBuffer);

private:
	FShaderResourceParameter srpBuffer;
	FShaderResourceParameter srpTexture;
};

struct FSketchVertex
{
    FVector4    Position;
    FVector2D   UV;
};

class FSketchVertexDeclaration : public FRenderResource
{
public:
    FVertexDeclarationRHIRef VertexDeclarationRHI;

    virtual void InitRHI() override
    {
        FVertexDeclarationElementList Elements;
        uint32 Stride = sizeof(FSketchVertex);
        Elements.Add(FVertexElement(0, STRUCT_OFFSET(FSketchVertex, Position), VET_Float4, 0, Stride));
        Elements.Add(FVertexElement(0, STRUCT_OFFSET(FSketchVertex, UV), VET_Float2, 1, Stride));
        VertexDeclarationRHI = RHICreateVertexDeclaration(Elements);
    }
  
    virtual void ReleaseRHI() override
    {
        VertexDeclarationRHI->Release();
    }
};