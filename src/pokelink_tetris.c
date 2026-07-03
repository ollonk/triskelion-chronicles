#include "global.h"
#include "bg.h"
#include "gpu_regs.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "random.h"
#include "save.h"
#include "scanline_effect.h"
#include "sound.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"

enum
{
    WIN_TETRIS_FIELD,
    WIN_TETRIS_PANEL,
};

enum
{
    TETRIS_STATE_FADE_IN,
    TETRIS_STATE_MODE_SELECT,
    TETRIS_STATE_PLAY,
    TETRIS_STATE_EXIT,
};

enum
{
    TETRIS_MODE_CLASSIC,
    TETRIS_MODE_BLAST,
};

#define TETRIS_W 10
#define TETRIS_H 20
#define TETRIS_CELL 6
#define TETRIS_FIELD_X 17
#define TETRIS_FIELD_Y 15
#define TETRIS_SCORE_MAX 999999
#define BLAST_W 8
#define BLAST_H 8
#define BLAST_CELL 11
#define BLAST_FIELD_X 7
#define BLAST_FIELD_Y 29
#define BLAST_OFFER_COUNT 3
#define BLAST_PIECE_GRID 5

struct PokeLinkTetris
{
    u8 board[TETRIS_H][TETRIS_W];
    u8 mode;
    u8 modeCursor;
    u8 piece;
    u8 next;
    u8 rot;
    s8 x;
    s8 y;
    u8 fallTimer;
    u8 fallDelay;
    u8 level;
    u16 lines;
    u32 score;
    bool8 paused;
    bool8 over;
    bool8 redraw;
    bool8 highDirty;
    u8 blastOffers[BLAST_OFFER_COUNT];
    bool8 blastOfferUsed[BLAST_OFFER_COUNT];
    u8 blastSelected;
    u8 blastCombo;
};

static void CB2_PokeLinkTetris(void);
static void VBlankCB_PokeLinkTetris(void);
static void Task_PokeLinkTetris(u8 taskId);
static void DrawModeSelect(void);
static void StartTetrisMode(u8 mode);
static void InitTetrisGame(void);
static void SpawnTetrisPiece(void);
static bool8 TetrisCollision(s8 x, s8 y, u8 piece, u8 rot);
static bool8 MoveTetrisPiece(s8 dx, s8 dy);
static void RotateTetrisPiece(s8 dir);
static void HardDropTetrisPiece(void);
static void LockTetrisPiece(void);
static u8 ClearTetrisLines(void);
static void AddTetrisLineScore(u8 count);
static void SetTetrisFallDelay(void);
static void DrawTetris(void);
static void DrawTetrisField(void);
static void DrawTetrisCell(u8 windowId, u8 x, u8 y, u8 color);
static void DrawTetrisPanel(void);
static void DrawTetrisNumber(u8 y, const u8 *label, u32 value, u8 digits);
static void DrawTetrisPreview(void);
static void InitBlastGame(void);
static void GenerateBlastOffers(void);
static bool8 BlastCanPlace(u8 offer, u8 x, u8 y);
static bool8 BlastAnyPieceFits(void);
static bool8 BlastPlaceSelected(void);
static u8 BlastClearLines(void);
static void BlastSelectNext(void);
static void DrawBlast(void);
static void DrawBlastField(void);
static void DrawBlastCell(u8 x, u8 y, u8 color);
static void DrawBlastPanel(void);
static void DrawBlastPreview(u8 offer, u8 x, u8 y);
static u8 BlastPieceCellCount(u8 piece);
static u8 BlastPieceColor(u8 piece);
static void AddScore(u32 points);
static void CleanupTetris(void);
static u8 RandomTetrisPiece(void);
static u16 TetrisMask(u8 piece, u8 rot);
static u8 TetrisColor(u8 piece);
static void SaveTetrisHighScoreIfNeeded(void);

static const u16 sTetrisPal[] =
{
    RGB_BLACK,
    RGB_WHITE,
    RGB(6, 6, 7),
    RGB(18, 19, 19),
    RGB(31, 6, 5),
    RGB(31, 17, 2),
    RGB(31, 29, 4),
    RGB(6, 26, 7),
    RGB(5, 25, 31),
    RGB(5, 9, 31),
    RGB(22, 7, 30),
    RGB(22, 22, 24),
    RGB(10, 10, 11),
    RGB(27, 27, 25),
    RGB(14, 14, 16),
    RGB_BLACK,
};

static const struct BgTemplate sTetrisBgTemplates[] =
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

static const struct WindowTemplate sTetrisWindowTemplates[] =
{
    [WIN_TETRIS_FIELD] =
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 13,
        .height = 20,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [WIN_TETRIS_PANEL] =
    {
        .bg = 0,
        .tilemapLeft = 13,
        .tilemapTop = 0,
        .width = 17,
        .height = 20,
        .paletteNum = 15,
        .baseBlock = 261
    },
    DUMMY_WIN_TEMPLATE
};

