// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTankPawnChild.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AToonTankPawnChild::AToonTankPawnChild()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AToonTankPawnChild::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(GetController());
}

// Called every frame
void AToonTankPawnChild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotate();
	Move();

	if (PlayerControllerRef)
	{	
		FHitResult TraceHitResult;
		PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
		FVector HitLocation = TraceHitResult.ImpactPoint;

		RotateTurret(HitLocation);
	}
}

// Called to bind functionality to input
void AToonTankPawnChild::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AToonTankPawnChild::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Turn", this, &AToonTankPawnChild::CalculateRotateInput);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AToonTankPawnChild::Fire);

}

void AToonTankPawnChild::HandleDestruction()
{
	Super::HandleDestruction();

	bIsPlayerAlive = false;

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

bool AToonTankPawnChild::GetIsPlayerAlive()
{
	return bIsPlayerAlive;
}

void AToonTankPawnChild::CalculateMoveInput(float Value)
{
	MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void AToonTankPawnChild::CalculateRotateInput(float Value)
{
	float RotateAmout = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0.0f, RotateAmout, 0.0f);
	RotationDirection = FQuat(Rotation);
}

void AToonTankPawnChild::Move()
{
	AddActorLocalOffset(MoveDirection, true);
}

void AToonTankPawnChild::Rotate()
{
	AddActorLocalRotation(RotationDirection, true);
}
