#include "global.h"
#include "bg.h"
#include "gpu_regs.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"

enum
{
    WIN_RADIO,
};

enum
{
    RADIO_STATE_FADE_IN,
    RADIO_STATE_MAIN,
    RADIO_STATE_EXIT,
};

#define RADIO_STATION_COUNT 5
#define RADIO_MIN_FREQ 40
#define RADIO_MAX_FREQ 160
#define RADIO_TUNE_X 28
#define RADIO_TUNE_Y 42
#define RADIO_TUNE_WIDTH 184
#define RADIO_TUNE_STEP 5

struct PokeLinkRadioState
{
    u8 station;
    u8 frequency;
    u8 waveFrame;
    u16 savedMusic;
    bool8 playing;
    bool8 redraw;
};

struct RadioStation
{
    const u8 *name;
    const u8 *host;
    const u8 *tagline;
    u8 frequency;
    u16 song;
};

static void CB2_PokeLinkRadio(void);
static void VBlankCB_PokeLinkRadio(void);
static void Task_PokeLinkRadio(u8 taskId);
static void DrawRadio(void);
static void DrawRadioShell(void);
static void DrawRadioDial(void);
static void DrawRadioStationList(void);
static void DrawRadioInfo(void);
static void DrawRadioWave(void);
static void TuneRadioBy(s8 amount);
static void SetRadioStation(u8 station);
static u8 GetNearestStation(u8 frequency);
static s16 GetFrequencyNeedleX(u8 frequency);
static void PlayRadioStation(void);
static void StopRadioStation(void);
static void CleanupRadio(void);

static const u16 sRadioPal[] =
{
    RGB_BLACK,
    RGB_WHITE,
    RGB(3, 5, 8),
    RGB(7, 10, 13),
    RGB(14, 17, 17),
    RGB(23, 24, 22),
    RGB(30, 29, 22),
    RGB(27, 23, 12),
    RGB(9, 22, 25),
    RGB(6, 15, 20),
    RGB(21, 6, 6),
    RGB(30, 12, 7),
    RGB(12, 27, 11),
    RGB(5, 18, 8),
    RGB(18, 18, 20),
    RGB_BLACK,
};

static const struct BgTemplate sRadioBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
};

static const struct WindowTemplate sRadioWindowTemplates[] =
{
    [WIN_RADIO] =
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 20,
        .paletteNum = 15,
        .baseBlock = 1
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 sRadioTextColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};
static const u8 sRadioDarkTextColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
static const u8 sText_RadioTitle[] = _("POKéLINK RADIO");
static const u8 sText_RadioLive[] = _("ON AIR");
static const u8 sText_RadioMuted[] = _("TUNED");
static const u8 sText_RadioFreq[] = _("FREQ");
static const u8 sText_RadioSignal[] = _("SIGNAL");
static const u8 sText_RadioHelp[] = _("{DPAD_LEFTRIGHT} Tune  {A_BUTTON} Listen  {B_BUTTON} Back");
static const u8 sText_RadioPokeMusic[] = _("POKéMON MUSIC");
static const u8 sText_RadioOakTalk[] = _("OAK TALK");
static const u8 sText_RadioLucky[] = _("LUCKY CHANNEL");
static const u8 sText_RadioVariety[] = _("VARIETY");
static const u8 sText_RadioTravel[] = _("TRAVEL LOG");
static const u8 sText_HostDJMary[] = _("DJ MARY");
static const u8 sText_HostOak[] = _("PROF.OAK");
static const u8 sText_HostBuena[] = _("BUENA");
static const u8 sText_HostLily[] = _("LILY");
static const u8 sText_HostGuide[] = _("GUIDE GENT");
static const u8 sText_TagMusic[] = _("Songs for walking routes.");
static const u8 sText_TagOak[] = _("Species talk and field tips.");
static const u8 sText_TagLucky[] = _("Numbers, prizes, good luck.");
static const u8 sText_TagVariety[] = _("Calls, gossip, trainer chat.");
static const u8 sText_TagTravel[] = _("Landmarks across the regions.");

static const struct RadioStation sRadioStations[RADIO_STATION_COUNT] =
{
    {sText_RadioPokeMusic, sText_HostDJMary, sText_TagMusic, 45, MUS_ROUTE101},
    {sText_RadioOakTalk,   sText_HostOak,    sText_TagOak,   70, MUS_RG_OAK},
    {sText_RadioLucky,     sText_HostBuena,  sText_TagLucky, 95, MUS_GAME_CORNER},
    {sText_RadioVariety,   sText_HostLily,   sText_TagVariety, 125, MUS_SLATEPORT},
    {sText_RadioTravel,    sText_HostGuide,  sText_TagTravel, 150, MUS_RG_ROUTE1},
};

static EWRAM_DATA struct PokeLinkRadioState *sRadio = NULL;

