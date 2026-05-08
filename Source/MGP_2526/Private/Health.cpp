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
    //ignores 0 or invalid damage
    if (DamageAmount <= 0)
    {
        return;
    }

    // Capture previous shield before applying damage
    const int PrevShield = Shield;

    // Track whether shield was >0 before applying damage to detect shield break
    const bool bShieldWasPositive = (PrevShield > 0);

    // Apply to shield first unless bypassing
    if (!bBypassShield && Shield > 0)
    {
        // Damage to shield first (can't exceed current shield)
        int DamageToShield = FMath::Min(Shield, DamageAmount);
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
    //Start regen delay timer
    GetWorld()->GetTimerManager().SetTimer(ShieldRegenDelayHandle, this, &UHealth::StartShieldRegen, DelayToUse, false);

    ClampAndBroadcast(PrevShield);
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

void UHealth::ClampAndBroadcast(int PrevShield)
{
    // Clamp values, ensure it stays in valid ranges
    Health = FMath::Clamp(Health, 0, MaxHealth);
    Shield = FMath::Clamp(Shield, 0, MaxShield);

    // Check if shield broke (shield previously >0 and now 0)
    if (PrevShield > 0 && Shield == 0)
    {
        OnShieldBroke.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("%s: Shield broke"), *GetOwner()->GetName());
    }

    // Check for Death (health reached zero)
    if (Health == 0)
    {
        OnDied.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("%s: Died"), *GetOwner()->GetName());
    }
}

void UHealth::StartShieldRegen()
{
    /// Only regen if shield is not full and not already regenerating
    if (Shield < MaxShield)
    {
        FTimerManager& TM = GetWorld()->GetTimerManager();
        
        //prevent duplicate regen timers
        if (!TM.IsTimerActive(ShieldRegenTickHandle))
        {
            // Broadcast that regen is starting
            OnShieldRegenStarted.Broadcast();

            // Start the tick timer
            TM.SetTimer(ShieldRegenTickHandle, this, &UHealth::RegenShieldTick, ShieldRegenRate, true);
        }
    }
}

void UHealth::RegenShieldTick()
{
    // Add shield and clamp to max
    Shield = FMath::Clamp(Shield + ShieldRegenAmount, 0, MaxShield);

    // Stop when full
    if (Shield >= MaxShield)
    {
        GetWorld()->GetTimerManager().ClearTimer(ShieldRegenTickHandle);
   }
}