static const u16 sPieceMasks[7][4] =
{
    {0x00F0, 0x4444, 0x0F00, 0x2222},
    {0x0071, 0x0226, 0x0470, 0x0322},
    {0x0074, 0x0622, 0x0170, 0x0223},
    {0x0660, 0x0660, 0x0660, 0x0660},
    {0x0036, 0x0462, 0x0036, 0x0462},
    {0x0063, 0x0264, 0x0063, 0x0264},
    {0x0072, 0x0262, 0x0270, 0x0232},
};

static const u32 sBlastPieceMasks[] =
{
    0x000001, // single
    0x000003, // 2 horizontal
    0x000007, // 3 horizontal
    0x00000F, // 4 horizontal
    0x000021, // 2 vertical
    0x000421, // 3 vertical
    0x008421, // 4 vertical
    0x000063, // 2x2
    0x001CE7, // 3x3
    0x000423, // small L
    0x0000E2, // small T
    0x0000C6, // small S
    0x000086, // small Z
    0x0010843, // tall L
    0x000087, // long L
    0x0008E2, // plus
};

static const u8 sBlastPieceWidths[] =
{
    1, 2, 3, 4, 1, 1, 1, 2, 3, 2, 3, 3, 3, 2, 3, 3
};

static const u8 sBlastPieceHeights[] =
{
    1, 1, 1, 1, 2, 3, 4, 2, 3, 3, 2, 2, 2, 4, 2, 3
};

static const u8 sPieceColors[7] = {8, 9, 5, 6, 7, 4, 10};
static const u8 sBlastPieceColors[] = {9, 8, 10, 11, 12, 13, 14, 6, 5, 7, 9, 10, 8, 12, 11, 13};
static const u8 sFallDelays[] = {48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5};
static const u8 sTextColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};
static const u8 sText_Tetris[] = _("TETRIS");
static const u8 sText_SelectMode[] = _("SELECT MODE");
static const u8 sText_Classic[] = _("CLASSIC");
static const u8 sText_Blast[] = _("BLAST");
static const u8 sText_Next[] = _("NEXT");
static const u8 sText_Pieces[] = _("PIECES");
static const u8 sText_Score[] = _("SCORE");
static const u8 sText_High[] = _("HIGH");
static const u8 sText_Level[] = _("LEVEL");
static const u8 sText_Lines[] = _("LINES");
static const u8 sText_RowsCols[] = _("LINES");
static const u8 sText_Help1[] = _("DPAD MOVE  A/B ROT");
static const u8 sText_Help2[] = _("UP DROP  START/SEL");
static const u8 sText_BlastHelp1[] = _("DPAD MOVE A SET B NEXT");
static const u8 sText_Paused[] = _("PAUSED");
static const u8 sText_GameOver[] = _("GAME OVER");
static const u8 sText_Again[] = _("A AGAIN");
static const u8 sText_Exit[] = _("SEL EXIT");

static EWRAM_DATA struct PokeLinkTetris *sTetris = NULL;

void CB2_InitPokeLinkTetris(void)
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
        InitBgsFromTemplates(0, sTetrisBgTemplates, ARRAY_COUNT(sTetrisBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        InitWindows(sTetrisWindowTemplates);
        FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 32, 32);
        CopyBgTilemapBufferToVram(0);
        DeactivateAllTextPrinters();
        ResetPaletteFade();
        ResetTasks();
        ResetSpriteData();
        FreeAllSpritePalettes();
        ScanlineEffect_Stop();
        LoadPalette(sTetrisPal, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        gMain.state++;
        break;
    case 1:
        sTetris = AllocZeroed(sizeof(*sTetris));
        if (sTetris == NULL)
        {
            SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
            return;
        }
        sTetris->modeCursor = TETRIS_MODE_CLASSIC;
        DrawModeSelect();
        ShowBg(0);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_PokeLinkTetris);
        CreateTask(Task_PokeLinkTetris, 0);
        SetMainCallback2(CB2_PokeLinkTetris);
        gMain.state = 0;
        break;
    }
}

