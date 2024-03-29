// // @2023 All rights reversed by Reverse-Alpha Studios

#include "MeleeWeapon.h"

#include "Ascend/AI/AIEntityModule.h"
#include "Ascend/AI/Actors/Humanoid/AIMeleeHumanoidModule.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


static FName NAME_DamageCollision(TEXT("Damage Collision"));

#pragma region Main
AMeleeWeapon::AMeleeWeapon()
{
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(NAME_DamageCollision);
	DamageCollision->SetupAttachment(WeaponMesh);

	DamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SetWeaponType(EWeaponType::EWT_Melee);
	FVector BoxExtent(2.0f, 2.0f, 40.0f);
	DamageCollision->SetBoxExtent(BoxExtent);

	if (DamageCollision)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageCollision is valid."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DamageCollision is NOT valid."));
	}
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("MeleeWeapon");


	EditMeleeWeaponsByType();
}

void AMeleeWeapon::EditMeleeWeaponsByType()
{

	if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::EMT_Sword)
	{

	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::EMT_Katana)
	{
		FVector BoxExtent(2.0f, 2.0f, 40.0f);
		DamageCollision->SetBoxExtent(BoxExtent);
	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::EMT_Axe)
	{

	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::EMT_Mace)
	{

	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::EMT_Dagger)
	{

	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::EMT_Club)
	{

	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::EMT_Spear)
	{

	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::EMT_Shield)
	{

	}
	else if (MeleeWeaponProperties.GetMeleeWeaponType() == EMeleeWeaponType::EMT_HiddenBlade)
	{

	}
}

#pragma endregion

#pragma region Weapon-Overlaps

