/* See LICENSE file for copyright and license details. */


/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 10;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int cursor_wrap = 1;
static const char *fonts[]          = { "mono:size=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "JetBrainsMonoNL:size=10";
static const char font_size         = 10; /* this setting is used for padding in colored status bar */
static const char col_gray1[]       = "#131313";
static const char col_gray2[]       = "#212121";
static const char col_cyan[]        = "#d79921";
static const char *tags[] = { "1", "2", "3", "4", "5", "6" };

#define WHITE "#ebdbb2"
#define GRAY "#504945"
#define SEMI_BLACK "#212121"
#define BLACK "#131313"
#define YELLOW "#d79921"
#define RED "#770000"
#define ACCENT GRAY

static const char *colors[][3]  = {
	/*               fg         bg         border   */
	[SchemeNorm] = { WHITE, BLACK, SEMI_BLACK },
	[SchemeSel]  = { WHITE, ACCENT,  RED  },
	[SchemeTabActive]  = { BLACK, WHITE, GRAY },
	[SchemeTabInactive]  = { WHITE, SEMI_BLACK, BLACK },
	{ "#b8bb26", BLACK, BLACK },  // \x05 green
	{ "#fb4934", BLACK, BLACK },  // \x06 RED
	{ "#c58085", BLACK, BLACK },  // \x07 purple
	{ "#db930d", BLACK, BLACK },  // \x08 Yellow
	{ "#458588", BLACK, BLACK },  // \x09 blue
	{ BLACK, "#b8bb26", BLACK },  // \x09 BG green
};

static int g_to_swallow = 1;


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",             NULL,  NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox",          NULL,  NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "st-256color",      NULL,  NULL,           0,         0,          1,           1,        -1 },
	{ "chess",            NULL,  NULL,           0,         1,          0,           1,        -1 },
	{ "Blueman-manager",  NULL,  NULL,           0,         1,          0,           1,        -1 },
	{ NULL,      NULL,   "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

/* Bartabgroups properties */
#define BARTAB_BORDERS 1       // 0 = off, 1 = on
#define BARTAB_BOTTOMBORDER 1  // 0 = off, 1 = on
#define BARTAB_TAGSINDICATOR 1 // 0 = off, 1 = on if >1 client/view tag, 2 = always on
#define BARTAB_TAGSPX 5        // # pixels for tag grid boxes
#define BARTAB_TAGSROWS 3      // # rows in tag grid (9 tags, e.g. 3x3)
static void (*bartabmonfns[])(Monitor *) = { monocle /* , customlayoutfn */ };
static void (*bartabfloatfns[])(Monitor *) = { NULL /* , customlayoutfn */ };


static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
/* static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL }; */
// static const char *dmenucmd[] = { "rofi", "-modi", "drun", "-show", "drun", NULL };
static const char *dmenucmd[] = { "/home/white/.config/rofi/launchers/type-4/./launcher.sh", NULL };
/* rofi -modi drun -show drun */
static const char *termcmd[]  = { "st", NULL };

/* static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL }; */
/* static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL }; */

/* static const char *upvol[]      = { "/usr/bin/amixer",  "set", "Master", "5%+", NULL }; */
/* static const char *downvol[]    = { "/usr/bin/amixer",  "set", "Master", "5%-", NULL }; */

#define IG(x) (x == 1)

void
toggle_swallow(const Arg *arg)
{
	g_to_swallow = !g_to_swallow;
	if (g_to_swallow)
		system("dunstify \"Swallow: On\" -r 7 -t 1000");
	else
		system("dunstify \"Swallow: Off\" -r 7 -t 1000");
}

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_slash,      spawn,      {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_s,      toggle_swallow,  {0} },
	{ MODKEY,                       XK_h,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_l,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	/* { MODKEY, 			XK_space,  spawn,	SHCMD("~/archive/.scripts/toggle_keyboard.sh")}, */
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
static const Button buttons[] = {
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