static void CB2_PokeLinkTetris(void)
{
    RunTasks();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_PokeLinkTetris(void)
{
    TransferPlttBuffer();
}

#define tState data[0]

static void Task_PokeLinkTetris(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case TETRIS_STATE_FADE_IN:
        if (!gPaletteFade.active)
            tState = TETRIS_STATE_MODE_SELECT;
        break;
    case TETRIS_STATE_MODE_SELECT:
        if (JOY_NEW(DPAD_UP | DPAD_DOWN))
        {
            PlaySE(SE_SELECT);
            sTetris->modeCursor ^= 1;
            DrawModeSelect();
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            StartTetrisMode(sTetris->modeCursor);
            tState = TETRIS_STATE_PLAY;
        }
        else if (JOY_NEW(B_BUTTON | SELECT_BUTTON))
        {
            PlaySE(SE_SELECT);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            tState = TETRIS_STATE_EXIT;
        }
        break;
    case TETRIS_STATE_PLAY:
        if (JOY_NEW(SELECT_BUTTON))
        {
            PlaySE(SE_SELECT);
            SaveTetrisHighScoreIfNeeded();
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            tState = TETRIS_STATE_EXIT;
            break;
        }
        if (sTetris->over)
        {
            if (JOY_NEW(A_BUTTON))
            {
                PlaySE(SE_SELECT);
                StartTetrisMode(sTetris->mode);
            }
            break;
        }
        if (JOY_NEW(START_BUTTON))
        {
            if (sTetris->mode == TETRIS_MODE_CLASSIC)
            {
                PlaySE(SE_SELECT);
                sTetris->paused ^= TRUE;
                sTetris->redraw = TRUE;
            }
            else
            {
                PlaySE(SE_SELECT);
                StartTetrisMode(TETRIS_MODE_BLAST);
            }
        }
        if (sTetris->mode == TETRIS_MODE_BLAST)
        {
            if (JOY_REPEAT(DPAD_LEFT) && sTetris->x > 0)
            {
                sTetris->x--;
                sTetris->redraw = TRUE;
            }
            if (JOY_REPEAT(DPAD_RIGHT) && sTetris->x < BLAST_W - 1)
            {
                sTetris->x++;
                sTetris->redraw = TRUE;
            }
            if (JOY_REPEAT(DPAD_UP) && sTetris->y > 0)
            {
                sTetris->y--;
                sTetris->redraw = TRUE;
            }
            if (JOY_REPEAT(DPAD_DOWN) && sTetris->y < BLAST_H - 1)
            {
                sTetris->y++;
                sTetris->redraw = TRUE;
            }
            if (JOY_NEW(B_BUTTON))
                BlastSelectNext();
            if (JOY_NEW(A_BUTTON) && BlastPlaceSelected())
            {
                if (sTetris->score > gSaveBlock3Ptr->pokeLinkBlastHighScore)
                {
                    gSaveBlock3Ptr->pokeLinkBlastHighScore = sTetris->score;
                    sTetris->highDirty = TRUE;
                }
            }
        }
        else if (!sTetris->paused)
        {
            if (JOY_REPEAT(DPAD_LEFT))
                MoveTetrisPiece(-1, 0);
            if (JOY_REPEAT(DPAD_RIGHT))
                MoveTetrisPiece(1, 0);
            if (JOY_NEW(A_BUTTON))
                RotateTetrisPiece(1);
            if (JOY_NEW(B_BUTTON))
                RotateTetrisPiece(-1);
            if (JOY_NEW(DPAD_UP))
                HardDropTetrisPiece();
            else if (JOY_REPEAT(DPAD_DOWN) && MoveTetrisPiece(0, 1))
            {
                if (sTetris->score < TETRIS_SCORE_MAX)
                    sTetris->score++;
            }
            if (++sTetris->fallTimer >= sTetris->fallDelay)
            {
                sTetris->fallTimer = 0;
                if (!MoveTetrisPiece(0, 1))
                    LockTetrisPiece();
            }
        }
        if (sTetris->redraw)
        {
            if (sTetris->mode == TETRIS_MODE_BLAST)
                DrawBlast();
            else
                DrawTetris();
        }
        break;
    case TETRIS_STATE_EXIT:
        if (!gPaletteFade.active)
        {
            CleanupTetris();
            DestroyTask(taskId);
            SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        }
        break;
    }
}

#undef tState

static void DrawModeSelect(void)
{
    FillWindowPixelBuffer(WIN_TETRIS_FIELD, PIXEL_FILL(0));
    FillWindowPixelBuffer(WIN_TETRIS_PANEL, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_NORMAL, 18, 42, sTextColors, TEXT_SKIP_DRAW, sText_Tetris);
    AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_NARROW, 8, 66, sTextColors, TEXT_SKIP_DRAW, sText_SelectMode);
    AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_NORMAL, 28, 94, sTextColors, TEXT_SKIP_DRAW, sText_Classic);
    AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_NORMAL, 34, 118, sTextColors, TEXT_SKIP_DRAW, sText_Blast);
    AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_NORMAL, 14, sTetris->modeCursor == TETRIS_MODE_CLASSIC ? 94 : 118, sTextColors, TEXT_SKIP_DRAW, gText_SelectorArrow);
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_NARROW, 4, 56, sTextColors, TEXT_SKIP_DRAW, sText_Classic);
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_SMALL, 4, 76, sTextColors, TEXT_SKIP_DRAW, sText_Help1);
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_NARROW, 4, 106, sTextColors, TEXT_SKIP_DRAW, sText_Blast);
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_SMALL, 4, 126, sTextColors, TEXT_SKIP_DRAW, sText_BlastHelp1);
    PutWindowTilemap(WIN_TETRIS_FIELD);
    PutWindowTilemap(WIN_TETRIS_PANEL);
    CopyWindowToVram(WIN_TETRIS_FIELD, COPYWIN_FULL);
    CopyWindowToVram(WIN_TETRIS_PANEL, COPYWIN_FULL);
}

static void StartTetrisMode(u8 mode)
{
    sTetris->mode = mode;
    if (mode == TETRIS_MODE_BLAST)
        InitBlastGame();
    else
        InitTetrisGame();
}