void CB2_InitPokeLinkRadio(void)
{
    switch (gMain.state)
    {
    case 0:
    default:
        SetVBlankCallback(NULL);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sRadioBgTemplates, ARRAY_COUNT(sRadioBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        InitWindows(sRadioWindowTemplates);
        FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 32, 32);
        CopyBgTilemapBufferToVram(0);
        DeactivateAllTextPrinters();
        ResetPaletteFade();
        ResetTasks();
        ResetSpriteData();
        FreeAllSpritePalettes();
        ScanlineEffect_Stop();
        LoadPalette(sRadioPal, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        gMain.state++;
        break;
    case 1:
        sRadio = AllocZeroed(sizeof(*sRadio));
        if (sRadio == NULL)
        {
            SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
            return;
        }
        sRadio->savedMusic = GetCurrentMapMusic();
        sRadio->station = 0;
        sRadio->frequency = sRadioStations[0].frequency;
        PlayRadioStation();
        DrawRadio();
        ShowBg(0);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_PokeLinkRadio);
        CreateTask(Task_PokeLinkRadio, 0);
        SetMainCallback2(CB2_PokeLinkRadio);
        gMain.state = 0;
        break;
    }
}

static void CB2_PokeLinkRadio(void)
{
    RunTasks();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_PokeLinkRadio(void)
{
    TransferPlttBuffer();
}

#define tState data[0]
#define tTimer data[1]

static void Task_PokeLinkRadio(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case RADIO_STATE_FADE_IN:
        if (!gPaletteFade.active)
            tState = RADIO_STATE_MAIN;
        break;
    case RADIO_STATE_MAIN:
        tTimer++;
        if ((tTimer & 15) == 0)
        {
            sRadio->waveFrame++;
            DrawRadioWave();
        }
        if (JOY_NEW(DPAD_LEFT))
            TuneRadioBy(-RADIO_TUNE_STEP);
        else if (JOY_NEW(DPAD_RIGHT))
            TuneRadioBy(RADIO_TUNE_STEP);
        else if (JOY_NEW(DPAD_UP))
        {
            PlaySE(SE_SELECT);
            SetRadioStation((sRadio->station + RADIO_STATION_COUNT - 1) % RADIO_STATION_COUNT);
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            PlaySE(SE_SELECT);
            SetRadioStation((sRadio->station + 1) % RADIO_STATION_COUNT);
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            PlayRadioStation();
            DrawRadio();
        }
        else if (JOY_NEW(B_BUTTON | SELECT_BUTTON))
        {
            PlaySE(SE_SELECT);
            StopRadioStation();
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            tState = RADIO_STATE_EXIT;
        }

        if (sRadio->redraw)
        {
            sRadio->redraw = FALSE;
            DrawRadio();
        }
        break;
    case RADIO_STATE_EXIT:
        if (!gPaletteFade.active)
        {
            CleanupRadio();
            DestroyTask(taskId);
            SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        }
        break;
    }
}

#undef tState
#undef tTimer

static void DrawRadio(void)
{
    FillWindowPixelBuffer(WIN_RADIO, PIXEL_FILL(2));
    DrawRadioShell();
    DrawRadioDial();
    DrawRadioStationList();
    DrawRadioInfo();
    DrawRadioWave();
    AddTextPrinterParameterized3(WIN_RADIO, FONT_NARROW, 18, 146, sRadioTextColors, TEXT_SKIP_DRAW, sText_RadioHelp);
    PutWindowTilemap(WIN_RADIO);
    CopyWindowToVram(WIN_RADIO, COPYWIN_FULL);
}

static void DrawRadioShell(void)
{
    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(3), 8, 8, 224, 136);
    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(5), 11, 11, 218, 130);
    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(4), 15, 16, 210, 34);
    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(8), 17, 18, 206, 30);
    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(9), 20, 21, 200, 24);
    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(4), 15, 56, 98, 82);
    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(1), 17, 58, 94, 78);
    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(4), 118, 56, 107, 82);
    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(1), 120, 58, 103, 78);
    AddTextPrinterParameterized3(WIN_RADIO, FONT_NORMAL, 17, 10, sRadioTextColors, TEXT_SKIP_DRAW, sText_RadioTitle);
}

static void DrawRadioDial(void)
{
    u8 i;
    s16 x = GetFrequencyNeedleX(sRadio->frequency);

    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(6), RADIO_TUNE_X, RADIO_TUNE_Y, RADIO_TUNE_WIDTH, 3);
    for (i = 0; i < RADIO_STATION_COUNT; i++)
    {
        s16 stationX = GetFrequencyNeedleX(sRadioStations[i].frequency);
        FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(i == sRadio->station ? 11 : 7), stationX - 1, RADIO_TUNE_Y - 5, 3, 13);
    }
    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(10), x - 2, RADIO_TUNE_Y - 8, 5, 18);
    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(1), x - 1, RADIO_TUNE_Y - 6, 3, 14);
}

