// // @2023 All rights reversed by Reverse-Alpha Studios


#include "ProjectileModule.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"


AProjectileModule::AProjectileModule()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	SetRootComponent(CollisionBox);

	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
}

void AProjectileModule::BeginPlay()
{
	Super::BeginPlay();
	
	if (ProjectileTracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached
		(
			ProjectileTracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}

	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectileModule::OnHit);
}


void AProjectileModule::SpawnTrailSystem()
{
	if (TrailSystem)
	{
		TrailSystemComponent = 
		UNiagaraFunctionLibrary::SpawnSystemAttached
		(
			TrailSystem,
			GetRootComponent(),
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition,
			false
		);
	}
}

void AProjectileModule::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AProjectileModule::OnExplodeDamage()
{
	UGameplayStatics::ApplyRadialDamageWithFalloff
	(
		this,
		ExplosiveDamage,
		10.0f,
		GetActorLocation(),
		InnerRadiusDamage,
		OuterRadiusDamage,
		1.0f,
		UDamageType::StaticClass(),
		TArray<AActor*>(),
		this,
		GetInstigatorController()
	);
}

void AProjectileModule::Destroyed()
{
	Super::Destroyed();

	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation
		(
			GetWorld(),
			ImpactParticles,
			GetActorTransform()
		);
	}

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			this,
			ImpactSound,
			GetActorLocation()
		);
	}
}

void AProjectileModule::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer
	(
		DestroyTimer,
		this,
		&AProjectileModule::DestroyTimerFinished,
		DestroyTime
	);
}

void AProjectileModule::DestroyTimerFinished()
{
	Destroy();
}

