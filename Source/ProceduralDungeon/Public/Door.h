/*
 * MIT License
 *
 * Copyright (c) 2019-2022, 2023 Benoit Pelletier
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralDungeonTypes.h"
#include "Door.generated.h"

class URoom;

UCLASS(Blueprintable, ClassGroup = "Procedural Dungeon")
class PROCEDURALDUNGEON_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	ADoor();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }

public:
	void SetConnectingRooms(URoom* RoomA, URoom* RoomB);

	UFUNCTION(BlueprintPure, Category = "Door", meta = (CompactNodeTitle = "Is Locked"))
	FORCEINLINE bool IsLocked() { return bLocked; }
	UFUNCTION(BlueprintPure, Category = "Door", meta = (CompactNodeTitle = "Is Open"))
	FORCEINLINE bool IsOpen() { return bIsOpen; }

	// TODO: In next major release, replace OpenDoor() and CloseDoor() with a unique function OpenDoor(bool)
	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor() { bShouldBeOpen = true; }
	UFUNCTION(BlueprintCallable, Category = "Door")
	void CloseDoor() { bShouldBeOpen = false; }

	UFUNCTION(BlueprintCallable, Category = "Door")
	void Lock(bool lock) { bShouldBeLocked = lock; }

protected:
	UFUNCTION()
	virtual void OnDoorLock() {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Door", meta = (DisplayName = "On Locked"))
	void OnDoorLock_BP();

	UFUNCTION()
	virtual void OnDoorUnlock() {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Door", meta = (DisplayName = "On Unlocked"))
	void OnDoorUnlock_BP();

	UFUNCTION()
	virtual void OnDoorOpen() {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Door", meta = (DisplayName = "On Open"))
	void OnDoorOpen_BP();

	UFUNCTION()
	virtual void OnDoorClose() {}
	UFUNCTION(BlueprintImplementableEvent, Category = "Door", meta = (DisplayName = "On Close"))
	void OnDoorClose_BP();

protected:
	bool bLocked = false;
	bool bIsOpen = false;

	UPROPERTY(Replicated)
	bool bShouldBeLocked = false;

	UPROPERTY(Replicated)
	bool bShouldBeOpen = false;

	// The two connected rooms to this door
	UPROPERTY()
	URoom* RoomA;
	UPROPERTY()
	URoom* RoomB;

	UPROPERTY(ReplicatedUsing=OnRep_IndexRoomA)
	int IndexRoomA;
	UPROPERTY(ReplicatedUsing=OnRep_IndexRoomB)
	int IndexRoomB;

	UPROPERTY(EditAnywhere, Category = "Door", meta = (DisplayName = "Always Visible"))
	bool bAlwaysVisible = false;

	UPROPERTY(EditAnywhere, Category = "Door", meta = (DisplayName = "Always Unlocked"))
	bool bAlwaysUnlocked = false;

private:
	UFUNCTION() // Needed macro for replication to work
	void OnRep_IndexRoomA();

	UFUNCTION() // Needed macro for replication to work
	void OnRep_IndexRoomB();

public:
	static void DrawDebug(UWorld* World, FIntVector DoorCell = FIntVector::ZeroValue, EDoorDirection DoorRot = EDoorDirection::NbDirection, FTransform Transform = FTransform::Identity, bool includeOffset = false, bool isConnected = true, bool isValid = true);
};