static void InitTetrisGame(void)
{
    memset(sTetris->board, 0, sizeof(sTetris->board));
    sTetris->score = 0;
    sTetris->lines = 0;
    sTetris->level = 0;
    sTetris->paused = FALSE;
    sTetris->over = FALSE;
    sTetris->highDirty = FALSE;
    sTetris->next = RandomTetrisPiece();
    SetTetrisFallDelay();
    SpawnTetrisPiece();
    sTetris->redraw = TRUE;
}

static void SpawnTetrisPiece(void)
{
    sTetris->piece = sTetris->next;
    sTetris->next = RandomTetrisPiece();
    sTetris->rot = 0;
    sTetris->x = 3;
    sTetris->y = 0;
    sTetris->fallTimer = 0;
    if (TetrisCollision(sTetris->x, sTetris->y, sTetris->piece, sTetris->rot))
    {
        sTetris->over = TRUE;
        SaveTetrisHighScoreIfNeeded();
    }
}

static bool8 TetrisCollision(s8 x, s8 y, u8 piece, u8 rot)
{
    u16 mask = TetrisMask(piece, rot);
    u8 row;
    u8 col;
    s8 boardX;
    s8 boardY;

    for (row = 0; row < 4; row++)
    {
        for (col = 0; col < 4; col++)
        {
            if (!(mask & (1 << (row * 4 + col))))
                continue;
            boardX = x + col;
            boardY = y + row;
            if (boardX < 0 || boardX >= TETRIS_W || boardY >= TETRIS_H)
                return TRUE;
            if (boardY >= 0 && sTetris->board[(u8)boardY][(u8)boardX] != 0)
                return TRUE;
        }
    }

    return FALSE;
}

static bool8 MoveTetrisPiece(s8 dx, s8 dy)
{
    if (TetrisCollision(sTetris->x + dx, sTetris->y + dy, sTetris->piece, sTetris->rot))
        return FALSE;

    sTetris->x += dx;
    sTetris->y += dy;
    sTetris->redraw = TRUE;
    return TRUE;
}

static void RotateTetrisPiece(s8 dir)
{
    static const s8 sKicks[] = {0, -1, 1, -2, 2};
    u8 rot = (sTetris->rot + (dir > 0 ? 1 : 3)) & 3;
    u8 i;

    for (i = 0; i < ARRAY_COUNT(sKicks); i++)
    {
        if (!TetrisCollision(sTetris->x + sKicks[i], sTetris->y, sTetris->piece, rot))
        {
            sTetris->x += sKicks[i];
            sTetris->rot = rot;
            sTetris->redraw = TRUE;
            PlaySE(SE_SELECT);
            return;
        }
    }

    PlaySE(SE_FAILURE);
}

static void HardDropTetrisPiece(void)
{
    u8 rows = 0;

    while (MoveTetrisPiece(0, 1))
        rows++;
    sTetris->score += rows * 2;
    if (sTetris->score > TETRIS_SCORE_MAX)
        sTetris->score = TETRIS_SCORE_MAX;
    LockTetrisPiece();
}

static void LockTetrisPiece(void)
{
    u16 mask = TetrisMask(sTetris->piece, sTetris->rot);
    u8 row;
    u8 col;
    s8 boardX;
    s8 boardY;

    for (row = 0; row < 4; row++)
    {
        for (col = 0; col < 4; col++)
        {
            if (!(mask & (1 << (row * 4 + col))))
                continue;
            boardX = sTetris->x + col;
            boardY = sTetris->y + row;
            if (boardX >= 0 && boardX < TETRIS_W && boardY >= 0 && boardY < TETRIS_H)
                sTetris->board[(u8)boardY][(u8)boardX] = TetrisColor(sTetris->piece);
        }
    }

    AddTetrisLineScore(ClearTetrisLines());
    if (sTetris->score > gSaveBlock3Ptr->pokeLinkTetrisHighScore)
    {
        gSaveBlock3Ptr->pokeLinkTetrisHighScore = sTetris->score;
        sTetris->highDirty = TRUE;
    }
    SpawnTetrisPiece();
    sTetris->redraw = TRUE;
}

static u8 ClearTetrisLines(void)
{
    s8 row;
    s8 dst = TETRIS_H - 1;
    u8 col;
    bool8 full;
    u8 cleared = 0;

    for (row = TETRIS_H - 1; row >= 0; row--)
    {
        full = TRUE;
        for (col = 0; col < TETRIS_W; col++)
        {
            if (sTetris->board[(u8)row][col] == 0)
            {
                full = FALSE;
                break;
            }
        }
        if (full)
        {
            cleared++;
        }
        else
        {
            if (dst != row)
                memcpy(sTetris->board[(u8)dst], sTetris->board[(u8)row], TETRIS_W);
            dst--;
        }
    }
    while (dst >= 0)
    {
        memset(sTetris->board[(u8)dst], 0, TETRIS_W);
        dst--;
    }

    return cleared;
}

static void AddTetrisLineScore(u8 count)
{
    static const u16 sLineScores[] = {0, 40, 100, 300, 1200};

    if (count == 0)
        return;

    sTetris->score += sLineScores[count] * (sTetris->level + 1);
    if (sTetris->score > TETRIS_SCORE_MAX)
        sTetris->score = TETRIS_SCORE_MAX;
    sTetris->lines += count;
    if (sTetris->lines / 10 > sTetris->level && sTetris->level < 20)
    {
        sTetris->level = sTetris->lines / 10;
        SetTetrisFallDelay();
    }
    PlaySE(count == 4 ? SE_RG_CARD_FLIP : SE_SELECT);
}

