// Health.cpp
#include "Health.h"
#include "GameFramework/Actor.h"

UHealth::UHealth()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Ensure current values start at max
    Health = MaxHealth;
    Shield = MaxShield;
}

void UHealth::BeginPlay()
{
    Super::BeginPlay();

    // In case Max values were changed in editor after construction, ensure current values are valid
    Health = FMath::Clamp(Health, 0, MaxHealth);
    Shield = FMath::Clamp(Shield, 0, MaxShield);
}

void UHealth::TakeDamage(int DamageAmount)
{
    if (DamageAmount <= 0)
    {
        return;
    }

    // Track whether shield was >0 before applying damage to detect shield break
    const bool bShieldWasPositive = (Shield > 0);

    if (!bBypassShield && Shield > 0)
    {
        // Apply to shield first
        int32 DamageToShield = FMath::Min(Shield, DamageAmount);
        Shield -= DamageToShield;
        DamageAmount -= DamageToShield;
    }

    // Any remaining damage applies to health
    if (DamageAmount > 0)
    {
        Health -= DamageAmount;
    }
    OnDamaged.Broadcast();

    // Reset regen timer whenever damage is taken
    GetWorld()->GetTimerManager().ClearTimer(ShieldRegenDelayHandle);
    GetWorld()->GetTimerManager().ClearTimer(ShieldRegenTickHandle);

    float DelayToUse = ShieldRegenDelay;

    // If shield was >0 and now is 0, use the longer delay
    if (bShieldWasPositive && Shield == 0)
    {
        DelayToUse = ShieldBreakRegenDelay;
    }
    GetWorld()->GetTimerManager().SetTimer(ShieldRegenDelayHandle, this, &UHealth::StartShieldRegen, DelayToUse, false);

    ClampAndBroadcast();
}

void UHealth::Heal(int HealAmount)
{
    if (HealAmount <= 0)
    {
        return;
    }

    Health += HealAmount;
    Health = FMath::Clamp(Health, 0, MaxHealth);
}

void UHealth::RestoreShield(int ShieldAmount)
{
    if (ShieldAmount <= 0)
    {
        return;
    }

    Shield += ShieldAmount;
    Shield = FMath::Clamp(Shield, 0, MaxShield);
}

void UHealth::RestoreFully()
{
    Health = MaxHealth;
    Shield = MaxShield;
}

void UHealth::SetMaxValues(int NewMaxHealth, int NewMaxShield, bool bResetCurrent)
{
    MaxHealth = FMath::Max(1, NewMaxHealth);
    MaxShield = FMath::Max(0, NewMaxShield);

    if (bResetCurrent)
    {
        Health = MaxHealth;
        Shield = MaxShield;
    }
    else
    {
        Health = FMath::Clamp(Health, 0, MaxHealth);
        Shield = FMath::Clamp(Shield, 0, MaxShield);
    }
}

void UHealth::ClampAndBroadcast()
{
    // Clamp values
    const int PrevShield = Shield;
    Health = FMath::Clamp(Health, 0, MaxHealth);
    Shield = FMath::Clamp(Shield, 0, MaxShield);

    // Shield broke: previously >0 and now == 0
    if (PrevShield > 0 && Shield == 0)
    {
        OnShieldBroke.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("%s: Shield broke"), *GetOwner()->GetName());
    }

    // Died: health reached zero
    if (Health == 0)
    {
        OnDied.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("%s: Died"), *GetOwner()->GetName());
    }
}

void UHealth::StartShieldRegen()
{
    /// Only regen if shield is not full
    if (Shield < MaxShield)
    {
        GetWorld()->GetTimerManager().SetTimer(ShieldRegenTickHandle, this, &UHealth::RegenShieldTick,  ShieldRegenRate, true);
    }
}

void UHealth::RegenShieldTick()
{
    Shield = FMath::Clamp(Shield + ShieldRegenAmount, 0, MaxShield);

    // Stop when full
    if (Shield >= MaxShield)
    {
        GetWorld()->GetTimerManager().ClearTimer(ShieldRegenTickHandle);
   }
}