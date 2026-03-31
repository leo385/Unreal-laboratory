// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "Data/ItemData.h"
#include "Data/ItemDefinition.h"

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	check(PickupMeshComponent != nullptr);

	// Make the mesh the root component
	SetRootComponent(PickupMeshComponent);

}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup();
	
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Retrive data values from associated pickup data table
void APickupBase::InitializePickup()
{
	// Check if data table asset is existing
	const FSoftObjectPath TablePath = PickupDataTable.ToSoftObjectPath();
	if(!TablePath.IsNull() && !PickupItemID.IsNone()) {
		/*
			If data table is available, get it now, otherwise load it.
		*/
		UDataTable* LoadedDataTable = PickupDataTable.IsValid()
		? PickupDataTable.Get()
		: PickupDataTable.LoadSynchronous();

		if(!LoadedDataTable) {
			return;
		}

		// Use the pickup ID to look up in the table and save the corresponding table row
		const FItemData* ItemDataRow = LoadedDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());
		if(!ItemDataRow) {
			return;
		}

		/*
			If the data asset is available, get it now, otherwise load it.
		*/
		UItemDefinition* TempItemDefinition = ItemDataRow->ItemBase.IsValid()
		? ItemDataRow->ItemBase.Get()
		: ItemDataRow->ItemBase.LoadSynchronous();

		if(!TempItemDefinition) {
			return;
		}

		ReferenceItem = NewObject<UItemDefinition>(this);
		ReferenceItem->ID = ItemDataRow->ID;
		ReferenceItem->ItemType = ItemDataRow->ItemType;
		ReferenceItem->ItemText = ItemDataRow->ItemText;
		ReferenceItem->WorldMesh = TempItemDefinition->WorldMesh;

		UStaticMesh* LoadedMesh = TempItemDefinition->WorldMesh.IsValid()
		? TempItemDefinition->WorldMesh.Get()
		: TempItemDefinition->WorldMesh.LoadSynchronous();

		if(LoadedMesh) {
			// Set the pickup's mesh
			PickupMeshComponent->SetStaticMesh(LoadedMesh);
		}
	}
}