static void SetTetrisFallDelay(void)
{
    if (sTetris->level < ARRAY_COUNT(sFallDelays))
        sTetris->fallDelay = sFallDelays[sTetris->level];
    else
        sTetris->fallDelay = 4;
}

static void DrawTetris(void)
{
    DrawTetrisField();
    DrawTetrisPanel();
    sTetris->redraw = FALSE;
}

static void DrawTetrisField(void)
{
    u8 row;
    u8 col;
    u8 ghostY = sTetris->y;
    u16 mask;
    u8 color;
    u8 x;
    u8 y;

    FillWindowPixelBuffer(WIN_TETRIS_FIELD, PIXEL_FILL(0));
    FillWindowPixelRect(WIN_TETRIS_FIELD, PIXEL_FILL(3), TETRIS_FIELD_X - 4, TETRIS_FIELD_Y - 4, TETRIS_W * TETRIS_CELL + 8, TETRIS_H * TETRIS_CELL + 8);
    FillWindowPixelRect(WIN_TETRIS_FIELD, PIXEL_FILL(12), TETRIS_FIELD_X - 1, TETRIS_FIELD_Y - 1, TETRIS_W * TETRIS_CELL + 2, TETRIS_H * TETRIS_CELL + 2);
    FillWindowPixelRect(WIN_TETRIS_FIELD, PIXEL_FILL(0), TETRIS_FIELD_X, TETRIS_FIELD_Y, TETRIS_W * TETRIS_CELL, TETRIS_H * TETRIS_CELL);

    for (row = 0; row < TETRIS_H; row++)
    {
        for (col = 0; col < TETRIS_W; col++)
        {
            if (sTetris->board[row][col] != 0)
                DrawTetrisCell(WIN_TETRIS_FIELD, col, row, sTetris->board[row][col]);
        }
    }

    if (!sTetris->over)
    {
        while (!TetrisCollision(sTetris->x, ghostY + 1, sTetris->piece, sTetris->rot))
            ghostY++;
        mask = TetrisMask(sTetris->piece, sTetris->rot);
        for (row = 0; row < 4; row++)
        {
            for (col = 0; col < 4; col++)
            {
                if (!(mask & (1 << (row * 4 + col))))
                    continue;
                x = sTetris->x + col;
                y = ghostY + row;
                if (x < TETRIS_W && y < TETRIS_H && sTetris->board[y][x] == 0)
                    DrawTetrisCell(WIN_TETRIS_FIELD, x, y, 14);
            }
        }
        color = TetrisColor(sTetris->piece);
        for (row = 0; row < 4; row++)
        {
            for (col = 0; col < 4; col++)
            {
                if (!(mask & (1 << (row * 4 + col))))
                    continue;
                x = sTetris->x + col;
                y = sTetris->y + row;
                if (x < TETRIS_W && y < TETRIS_H)
                    DrawTetrisCell(WIN_TETRIS_FIELD, x, y, color);
            }
        }
    }

    if (sTetris->paused)
        AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_NORMAL, 31, 70, sTextColors, TEXT_SKIP_DRAW, sText_Paused);
    if (sTetris->over)
    {
        AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_NORMAL, 22, 62, sTextColors, TEXT_SKIP_DRAW, sText_GameOver);
        AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_SMALL, 32, 78, sTextColors, TEXT_SKIP_DRAW, sText_Again);
        AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_SMALL, 30, 88, sTextColors, TEXT_SKIP_DRAW, sText_Exit);
    }

    PutWindowTilemap(WIN_TETRIS_FIELD);
    CopyWindowToVram(WIN_TETRIS_FIELD, COPYWIN_FULL);
}

static void DrawTetrisCell(u8 windowId, u8 x, u8 y, u8 color)
{
    u8 px = TETRIS_FIELD_X + x * TETRIS_CELL;
    u8 py = TETRIS_FIELD_Y + y * TETRIS_CELL;

    FillWindowPixelRect(windowId, PIXEL_FILL(color), px, py, TETRIS_CELL - 1, TETRIS_CELL - 1);
    FillWindowPixelRect(windowId, PIXEL_FILL(13), px + 1, py + 1, TETRIS_CELL - 3, 1);
}

static void DrawTetrisPanel(void)
{
    FillWindowPixelBuffer(WIN_TETRIS_PANEL, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_NORMAL, 30, 6, sTextColors, TEXT_SKIP_DRAW, sText_Tetris);
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_NORMAL, 8, 27, sTextColors, TEXT_SKIP_DRAW, sText_Next);
    DrawTetrisPreview();
    DrawTetrisNumber(60, sText_Score, sTetris->score, 6);
    DrawTetrisNumber(82, sText_High, gSaveBlock3Ptr->pokeLinkTetrisHighScore, 6);
    DrawTetrisNumber(104, sText_Level, sTetris->level, 2);
    DrawTetrisNumber(126, sText_Lines, sTetris->lines, 3);
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_SMALL, 0, 138, sTextColors, TEXT_SKIP_DRAW, sText_Help1);
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_SMALL, 0, 148, sTextColors, TEXT_SKIP_DRAW, sText_Help2);
    PutWindowTilemap(WIN_TETRIS_PANEL);
    CopyWindowToVram(WIN_TETRIS_PANEL, COPYWIN_FULL);
}

