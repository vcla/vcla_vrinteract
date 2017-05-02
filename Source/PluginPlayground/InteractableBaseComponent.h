// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractableBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PLUGINPLAYGROUND_API UInteractableBaseComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableBaseComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grabbing")
		void OnGrab(UHandObject* HandObject);

	virtual void OnGrab_Implementation(UHandObject* HandObject);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grabbing")
	void WhileGrabbed(UHandObject* HandObject);

	virtual void WhileGrabbed_Implementation(UHandObject* HandObject);
	
protected:
	FTransform PreviousHandTransform;
};
