// Fill out your copyright notice in the Description page of Project Settings.

#include "PluginPlayground.h"
#include "InteractableBaseComponent.h"


// Sets default values for this component's properties
UInteractableBaseComponent::UInteractableBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractableBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractableBaseComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UInteractableBaseComponent::OnGrab_Implementation(UHandObject* HandObject)
{
	//PreviousHandTransform = HandTransform;
}

void UInteractableBaseComponent::WhileGrabbed_Implementation(UHandObject* HandObject)
{
	/*FTransform RotateAroundHand = FTransform();
	FVector ObjToHand = HandTransform.GetLocation() - this->GetOwner()->GetActorLocation();
	FQuat DeltaRot = PreviousHandTransform.GetRotation().Inverse() * HandTransform.GetRotation();
	FVector HandToNewObj = DeltaRot * ObjToHand * -1;
	RotateAroundHand.SetLocation(ObjToHand + HandToNewObj);
	RotateAroundHand.SetRotation(DeltaRot);

	this->GetOwner()->SetActorTransform(HandTransform, false);*/
}