static void DrawTetrisNumber(u8 y, const u8 *label, u32 value, u8 digits)
{
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_NARROW, 8, y, sTextColors, TEXT_SKIP_DRAW, label);
    ConvertIntToDecimalStringN(gStringVar1, value, STR_CONV_MODE_RIGHT_ALIGN, digits);
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_NORMAL, 66, y - 2, sTextColors, TEXT_SKIP_DRAW, gStringVar1);
}

static void DrawTetrisPreview(void)
{
    u16 mask = TetrisMask(sTetris->next, 0);
    u8 row;
    u8 col;
    u8 x;
    u8 y;

    FillWindowPixelRect(WIN_TETRIS_PANEL, PIXEL_FILL(12), 64, 24, 38, 34);
    FillWindowPixelRect(WIN_TETRIS_PANEL, PIXEL_FILL(0), 66, 26, 34, 30);
    for (row = 0; row < 4; row++)
    {
        for (col = 0; col < 4; col++)
        {
            if (!(mask & (1 << (row * 4 + col))))
                continue;
            x = 70 + col * 6;
            y = 31 + row * 6;
            FillWindowPixelRect(WIN_TETRIS_PANEL, PIXEL_FILL(TetrisColor(sTetris->next)), x, y, 5, 5);
            FillWindowPixelRect(WIN_TETRIS_PANEL, PIXEL_FILL(13), x + 1, y + 1, 3, 1);
        }
    }
}

static void InitBlastGame(void)
{
    memset(sTetris->board, 0, sizeof(sTetris->board));
    sTetris->score = 0;
    sTetris->lines = 0;
    sTetris->level = 0;
    sTetris->paused = FALSE;
    sTetris->over = FALSE;
    sTetris->highDirty = FALSE;
    sTetris->blastCombo = 0;
    sTetris->x = 0;
    sTetris->y = 0;
    GenerateBlastOffers();
    sTetris->redraw = TRUE;
}

static void GenerateBlastOffers(void)
{
    u8 i;

    for (i = 0; i < BLAST_OFFER_COUNT; i++)
    {
        sTetris->blastOffers[i] = Random() % ARRAY_COUNT(sBlastPieceMasks);
        sTetris->blastOfferUsed[i] = FALSE;
    }
    sTetris->blastSelected = 0;
    if (!BlastAnyPieceFits())
    {
        sTetris->over = TRUE;
        SaveTetrisHighScoreIfNeeded();
    }
}

static bool8 BlastCanPlace(u8 offer, u8 x, u8 y)
{
    u8 piece;
    u32 mask;
    u8 row;
    u8 col;

    if (offer >= BLAST_OFFER_COUNT || sTetris->blastOfferUsed[offer])
        return FALSE;

    piece = sTetris->blastOffers[offer];
    if (x + sBlastPieceWidths[piece] > BLAST_W || y + sBlastPieceHeights[piece] > BLAST_H)
        return FALSE;

    mask = sBlastPieceMasks[piece];
    for (row = 0; row < BLAST_PIECE_GRID; row++)
    {
        for (col = 0; col < BLAST_PIECE_GRID; col++)
        {
            if ((mask & (1 << (row * BLAST_PIECE_GRID + col))) && sTetris->board[y + row][x + col] != 0)
                return FALSE;
        }
    }

    return TRUE;
}

static bool8 BlastAnyPieceFits(void)
{
    u8 offer;
    u8 row;
    u8 col;

    for (offer = 0; offer < BLAST_OFFER_COUNT; offer++)
    {
        if (sTetris->blastOfferUsed[offer])
            continue;
        for (row = 0; row < BLAST_H; row++)
        {
            for (col = 0; col < BLAST_W; col++)
            {
                if (BlastCanPlace(offer, col, row))
                    return TRUE;
            }
        }
    }

    return FALSE;
}

