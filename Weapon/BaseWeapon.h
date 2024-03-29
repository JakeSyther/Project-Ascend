// // @2023 All rights reversed by Reverse-Alpha Studios

#pragma once

#include "GameFramework/Actor.h"
#include "WeaponStructureBase.h"
#include "Ascend/Character/BaseCharacter.h"
#include "BaseWeapon.generated.h"

class ABaseCharacter;
class AAIMeleeHumanoidModule;

class UStaticMeshComponent;
class USphereComponent;
class USoundCue;
class UTexture2D;
class UCombatSystemComponent;

UCLASS()
class ASCEND_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
#pragma region Fundementals
public:	
	ABaseWeapon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ABaseCharacter* BaseCharacter;

	UPROPERTY()
	AAIMeleeHumanoidModule* AIMeleeHumanoid;

	UPROPERTY(VisibleAnywhere, Category = "DeveloperProperties|ClassProps")
	USphereComponent* AreaCollision;

	UPROPERTY(VisibleAnywhere, Category = "DeveloperProperties | ClassProps")
	UStaticMeshComponent* WeaponMesh;

public:
	USphereComponent* GetAreaCollision() const { return AreaCollision; }
	UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; };

	void SetAIMeleeHumanoid(AAIMeleeHumanoidModule* MeleeHumanoid) { AIMeleeHumanoid = MeleeHumanoid; }

#pragma endregion
#pragma region Weapon Classes

#pragma endregion
#pragma region WeaponProperties
private:
	UPROPERTY()
	ARangedWeapon* RangeWeaponClass = nullptr;

	UPROPERTY()
	AMeleeWeapon* MeleeWeaponClass  = nullptr;

public:
	void SetRangeWeapon(ARangedWeapon* RangeWeapon) { RangeWeaponClass  = RangeWeapon; }
	void SetMeleeWeapon(AMeleeWeapon* MeleeWeapon)  { MeleeWeaponClass  = MeleeWeapon; }

public:
	UPROPERTY(EditAnywhere, Category = "DeveloperProperties|ItemProps")
	EObtainingMethod ObtainingMethod;

	UPROPERTY(VisibleAnywhere, Category = "DeveloperProperties|ItemProps")
	ERarityCategory  RarityCategory;

	/*Defaults*/
	UPROPERTY(VisibleAnywhere, Category = "DeveloperProperties|ItemProps")
	EWeaponType		 WeaponType;

	UPROPERTY(VisibleAnywhere, Category = "DeveloperProperties|ItemProps")
	EWeaponState	 WeaponState;

public:
	FORCEINLINE EObtainingMethod GetObtainingMethod()	const { return ObtainingMethod; }
	FORCEINLINE EWeaponType		 GetWeaponType()		const { return WeaponType; }
	FORCEINLINE ERarityCategory  GetRarityCategory()	const { return RarityCategory; }
	FORCEINLINE EWeaponState	 GetWeaponState()		const { return WeaponState; }

	void SetObtainingMethod(EObtainingMethod newMethod) { ObtainingMethod = newMethod; }
	void SetRarityCategory(ERarityCategory ItemRarity) { RarityCategory = ItemRarity; }
	void SetWeaponType(EWeaponType newType)				{ WeaponType = newType; }
	void SetWeaponState(EWeaponState newState)
	{
		 WeaponState = newState;
		 CheckWeaponState();
	}
#pragma endregion
#pragma region ObtainingMethod

#pragma endregion
#pragma region WeaponType 

#pragma endregion
#pragma region WeaponState
public:
	void CheckWeaponState();
private:
	void InitialState();
	void EquipState();
	void DropState();
	void TradeableState();

#pragma endregion
#pragma region Overlap
private:
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function to handle actor end overlap events
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion
#pragma region Sound
private:
	UPROPERTY(EditAnywhere)	USoundCue* EquipSound;
	void PlayEquipWeaponSound();

protected:
	void InsertAndPlaySoundAtLocation(USoundCue* InsertPlayableSoundAtLocation, FVector Location);
	void InsertAndPlaySound2D(USoundCue* InsertPlayableSound2D);

#pragma endregion
};
