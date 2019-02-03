/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 8;       /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "xos4 Terminus:pixelsize=15" };
static const char dmenufont[]       = "xos4 Terminus:pixelsize=15";
static const char col_gray1[]       = "#222222";
static const char bg[]              = "#282828";
static const char black[]           = "#000000";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char orange_dark[]     = "#d65d0e";
static const char orange_light[]    = "#fe8019";
static const char gb_fg[]           = "#ebdbb2";
static const char gb_blue[]         = "#83a598";
static const char gb_grey[]         = "#928374";
static const char gb_bg4[]          = "#7c6f64";
static const char green_dark[]      = "#98971a";
static const char fg[]              = "#ebdbb2";


static const char *colors[][3]      = {
	/*               fg          bg         border   */
	[SchemeNorm] = { fg,         bg,        bg },
	[SchemeSel]  = { green_dark, bg,        gb_bg4 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Firefox",  NULL,       NULL,       1 << 1,       0,           0 },
	{ "mutt",     NULL,       NULL,       1 << 0,       0,           0 },
	{ "calender", NULL,       NULL,       1 << 0,       0,           0 },
	{ "rss",      NULL,       NULL,       1 << 0,       0,           0 },
	{ "org",      NULL,       NULL,       1 << 0,       0,           0 },
	{ "Rambox",   NULL,       NULL,       1 << 6,       0,           0 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]   = { "dmenu_run", "-m", dmenumon,                NULL };
static const char *termcmd[]    = { "st",                                       NULL };
/* screen */
static const char *screensetup[]    = { "monitor.sh",                           NULL };
static const char *screenext[]      = { "set-external.sh",                      NULL };
static const char *screendualext[]  = { "set-dual-external.sh",                 NULL };
static const char *screenint[]      = { "set-internal.sh",                      NULL };
/* sound */
static const char *volup[]      = { "amixer", "sset", "Master", "5%+",          NULL };
static const char *voldown[]    = { "amixer", "sset", "Master", "5%-",          NULL };
static const char *volmute[]    = { "amixer", "sset", "Master", "toggle",       NULL };
/* screen brightness */
static const char *bklu[]       = { "xbacklight", "-inc", "5",                  NULL };
static const char *bkld[]       = { "xbacklight", "-dec", "5",                  NULL };
/* music */
static const char *mpcnext[]    = { "mpc", "next",                              NULL };
static const char *mpcprev[]    = { "mpc", "prev",                              NULL };
static const char *mpctoggle[]  = { "mpc", "toggle",                            NULL };
/* misc */
static const char *lockcmd[]    = { "slock",                                    NULL };
static const char *mailcmd[]    = { "st", "-c", "mutt", "-e", "neomutt", NULL, "mutt" };
static const char *browsercmd[] = { "firefox", NULL, NULL, NULL, NULL, NULL, "Firefox" };
static const char *calcmd[]     = { "st", "-c", "calender", "-e", "calcurse", NULL, "calender" };
static const char *rsscmd[]     = { "st", "-c", "rss", "-e", "newsboat", NULL, "rss" };
static const char *orgcmd[]     = { "st", "-c", "org", "-e", "vim", "org/life.org", "org" };
static const char *chatcmd[]    = { "rambox", NULL, NULL, NULL, NULL, NULL, "Rambox"};
static const char *musiccmd[]   = { "st", "-e", "ncmpcpp",                      NULL };
static const char *rangercmd[]  = { "st", "-e", "ranger",                       NULL };
static const char *passmenucmd[]  = { "dmenu_pass",                               NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_semicolon, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_m,      runorraise,     {.v = mailcmd } },
	{ MODKEY,                       XK_b,      runorraise,     {.v = browsercmd } },
	{ ALTKEY,                       XK_space,  spawn,          {.v = rangercmd } },
	{ MODKEY,                       XK_r,      spawn,          {.v = rangercmd } },
	{ MODKEY,                       XK_z,      runorraise,     {.v = calcmd } },
	{ MODKEY,                       XK_n,      runorraise,     {.v = rsscmd } },
	{ MODKEY,                       XK_o,      runorraise,     {.v = orgcmd } },
	{ MODKEY,                       XK_c,      runorraise,     {.v = chatcmd } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          {.v = musiccmd } },
	{ MODKEY,                       XK_p,      spawn,          {.v = passmenucmd } },
    /* sys */
    { MODKEY,                       XK_Delete, spawn,          {.v = lockcmd } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = screensetup } },
    { ALTKEY,                       XK_d,      spawn,          {.v = screendualext } },
    { ALTKEY,                       XK_i,      spawn,          {.v = screenint } },
    /* base */
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_a,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    /* tagkey */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_7,                      4)
	TAGKEYS(                        XK_8,                      5)
	TAGKEYS(                        XK_9,                      6)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
    /* audio */
	{ 0,                XF86XK_AudioLowerVolume,      spawn,   {.v = voldown } },
	{ 0,                XF86XK_AudioMute,             spawn,   {.v = volmute } },
	{ 0,                XF86XK_AudioRaiseVolume,      spawn,   {.v = volup   } },
    /* screen brightness */
    { 0,                XF86XK_MonBrightnessUp,       spawn,   {.v = bklu } },
    { 0,                XF86XK_MonBrightnessDown,     spawn,   {.v = bkld } },
    /* screen setup */
	{ 0,                XF86XK_Display,               spawn,   {.v = screenext } },
    /* music */
	{ 0,                XF86XK_AudioPlay,             spawn,   {.v = mpctoggle } },
	{ 0,                XF86XK_AudioNext,             spawn,   {.v = mpcnext } },
	{ 0,                XF86XK_AudioPrev,             spawn,   {.v = mpcprev } },
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