static bool8 BlastPlaceSelected(void)
{
    u8 piece;
    u32 mask;
    u8 row;
    u8 col;
    u8 lines;

    if (!BlastCanPlace(sTetris->blastSelected, sTetris->x, sTetris->y))
    {
        PlaySE(SE_FAILURE);
        return FALSE;
    }

    piece = sTetris->blastOffers[sTetris->blastSelected];
    mask = sBlastPieceMasks[piece];
    for (row = 0; row < BLAST_PIECE_GRID; row++)
    {
        for (col = 0; col < BLAST_PIECE_GRID; col++)
        {
            if (mask & (1 << (row * BLAST_PIECE_GRID + col)))
                sTetris->board[sTetris->y + row][sTetris->x + col] = BlastPieceColor(piece);
        }
    }

    AddScore(BlastPieceCellCount(piece));
    sTetris->blastOfferUsed[sTetris->blastSelected] = TRUE;
    lines = BlastClearLines();
    if (lines != 0)
    {
        sTetris->blastCombo++;
        sTetris->lines += lines;
        AddScore(lines * 100 + sTetris->blastCombo * 25);
        PlaySE(lines > 1 ? SE_RG_CARD_FLIP : SE_SELECT);
    }
    else
    {
        sTetris->blastCombo = 0;
        PlaySE(SE_SELECT);
    }

    if (sTetris->blastOfferUsed[0] && sTetris->blastOfferUsed[1] && sTetris->blastOfferUsed[2])
    {
        GenerateBlastOffers();
    }
    else
    {
        if (sTetris->blastOfferUsed[sTetris->blastSelected])
            BlastSelectNext();
        if (!BlastAnyPieceFits())
        {
            sTetris->over = TRUE;
            SaveTetrisHighScoreIfNeeded();
        }
    }

    sTetris->redraw = TRUE;
    return TRUE;
}

static u8 BlastClearLines(void)
{
    bool8 clearRows[BLAST_H] = {FALSE};
    bool8 clearCols[BLAST_W] = {FALSE};
    u8 row;
    u8 col;
    u8 lines = 0;
    bool8 full;

    for (row = 0; row < BLAST_H; row++)
    {
        full = TRUE;
        for (col = 0; col < BLAST_W; col++)
        {
            if (sTetris->board[row][col] == 0)
            {
                full = FALSE;
                break;
            }
        }
        if (full)
        {
            clearRows[row] = TRUE;
            lines++;
        }
    }

    for (col = 0; col < BLAST_W; col++)
    {
        full = TRUE;
        for (row = 0; row < BLAST_H; row++)
        {
            if (sTetris->board[row][col] == 0)
            {
                full = FALSE;
                break;
            }
        }
        if (full)
        {
            clearCols[col] = TRUE;
            lines++;
        }
    }

    if (lines != 0)
    {
        for (row = 0; row < BLAST_H; row++)
        {
            for (col = 0; col < BLAST_W; col++)
            {
                if (clearRows[row] || clearCols[col])
                    sTetris->board[row][col] = 0;
            }
        }
    }

    return lines;
}

static void BlastSelectNext(void)
{
    u8 i;

    for (i = 0; i < BLAST_OFFER_COUNT; i++)
    {
        sTetris->blastSelected++;
        if (sTetris->blastSelected >= BLAST_OFFER_COUNT)
            sTetris->blastSelected = 0;
        if (!sTetris->blastOfferUsed[sTetris->blastSelected])
        {
            PlaySE(SE_SELECT);
            sTetris->redraw = TRUE;
            return;
        }
    }
}

static void DrawBlast(void)
{
    DrawBlastField();
    DrawBlastPanel();
    sTetris->redraw = FALSE;
}

static void DrawBlastField(void)
{
    u8 row;
    u8 col;
    u8 piece;
    u32 mask;
    bool8 canPlace;

    FillWindowPixelBuffer(WIN_TETRIS_FIELD, PIXEL_FILL(0));
    FillWindowPixelRect(WIN_TETRIS_FIELD, PIXEL_FILL(3), BLAST_FIELD_X - 4, BLAST_FIELD_Y - 4, BLAST_W * BLAST_CELL + 8, BLAST_H * BLAST_CELL + 8);
    FillWindowPixelRect(WIN_TETRIS_FIELD, PIXEL_FILL(12), BLAST_FIELD_X - 1, BLAST_FIELD_Y - 1, BLAST_W * BLAST_CELL + 2, BLAST_H * BLAST_CELL + 2);
    FillWindowPixelRect(WIN_TETRIS_FIELD, PIXEL_FILL(0), BLAST_FIELD_X, BLAST_FIELD_Y, BLAST_W * BLAST_CELL, BLAST_H * BLAST_CELL);

    for (row = 0; row < BLAST_H; row++)
    {
        for (col = 0; col < BLAST_W; col++)
            DrawBlastCell(col, row, sTetris->board[row][col] == 0 ? 2 : sTetris->board[row][col]);
    }

    if (!sTetris->over)
    {
        piece = sTetris->blastOffers[sTetris->blastSelected];
        mask = sBlastPieceMasks[piece];
        canPlace = BlastCanPlace(sTetris->blastSelected, sTetris->x, sTetris->y);
        for (row = 0; row < BLAST_PIECE_GRID; row++)
        {
            for (col = 0; col < BLAST_PIECE_GRID; col++)
            {
                if (mask & (1 << (row * BLAST_PIECE_GRID + col)))
                    DrawBlastCell(sTetris->x + col, sTetris->y + row, canPlace ? BlastPieceColor(piece) : 8);
            }
        }
    }

    if (sTetris->over)
    {
        AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_NORMAL, 22, 62, sTextColors, TEXT_SKIP_DRAW, sText_GameOver);
        AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_SMALL, 32, 78, sTextColors, TEXT_SKIP_DRAW, sText_Again);
        AddTextPrinterParameterized3(WIN_TETRIS_FIELD, FONT_SMALL, 30, 88, sTextColors, TEXT_SKIP_DRAW, sText_Exit);
    }

    PutWindowTilemap(WIN_TETRIS_FIELD);
    CopyWindowToVram(WIN_TETRIS_FIELD, COPYWIN_FULL);
}