static void DrawRadioStationList(void)
{
    u8 i;

    for (i = 0; i < RADIO_STATION_COUNT; i++)
    {
        u8 y = 63 + i * 14;
        FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(i == sRadio->station ? 8 : 1), 21, y - 1, 86, 12);
        if (i == sRadio->station)
            AddTextPrinterParameterized3(WIN_RADIO, FONT_NARROW, 23, y, sRadioTextColors, TEXT_SKIP_DRAW, sRadioStations[i].name);
        else
            AddTextPrinterParameterized3(WIN_RADIO, FONT_NARROW, 23, y, sRadioDarkTextColors, TEXT_SKIP_DRAW, sRadioStations[i].name);
    }
}

static void DrawRadioInfo(void)
{
    const struct RadioStation *station = &sRadioStations[sRadio->station];
    u8 x;

    AddTextPrinterParameterized3(WIN_RADIO, FONT_NORMAL, 126, 62, sRadioDarkTextColors, TEXT_SKIP_DRAW, sRadio->playing ? sText_RadioLive : sText_RadioMuted);
    AddTextPrinterParameterized3(WIN_RADIO, FONT_NARROW, 126, 80, sRadioDarkTextColors, TEXT_SKIP_DRAW, station->host);
    AddTextPrinterParameterized3(WIN_RADIO, FONT_NARROW, 126, 95, sRadioDarkTextColors, TEXT_SKIP_DRAW, station->tagline);
    AddTextPrinterParameterized3(WIN_RADIO, FONT_NARROW, 126, 119, sRadioDarkTextColors, TEXT_SKIP_DRAW, sText_RadioFreq);
    ConvertIntToDecimalStringN(gStringVar1, station->frequency, STR_CONV_MODE_RIGHT_ALIGN, 3);
    AddTextPrinterParameterized3(WIN_RADIO, FONT_NARROW, 161, 119, sRadioDarkTextColors, TEXT_SKIP_DRAW, gStringVar1);
    AddTextPrinterParameterized3(WIN_RADIO, FONT_NARROW, 187, 119, sRadioDarkTextColors, TEXT_SKIP_DRAW, sText_RadioSignal);

    for (x = 0; x < 4; x++)
        FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(x <= sRadio->station ? 12 : 14), 198 + x * 5, 129 - x * 3, 3, 4 + x * 3);
}

static void DrawRadioWave(void)
{
    u8 i;

    FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(9), 32, 25, 175, 11);
    for (i = 0; i < 29; i++)
    {
        u8 height = 2 + ((i + sRadio->waveFrame + sRadio->station) & 3) * 2;
        u8 color = (i & 1) ? 6 : 1;
        FillWindowPixelRect(WIN_RADIO, PIXEL_FILL(color), 35 + i * 6, 31 - height / 2, 3, height);
    }
    CopyWindowToVram(WIN_RADIO, COPYWIN_GFX);
}

static void TuneRadioBy(s8 amount)
{
    s16 frequency = sRadio->frequency + amount;
    u8 station;

    if (frequency < RADIO_MIN_FREQ)
        frequency = RADIO_MAX_FREQ;
    else if (frequency > RADIO_MAX_FREQ)
        frequency = RADIO_MIN_FREQ;

    sRadio->frequency = frequency;
    station = GetNearestStation(sRadio->frequency);
    if (station != sRadio->station)
    {
        sRadio->station = station;
        if (sRadio->playing)
            PlayRadioStation();
    }
    PlaySE(SE_SELECT);
    sRadio->redraw = TRUE;
}

static void SetRadioStation(u8 station)
{
    if (station >= RADIO_STATION_COUNT)
        station = 0;

    sRadio->station = station;
    sRadio->frequency = sRadioStations[station].frequency;
    if (sRadio->playing)
        PlayRadioStation();
    sRadio->redraw = TRUE;
}

static u8 GetNearestStation(u8 frequency)
{
    u8 i;
    u8 station = 0;
    u8 bestDistance = 0xFF;

    for (i = 0; i < RADIO_STATION_COUNT; i++)
    {
        u8 stationFreq = sRadioStations[i].frequency;
        u8 distance = frequency > stationFreq ? frequency - stationFreq : stationFreq - frequency;
        if (distance < bestDistance)
        {
            bestDistance = distance;
            station = i;
        }
    }

    return station;
}

static s16 GetFrequencyNeedleX(u8 frequency)
{
    return RADIO_TUNE_X + ((frequency - RADIO_MIN_FREQ) * RADIO_TUNE_WIDTH) / (RADIO_MAX_FREQ - RADIO_MIN_FREQ);
}

static void PlayRadioStation(void)
{
    sRadio->playing = TRUE;
    PlayBGM(sRadioStations[sRadio->station].song);
}

static void StopRadioStation(void)
{
    if (sRadio->playing)
    {
        sRadio->playing = FALSE;
        PlayBGM(sRadio->savedMusic);
    }
}

static void CleanupRadio(void)
{
    SetVBlankCallback(NULL);
    FreeAllWindowBuffers();
    FREE_AND_SET_NULL(sRadio);
}
