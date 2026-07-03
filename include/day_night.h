#ifndef GUARD_DAY_NIGHT_H
#define GUARD_DAY_NIGHT_H

void DayNight_TintPaletteEntries(u16 offset, u16 count);
void DayNight_UpdateOverworldPaletteTint(bool8 force);
void DayNight_AdvanceToNextDayNightPeriod(void);
void Script_AdvanceTimeForRest(void);

#endif // GUARD_DAY_NIGHT_H
