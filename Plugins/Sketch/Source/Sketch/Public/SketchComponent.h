#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SketchShader.h"
#include "SketchComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKETCH_API USketchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USketchComponent();

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category=Sketch)
	class UTextureRenderTarget2D* renderTexture;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category=Sketch)
	class UTexture2D* texture2D;

protected:
	virtual void BeginPlay() override;
  	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void ExecuteInRenderThread(FRHICommandListImmediate& RHICmdList, FTextureRenderTargetResource* OutputRenderTargetResource);
	void DrawIndexedPrimitiveUP(
		FRHICommandList& RHICmdList,
		uint32 PrimitiveType,
		uint32 MinVertexIndex,
		uint32 NumVertices,
		uint32 NumPrimitives,
		const void* IndexData,
		uint32 IndexDataStride,
		const void* VertexData,
		uint32 VertexDataStride );

private:
	struct SketchData
	{
		FVector2D position;
		FVector2D acceleration;
	};

private:
	FConstantParameters constantParameters;
	FVariableParameters variableParameters;

	TResourceArray<SketchData> structuredBufferResourceArray;
	FStructuredBufferRHIRef structuredBuffer;
	FShaderResourceViewRHIRef structuredBufferSRV;
};
