/* See LICENSE file for copyright and license details. */

/* Allow use of special keys */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int padbar             = 8;        /* adjusts the bars padding */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "あんずもじ:style=Regular:size=18",
					"Go Mono:style=bold:size=14",
					"Source Han Sans JP:style=Medium:size=14",
					"Source Han Sans CN:style=Medium:size=14",
					"Source Han Sans KR:style=Medium:size=14" };
static const char dmenufont[]       = "Go Mono:style=bold:size=14";
static const char col_gray1[]       = "#090909";
static const char col_gray2[]       = "#1b1b1b";
static const char col_gray3[]       = "#666666";
static const char col_gray4[]       = "#999999";
static const char col_cyan[]        = "#444444";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray1 },
	[SchemeSel]  = { col_gray4, col_gray1, col_cyan },
};
static int fakefullscreen          = 1;

/* tagging */
static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       4,            1,           -1 },
	{ "mgba-sdl", NULL,       NULL,       0,            1,           -1 },
/*	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 }, */
};

/* layout(s) */
static const float mfact     = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

/* HACK: doing this properly would make it hard to merge upstream commits */
#include "layouts.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
};

/* key definitions */
#define MODKEY Mod1Mask
#define WINKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* screenshot output file */
#define SCROT "~/media/pic/screengrabs/$(date -u +%Y年%m月%d日%T).png"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_gray1, "-sf", col_gray4, NULL };
static const char *termcmd[]   = { "st", NULL };
/* static const char *volupcmd[]  = { "amixer", "-q", "set", "Speaker", "2+", NULL };
 * static const char *voldncmd[]  = { "amixer", "-q", "set", "Speaker", "2-", NULL };
 * static const char *volmucmd[]  = { "amixer", "-q", "set", "Speaker", "toggle", NULL };
 */
static const char *volmucmd[]  = { "amixer", "-q", "set", "IEC958", "toggle", NULL };
static const char *webcmd[]    = { "furfox", NULL };
static const char *mpdtog[]    = { "mpdtoggle", NULL };
static const char *mpdnext[]   = { "mpdnext", NULL };
static const char *mpdprev[]   = { "mpdprev", NULL };
static const char *lock[]      = { "slock", NULL };
static const char *rotleft[]   = { "rotatemon", "DP-1", "left", NULL };
static const char *rotnorm[]   = { "rotatemon", "DP-1", "normal", NULL };
static const char *rebootcmd[] = { "doas", "/sbin/shutdown", "-r", "now", NULL };
static const char *quitcmd[]   = { "doas", "/sbin/shutdown", "-h", "now", NULL };
static const char *sleepcmd[]  = { "doas", "/usr/l/bin/zzz", NULL };

static Key keys[] = {
	/* modifier                     key        			function        argument */
/*	{ 0,				XF86XK_AudioRaiseVolume,	spawn,		{.v = volupcmd } },
 *	{ 0,				XF86XK_AudioLowerVolume,	spawn,		{.v = voldncmd } },
 */
	{ 0,				XF86XK_AudioMute,		spawn,		{.v = volmucmd } },
	{ 0,				XF86XK_AudioPlay,		spawn,		{.v = mpdtog } },
	{ 0,				XF86XK_AudioNext,		spawn,		{.v = mpdnext } },
	{ 0,				XF86XK_AudioPrev,		spawn,		{.v = mpdprev } },
	{ MODKEY|ShiftMask,             XK_w,      			spawn,          {.v = webcmd } },
	{ MODKEY|ShiftMask,		XK_l,				spawn,		{.v = lock } },
	{ WINKEY|ShiftMask,		XK_p,				spawn,		{.v = quitcmd } },
	{ WINKEY|ShiftMask,		XK_r,				spawn,		{.v = rebootcmd } },
	{ WINKEY|ShiftMask,		XK_z,				spawn,		{.v = sleepcmd } },
	{ WINKEY|ShiftMask,		XK_9,				spawn,		{.v = rotleft } },
	{ WINKEY|ShiftMask,		XK_0,				spawn,		{.v = rotnorm } },
	{ WINKEY|ShiftMask,		XK_2,				spawn,		SHCMD("xscreenshot | ff2png > "SCROT) },
	{ WINKEY|ShiftMask,		XK_3,				spawn,		SHCMD("xscreenshot $(pfw) | ff2png > "SCROT) },
	{ ControlMask,			XK_space,			spawn,		SHCMD("kill $(pgrep -n notify)") },
	{ MODKEY,                       XK_p,      			spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, 			spawn,          {.v = termcmd } },

	{ MODKEY,                       XK_b,      			togglebar,      {0} },
	{ MODKEY,                       XK_j,      			focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      			focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      			incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },

	/* Layouts */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_y,      togglefakefull, {0} },

	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