static void DrawBlastCell(u8 x, u8 y, u8 color)
{
    u8 px;
    u8 py;

    if (x >= BLAST_W || y >= BLAST_H)
        return;

    px = BLAST_FIELD_X + x * BLAST_CELL;
    py = BLAST_FIELD_Y + y * BLAST_CELL;
    FillWindowPixelRect(WIN_TETRIS_FIELD, PIXEL_FILL(color), px, py, BLAST_CELL - 1, BLAST_CELL - 1);
    FillWindowPixelRect(WIN_TETRIS_FIELD, PIXEL_FILL(13), px + 1, py + 1, BLAST_CELL - 3, 1);
}

static void DrawBlastPanel(void)
{
    FillWindowPixelBuffer(WIN_TETRIS_PANEL, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_NORMAL, 34, 6, sTextColors, TEXT_SKIP_DRAW, sText_Blast);
    DrawTetrisNumber(32, sText_Score, sTetris->score, 6);
    DrawTetrisNumber(54, sText_High, gSaveBlock3Ptr->pokeLinkBlastHighScore, 6);
    DrawTetrisNumber(76, sText_RowsCols, sTetris->lines, 3);
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_NARROW, 8, 96, sTextColors, TEXT_SKIP_DRAW, sText_Pieces);
    DrawBlastPreview(0, 8, 114);
    DrawBlastPreview(1, 50, 114);
    DrawBlastPreview(2, 92, 114);
    AddTextPrinterParameterized3(WIN_TETRIS_PANEL, FONT_SMALL, 0, 146, sTextColors, TEXT_SKIP_DRAW, sText_BlastHelp1);
    PutWindowTilemap(WIN_TETRIS_PANEL);
    CopyWindowToVram(WIN_TETRIS_PANEL, COPYWIN_FULL);
}

static void DrawBlastPreview(u8 offer, u8 x, u8 y)
{
    u8 piece = sTetris->blastOffers[offer];
    u32 mask = sBlastPieceMasks[piece];
    u8 row;
    u8 col;
    u8 color = sTetris->blastOfferUsed[offer] ? 3 : BlastPieceColor(piece);

    if (offer == sTetris->blastSelected && !sTetris->blastOfferUsed[offer])
        FillWindowPixelRect(WIN_TETRIS_PANEL, PIXEL_FILL(5), x - 3, y - 3, 31, 31);
    FillWindowPixelRect(WIN_TETRIS_PANEL, PIXEL_FILL(0), x - 1, y - 1, 27, 27);
    for (row = 0; row < BLAST_PIECE_GRID; row++)
    {
        for (col = 0; col < BLAST_PIECE_GRID; col++)
        {
            if (mask & (1 << (row * BLAST_PIECE_GRID + col)))
            {
                FillWindowPixelRect(WIN_TETRIS_PANEL, PIXEL_FILL(color), x + col * 5, y + row * 5, 4, 4);
                FillWindowPixelRect(WIN_TETRIS_PANEL, PIXEL_FILL(13), x + col * 5 + 1, y + row * 5 + 1, 2, 1);
            }
        }
    }
}

static u8 BlastPieceCellCount(u8 piece)
{
    u32 mask = sBlastPieceMasks[piece];
    u8 i;
    u8 count = 0;

    for (i = 0; i < BLAST_PIECE_GRID * BLAST_PIECE_GRID; i++)
    {
        if (mask & (1 << i))
            count++;
    }

    return count;
}

static u8 BlastPieceColor(u8 piece)
{
    return sBlastPieceColors[piece % ARRAY_COUNT(sBlastPieceColors)];
}

static void AddScore(u32 points)
{
    sTetris->score += points;
    if (sTetris->score > TETRIS_SCORE_MAX)
        sTetris->score = TETRIS_SCORE_MAX;
}

static void CleanupTetris(void)
{
    SetVBlankCallback(NULL);
    FreeAllWindowBuffers();
    FREE_AND_SET_NULL(sTetris);
}

static u8 RandomTetrisPiece(void)
{
    return Random() % 7;
}

static u16 TetrisMask(u8 piece, u8 rot)
{
    return sPieceMasks[piece][rot & 3];
}

static u8 TetrisColor(u8 piece)
{
    return sPieceColors[piece];
}

static void SaveTetrisHighScoreIfNeeded(void)
{
    u32 *highScore = sTetris->mode == TETRIS_MODE_BLAST
        ? &gSaveBlock3Ptr->pokeLinkBlastHighScore
        : &gSaveBlock3Ptr->pokeLinkTetrisHighScore;

    if (sTetris->score > *highScore)
    {
        *highScore = sTetris->score;
        sTetris->highDirty = TRUE;
    }
    if (sTetris->highDirty)
    {
        TrySavingData(SAVE_NORMAL);
        sTetris->highDirty = FALSE;
    }
}
