// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"




// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	FindAudioComponent();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

		UE_LOG(LogTemp, Warning, TEXT("value of delay is %f"), DoorCloseDelay);

	if(!PressurePlate)
	{
		UE_LOG(LogTemp,Error,TEXT("%s isn't working properly with the OpenDoor Component."),*GetOwner()->GetName());
	}
	
	
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * OpeningSpeed);
	FRotator DoorRot = GetOwner()->GetActorRotation();
	DoorRot.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRot);
	
	WasCloseSoundPlayed = false;

	if(!DoorSound){return;}

	if(!WasOpenSoundPlayed)
	{

	DoorSound->Play();

	WasOpenSoundPlayed = true;

	}

}

void UOpenDoor::CloseDoor(float DeltaTime)
{
     
	
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * ClosingSpeed);
	FRotator DoorRot = GetOwner()->GetActorRotation();
	DoorRot.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRot);

	if(!DoorSound){return;}
	
	WasOpenSoundPlayed = false;

	if(!WasCloseSoundPlayed)
	{

	DoorSound->Play();

	WasCloseSoundPlayed = true;

	}


}


void UOpenDoor::FindAudioComponent()
{
	DoorSound = GetOwner()->FindComponentByClass<UAudioComponent>();

	if(!DoorSound)
	{
		UE_LOG(LogTemp, Error, TEXT("DoorSound Null pointer on %s"), *GetOwner()->GetName());
	}
}


float UOpenDoor::TotalMassOfActors() const 
{
float TotalMass = 0.0f;

TArray<AActor*> OverlappingActors;

if (!PressurePlate){return TotalMass;}

PressurePlate->GetOverlappingActors(OUT OverlappingActors);


for(AActor* Actor : OverlappingActors)
{
	
	TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

}

return TotalMass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	if(PressurePlate && TotalMassOfActors() > MassToOpenDoor)
	{
	OpenDoor(DeltaTime);
	DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if(GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
	{
		CloseDoor(DeltaTime);
	}
	
	
}