// Function to handle damage by collision
void AMeleeWeapon::OnDamageCollisionBeginOverlap
(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// Check if the overlapping actor is the owner of this weapon or if the weapon is currently dealing damage
	if (OtherActor == GetOwner())
	{
		// Do not proceed if the overlapping actor is the owner or if the weapon is currently dealing damage
		return;
	}

	// Check if the overlapping component is valid and not simulating physics
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		// Do no proceed if the overlapping component is simulating physics
		return;
	}

	// Get the current melee weapon's constant weapon damage and penetration value from the weapon's properties
	float const WeaponDamage		= MeleeWeaponProperties.GetDamageValue();
	float const WeaponPenetration	= MeleeWeaponProperties.GetPenetrationValue();

	// Calculate total damage by multiplying the current damage by the weapon's penetration value
	float TotalDamage				= WeaponDamage * WeaponPenetration;

	// Calculate the durability damage by dividing the total damage by a fixed divisor
	float Divisor			= 2.0f;
	float DurabilityDamage	= TotalDamage / Divisor;

	// Get the current durability value of the melee weapon
	float CurrentDurability = MeleeWeaponProperties.GetDurability();

	// Update the durability property by subtracting the calculated durability damage
	// This simulates wear and tear on the weapon due to its usage
	MeleeWeaponProperties.SetDurability(CurrentDurability - DurabilityDamage);

	// TODO: In the future, based on durability values, the item can be unusable, breakable, or its physical damage or any multiplier can be reduced.

	if (bDealDamageAgain)
	{
		if (BaseCharacter != nullptr)
		{
			// If the weapon is spawned or picked up by an instance of the BaseCharacter class...

			// Cast AIEntities
			AAIEntityModule* AIEntity = Cast<AAIEntityModule>(OtherActor);

			if (AIEntity != nullptr)
			{
				UGameplayStatics::ApplyDamage
				(
					AIEntity,
					TotalDamage,
					BaseCharacter->GetInstigatorController(),
					this,
					UDamageType::StaticClass()
				);
				UE_LOG(LogTemp, Warning, TEXT("AIMeleeHumanoid Dealed"));
			}
		}

		if (AIMeleeHumanoid != nullptr)
		{
			// If the weapon is spawned or picked up by an instance of the AIMeleeHumanoid class...
			ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);
			if (PlayerCharacter != nullptr)
			{
				UGameplayStatics::ApplyDamage
				(
					PlayerCharacter,
					TotalDamage,
					AIMeleeHumanoid->GetInstigatorController(),
					this,
					UDamageType::StaticClass()
				);
			}
		}
		bDealDamageAgain = false;
	}
	// Set the flag to indicate that the weapon is currently dealing damage
	//// Check if the overlapping actor is an AI entity
	//AAIEntityModule* AIEntity = Cast<AAIEntityModule>(OtherActor);
	//ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);
	//if (AIEntity)
	//{
	//	// if the overlapping actor belongs to the same owner as the ai entity
	//	if (GetOwner() == AIEntity->GetOwner())
	//	{
	//		// Handle interaction between this actor and AI entity of the same owner
	//		 
	//		UE_LOG(LogTemp, Warning, TEXT("FLAG #1"));
	//	}
	//	else
	//	{
	//		// Handle interaction between this actor and AI entity of different owners
	//		// ApplyDamage to A.I entities
	//		APawn* PlayerInstigator = BaseCharacter->GetInstigator();
	//		if (PlayerInstigator != nullptr)
	//		{
	//			AController* PlayerInstigatorController = PlayerInstigator->GetController();
	//			if (PlayerInstigatorController != nullptr)
	//			{
	//				UGameplayStatics::ApplyDamage
	//				(
	//					AIEntity,
	//					TotalMeleeWeaponDamage,
	//					PlayerInstigatorController,
	//					this,
	//					UDamageType::StaticClass()
	//				);
	//				UE_LOG(LogTemp, Warning, TEXT("AIEntity Damage Done"));
	//			}
	//			else
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("PlayerInstigatorController nullptr"));
	//			}
	//		}
	//		else
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("PlayerInstigator nullptr"));
	//		}
	//	}
	//}
	//// Check if the overlapping actor is a player character
	//if (PlayerCharacter)
	//{
	//	// if the overlapping actor belongs to the same owner as the player character
	//	if (GetOwner() == PlayerCharacter->GetOwner())
	//	{
	//		// Handle interaction between this actor and player character of the same owner 
	//	}
	//	else
	//	{
	//		// Handle interaction between this actor and player character of different owners
	//		APawn* AIInstigator = AIEntity->GetInstigator();
	//		if (AIInstigator != nullptr)
	//		{
	//			AController* AIInstigatorController = AIInstigator->GetController();
	//			if (AIInstigatorController != nullptr)
	//			{
	//				// ApplyDamage to Character
	//				UGameplayStatics::ApplyDamage
	//				(
	//					PlayerCharacter,
	//					TotalMeleeWeaponDamage,
	//					AIInstigatorController,
	//					this,
	//					UDamageType::StaticClass()
	//				);
	//				UE_LOG(LogTemp, Warning, TEXT("Player Damage Done"));
	//			}
	//			else
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("AIInstigatorController nullptr"));
	//			}
	//		}
	//		else
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("AIInstigator nullptr"));
	//		}
	//	}
	//}
}

void AMeleeWeapon::ActivateWeaponOverlapDynamics(bool bActivate)
{
	if (bActivate)
	{
		if (DamageCollision->OnComponentBeginOverlap.IsBound() == false)
		{
			DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnDamageCollisionBeginOverlap);
		}
	}
	else
	{
		if (DamageCollision->OnComponentBeginOverlap.IsBound())
		{
			DamageCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AMeleeWeapon::OnDamageCollisionBeginOverlap);
			bDealDamageAgain = true;
		}
	}

	if (DamageCollision->OnComponentBeginOverlap.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap event handler is bound."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap event handler is not bound."));
	}
}

bool AMeleeWeapon::IsActorSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("AI")) && OtherActor->ActorHasTag(TEXT("AI"));
}
#pragma endregion
