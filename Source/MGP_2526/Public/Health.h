// Health.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FYouDied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldBroke);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDamaged);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MGP_2526_API UHealth : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealth();

protected:
    virtual void BeginPlay() override;

public:
    // Tick disabled by default; enable if you need per-frame logic
    // virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Maximum health value (editable in editor) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Stats")
    int MaxHealth = 80;

    /** Current health (visible in editor/runtime) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health|Stats")
    int Health = 80;

    /** Maximum shield value (editable in editor) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Shield")
    int MaxShield = 50;

    /** Current shield (visible in editor/runtime) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health|Shield")
    int Shield = 50;

    /** If true, damage bypasses shield and hits health directly */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Behavior")
    bool bBypassShield = false;

    /** Event broadcast when health reaches zero */
    UPROPERTY(BlueprintAssignable, Category = "Health|Events")
    FYouDied OnDied;

    /** Event broadcast when shield breaks (goes from >0 to 0) */
    UPROPERTY(BlueprintAssignable, Category = "Health|Events")
    FShieldBroke OnShieldBroke;

    /** Apply damage to this component. Damage is applied to shield first unless bBypassShield is true. */
    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(int DamageAmount);

    /** Heal health by amount (does not affect shield). Clamped to MaxHealth. */
    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(int HealAmount);

    /** Restore shield by amount. Clamped to MaxShield. */
    UFUNCTION(BlueprintCallable, Category = "Health")
    void RestoreShield(int ShieldAmount);

    /** Fully restore both health and shield to their max values. */
    UFUNCTION(BlueprintCallable, Category = "Health")
    void RestoreFully();

    /** Set max values and optionally reset current values to the new maxes */
    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetMaxValues(int NewMaxHealth, int NewMaxShield, bool bResetCurrent = false);

    /** Utility getters */
    UFUNCTION(BlueprintPure, Category = "Health")
    FORCEINLINE int GetHealth() const { return Health; }

    UFUNCTION(BlueprintPure, Category = "Health")
    FORCEINLINE int GetShield() const { return Shield; }

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealthPercent() const { return Health; }

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetShieldPercent() const { return Shield; }

    UPROPERTY(BlueprintAssignable, Category = "Health|Events")
    FDamaged OnDamaged;

    //FTimerHandle ShieldRegenDelayHandle;
    //FTimerHandle ShieldRegenTickHandle;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Shield Regen")
    //float ShieldRegenDelay = 5.f;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Shield Regen")
    //float ShieldRegenRate = 0.1f; // how often to regen

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Shield Regen")
    //int ShieldRegenAmount = 1; // how much per tick

    //void StartShieldRegen();
    //void RegenShieldTick();

protected:
    /** Internal helper to clamp values and handle death/shield-break logic */
    void ClampAndBroadcast();
};
