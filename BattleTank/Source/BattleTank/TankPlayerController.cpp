// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"

#define OUT

void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();

	if (GetControlledTank() != nullptr) {
		FString TankName = GetControlledTank()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s is on the Battlefield!"), *TankName);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("We lost the Tank!"));
	}
}

void ATankPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair() {
	if (!GetControlledTank()) { return; }

	FVector HitLocation; //Out parameter
	GetSightRayHitLocation(HitLocation);
	GetControlledTank()->AimAt(HitLocation);
}

bool ATankPlayerController::GetSightRayHitLocation(OUT FVector& HitLocation) const {
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(CrosshairX*ViewportSizeX, CrosshairY*ViewportSizeY);
	FVector LookDirection;

	if (GetLookDirection(ScreenLocation, LookDirection)) {
		GetLookVectorHitLocation(LookDirection, HitLocation);
	}
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {
	FVector CameraWorldLocation; //To be discarded
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, OUT CameraWorldLocation, OUT LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, OUT FVector& HitLocation) const {
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * LineTraceRange;

	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility)
	){
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false;
}