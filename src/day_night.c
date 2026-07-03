#include "global.h"
#include "clock.h"
#include "day_night.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "overworld.h"
#include "palette.h"
#include "rtc.h"
#include "script.h"
#include "constants/map_types.h"
#include "constants/rgb.h"

EWRAM_DATA static u8 sLastAppliedTimeOfDay = 0;
EWRAM_DATA static bool8 sLastAppliedTimeOfDayValid = FALSE;

static bool8 DayNight_IsTintEnabledForCurrentMap(void)
{
    switch (gMapHeader.mapType)
    {
    case MAP_TYPE_TOWN:
    case MAP_TYPE_CITY:
    case MAP_TYPE_ROUTE:
    case MAP_TYPE_OCEAN_ROUTE:
    case MAP_TYPE_OUTDOOR_DUNGEON:
        return OW_ENABLE_DNS;
    default:
        return FALSE;
    }
}

static u16 DayNight_ApplyTone(u16 color, u8 timeOfDay)
{
    u16 r = GET_R(color);
    u16 g = GET_G(color);
    u16 b = GET_B(color);

    switch (timeOfDay)
    {
    case TIME_MORNING:
        r = min(31, (r * 15) / 16 + 1);
        g = min(31, (g * 15) / 16 + 1);
        b = (b * 14) / 16;
        break;
    case TIME_EVENING:
        r = min(31, (r * 14) / 16 + 3);
        g = (g * 12) / 16;
        b = (b * 10) / 16;
        break;
    case TIME_NIGHT:
        r = (r * 8) / 16;
        g = (g * 9) / 16;
        b = min(31, (b * 12) / 16 + 4);
        break;
    case TIME_DAY:
    default:
        break;
    }

    return RGB(r, g, b);
}

void DayNight_TintPaletteEntries(u16 offset, u16 count)
{
    u16 i;
    u8 timeOfDay;

    if (!DayNight_IsTintEnabledForCurrentMap())
        return;

    timeOfDay = GetTimeOfDay();
    if (timeOfDay == TIME_DAY)
        return;

    for (i = 0; i < count; i++)
    {
        u16 palOffset = offset + i;
        u16 color = DayNight_ApplyTone(gPlttBufferUnfaded[palOffset], timeOfDay);

        gPlttBufferUnfaded[palOffset] = color;
        gPlttBufferFaded[palOffset] = color;
    }
}

void DayNight_UpdateOverworldPaletteTint(bool8 force)
{
    u8 i;
    u8 timeOfDay = GetTimeOfDay();

    if (!force && sLastAppliedTimeOfDayValid && sLastAppliedTimeOfDay == timeOfDay)
        return;

    sLastAppliedTimeOfDay = timeOfDay;
    sLastAppliedTimeOfDayValid = TRUE;

    if (gMapHeader.mapLayout == NULL)
        return;

    LoadMapTilesetPalettes(gMapHeader.mapLayout);

    for (i = 0; i < NUM_PALS_TOTAL; i++)
        ApplyWeatherColorMapToPal(i);
}

void DayNight_AdvanceToNextDayNightPeriod(void)
{
    u16 currentMinutes;
    u16 targetMinutes;
    u16 minutesToAdvance;

    RtcCalcLocalTime();
    currentMinutes = gLocalTime.hours * MINUTES_PER_HOUR + gLocalTime.minutes;
    if (GetTimeOfDay() == TIME_NIGHT)
        targetMinutes = DAY_HOUR_BEGIN * MINUTES_PER_HOUR;
    else
        targetMinutes = NIGHT_HOUR_BEGIN * MINUTES_PER_HOUR;

    if (targetMinutes <= currentMinutes)
        targetMinutes += HOURS_PER_DAY * MINUTES_PER_HOUR;

    minutesToAdvance = targetMinutes - currentMinutes;
    RtcAdvanceTimeBy(0, minutesToAdvance / MINUTES_PER_HOUR, minutesToAdvance % MINUTES_PER_HOUR, 0);
    DoTimeBasedEvents();
}

void Script_AdvanceTimeForRest(void)
{
    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE | SCREFF_HARDWARE);

    DayNight_AdvanceToNextDayNightPeriod();
    DayNight_UpdateOverworldPaletteTint(TRUE);
}
