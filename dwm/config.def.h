/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 15;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 15;       /* vert inner gap between windows */
static const unsigned int gappoh    = 15;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 15;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { " Hack Nerd Font:size=12" };
static const char dmenufont[]       = " Hack Nerd Font:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_white1[]      = "#ffffff";
static const char col_cyan[]        = "#005577";
static const char col_purp[]        = "#9966cc";
static const char col_red1[]        = "#fb0000";
static const char col_red2[]        = "#2B0000";
static const char col_black[]       = "#151515";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white1, col_black, col_red2 },
	[SchemeSel]  = { col_black, col_red1,  col_red1  },
};

/* tagging */
static const char *tags[] = { "ﲵ", "2", "3", "4", "5", "6", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",		NULL,	NULL,	0,            1,           -1 },
	{ "firefox-esr",	NULL,	NULL,	1 << 8,       0,           -1 },
	{ "st-256color",	NULL,	"mpd",	1 << 7,       0,           -1 },
	{ "WebCord",		NULL,	NULL,	1 << 6,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define PrintScreenDWM		0x0000ff61


/* helper for spawning shell commands in the pre dwm-5.0 fashion */

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] =			{ "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_black, "-nf", col_white1, "-sb", col_red1, "-sf", col_black, NULL };
static const char *termcmd[]  = 		{ "st", NULL };
static const char *mpdmenucmd[] = 		{ "/home/kweint/Documents/Scripts/dmenu/mpdmenu", NULL};
static const char *mpdvolumedowncmd[] =		{ "/home/kweint/Documents/Scripts/dunst/volumedunst", "mpd", "down" };
static const char *mpdvolumeupcmd[] = 		{ "/home/kweint/Documents/Scripts/dunst/volumedunst", "mpd", "up" };
static const char *mpctogglecmd[] = 		{ "/home/kweint/Documents/Scripts/dunst/volumedunst", "mpd", "mute" };
static const char *mpcnextcmd[] = 		{ "/home/kweint/Documents/Scripts/dunst/volumedunst", "mpd", "next" };
static const char *mpcprevcmd[] = 		{ "/home/kweint/Documents/Scripts/dunst/volumedunst", "mpd", "prev" };
static const char *volumedowncmd[] = 		{ "/home/kweint/Documents/Scripts/dunst/volumedunst", "down" };
static const char *volumeupcmd[] = 		{ "/home/kweint/Documents/Scripts/dunst/volumedunst", "up" };
static const char *volumemutecmd[] = 		{ "/home/kweint/Documents/Scripts/dunst/volumedunst", "mute" };
static const char *lockcmd[] = 			{ "slock" , NULL };
static const char *regionscreenshotcmd[] =	{ "bash", "/home/kweint/Documents/Scripts/screenshot.sh" };
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } }, 
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.03} },
       	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.03} },
	{ MODKEY|Mod1Mask,              XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_y,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,			XK_F12,    spawn,          {.v = mpdmenucmd } },
	{ MODKEY,			XK_F11,    spawn,          {.v = mpctogglecmd } },
	{ MODKEY,			XK_F9,     spawn,          {.v = lockcmd } },
	{ MODKEY,			XK_F8,     spawn,          {.v = mpcnextcmd } },
	{ MODKEY,			XK_F7,     spawn,          {.v = mpcprevcmd } },
	{ MODKEY|ShiftMask,		XK_equal,  spawn,          {.v = mpdvolumeupcmd} },
	{ MODKEY|ShiftMask,		XK_minus,  spawn,          {.v = mpdvolumedowncmd} },
	{ MODKEY|ControlMask,		XK_equal,  spawn,          {.v = volumeupcmd} },
	{ MODKEY|ControlMask,		XK_minus,  spawn,          {.v = volumedowncmd} },
	{ MODKEY|ControlMask,		XK_m,	   spawn,          {.v = volumemutecmd} },
	{ 0,				PrintScreenDWM,		spawn,		{.v = regionscreenshotcmd } },
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
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

