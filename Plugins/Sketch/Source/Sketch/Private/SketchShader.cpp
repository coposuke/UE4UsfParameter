#include "SketchShader.h"

IMPLEMENT_GLOBAL_SHADER_PARAMETER_STRUCT(FConstantParameters, "constants");
IMPLEMENT_GLOBAL_SHADER_PARAMETER_STRUCT(FVariableParameters, "variables");

FSketchShaderVS::FSketchShaderVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: FGlobalShader(Initializer)
{
	srpBuffer.Bind(Initializer.ParameterMap, TEXT("sketchData"));
}

void FSketchShaderVS::ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
{
	FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	OutEnvironment.CompilerFlags.Add(CFLAG_StandardOptimization);
}

bool FSketchShaderVS::Serialize(FArchive& Ar)
{
    return FGlobalShader::Serialize(Ar);
}

void FSketchShaderVS::SetUniformBuffers(FRHICommandList& commandList, FConstantParameters& constants, FVariableParameters& variables)
{
	SetUniformBufferParameter(commandList, GetVertexShader(), GetUniformBufferParameter<FConstantParameters>(),
		FConstantParametersRef::CreateUniformBufferImmediate(constants, UniformBuffer_SingleDraw));
	SetUniformBufferParameter(commandList, GetVertexShader(), GetUniformBufferParameter<FVariableParameters>(),
		FVariableParametersRef::CreateUniformBufferImmediate(variables, UniformBuffer_SingleDraw));
}

void FSketchShaderVS::SetStructuredBuffers(FRHICommandList& commandList, FShaderResourceViewRHIRef& structuredBuffer)
{
	if (srpBuffer.IsBound()) {
		commandList.SetShaderResourceViewParameter(GetVertexShader(), srpBuffer.GetBaseIndex(), structuredBuffer);
	}
}

FSketchShaderPS::FSketchShaderPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: FGlobalShader(Initializer)
{
	srpBuffer.Bind(Initializer.ParameterMap, TEXT("sketchData"));
	srpTexture.Bind(Initializer.ParameterMap, TEXT("mainTexture"));
}

void FSketchShaderPS::ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
{
	FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	OutEnvironment.CompilerFlags.Add(CFLAG_StandardOptimization);
}

bool FSketchShaderPS::Serialize(FArchive& Ar)
{
    return FGlobalShader::Serialize(Ar);
}

void FSketchShaderPS::SetUniformBuffers(FRHICommandList& commandList, FConstantParameters& constants, FVariableParameters& variables)
{
	SetUniformBufferParameter(commandList, GetPixelShader(), GetUniformBufferParameter<FConstantParameters>(),
		FConstantParametersRef::CreateUniformBufferImmediate(constants, UniformBuffer_SingleDraw));
	SetUniformBufferParameter(commandList, GetPixelShader(), GetUniformBufferParameter<FVariableParameters>(),
		FVariableParametersRef::CreateUniformBufferImmediate(variables, UniformBuffer_SingleDraw));
}

void FSketchShaderPS::SetTexture(FRHICommandList& commandList, FTextureRHIParamRef& texture)
{
	SetTextureParameter(commandList, GetPixelShader(), srpTexture, texture);
}

void FSketchShaderPS::SetStructuredBuffers(FRHICommandList& commandList, FShaderResourceViewRHIRef& structuredBuffer)
{
	if (srpBuffer.IsBound()) {
		commandList.SetShaderResourceViewParameter(GetPixelShader(), srpBuffer.GetBaseIndex(), structuredBuffer);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_SHADER_TYPE(, FSketchShaderVS, TEXT("/Plugins/Sketch/Shaders/SketchShader.usf"), TEXT("MainVS"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(, FSketchShaderPS, TEXT("/Plugins/Sketch/Shaders/SketchShader.usf"), TEXT("MainPS"), SF_Pixel);
