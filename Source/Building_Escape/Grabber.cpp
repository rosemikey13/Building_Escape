// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::HandleCheck()
{
		
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(!PhysicsHandle){

		UE_LOG(LogTemp,Error,TEXT("Physics Handle MUST be added. It is currently not present on %s."), *GetOwner()->GetName());

	}

}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	HandleCheck();
	InputCapture();
}

void UGrabber::InputCapture()
{

GrabberComp = GetOwner()->FindComponentByClass<UInputComponent>();

	if(GrabberComp){
		UE_LOG(LogTemp,Warning,TEXT("Grabber input present on %s"), *GetOwner()->GetName());
		GrabberComp->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		GrabberComp->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}

}

void UGrabber::Grab()
{

FHitResult HitResult = GetFirstPhsyicsBodyInReach();
UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
AActor* ActorHit = HitResult.GetActor();
if (ActorHit)
{
	if(!PhysicsHandle){return;}

	FVector LineTraceEnd = GetPlayerReach();

	PhysicsHandle->GrabComponentAtLocation(ComponentToGrab,NAME_None,LineTraceEnd);

}

}

FVector UGrabber::GetPlayerReach() const
{

FVector CurrentLocation;
FRotator CurrentRotation;

GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT CurrentLocation, OUT CurrentRotation);


return CurrentLocation + CurrentRotation.Vector() * Reach ;


}

void UGrabber::Release()
{
		if (!PhysicsHandle){return;}
		PhysicsHandle->ReleaseComponent();

}
FHitResult UGrabber::GetFirstPhsyicsBodyInReach() const
{


	
	FHitResult PlayerHit;
	
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());


	GetWorld()->LineTraceSingleByObjectType(OUT PlayerHit, GetPlayerWorldPos(), GetPlayerReach(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),TraceParams);
	// See what it hits

  AActor* ActorHit = PlayerHit.GetActor();



	return PlayerHit;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle){return;}
	
	if(PhysicsHandle->GrabbedComponent)
	{
		
		PhysicsHandle->SetTargetLocation(GetPlayerReach());

	}
	
}

FVector UGrabber::GetPlayerWorldPos() const
{

    FVector CurrentLocation;
	FRotator CurrentRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT CurrentLocation, OUT CurrentRotation);

	return CurrentLocation;

}