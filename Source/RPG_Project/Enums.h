#pragma once

UENUM(BlueprintType)
enum class ECharacterEquipState : uint8
{
	ECES_Unequipped UMETA(DisplayName = "Unequipped"),
	ECES_EquippedOneHandSword UMETA(DisplayName = "Equipped One Hand Sword")
};
UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking")
};