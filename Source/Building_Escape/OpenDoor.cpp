// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"



// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;
	
}

void UOpenDoor::OpenDoor(float DeltaTime)
{

    float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * 0.6f);
	FRotator DoorRot = GetOwner()->GetActorRotation();
	DoorRot.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRot);


}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UE_LOG(LogTemp, Warning, TEXT("The Yaw of %s is %f"), *GetOwner()->GetName(), GetOwner()->GetActorRotation().Yaw);
	
	if(PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
	OpenDoor(DeltaTime);
	}

}

