#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
#define ICONSIZE 14   /* icon size */
#define ICONSPACING 5 /* space between icon and title */
static const char *fonts[]          = { "Fixedsys Excelsior:size=11", "FontAwesome:size=14" };
static const char dmenufont[]       = "FixedsysExcelsior:size=11";
static const char col_gray1[]       = "#c0c7cf";
static const char col_gray2[]       = "#ccd4dc";
static const char col_gray3[]       = "#000000";
static const char col_gray4[]       = "#ffffff";
static const char col_gray5[]       = "#80888f";
static const char col_cyan[]        = "#0000aa";
static const char *colors[][4]      = {
  /*               fg         bg         borderh,     borderl   */
  [SchemeNorm] = { col_gray3, col_gray1, col_gray4,   col_gray5 },
  [SchemeSel]  = { col_gray3, col_gray2,  col_gray5,   col_gray4 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
   */
  /* class      instance    title       tags mask     isfloating   monitor */
  { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static void (*bartab_fulllayoutfor[])(Monitor *) = { monocle, grid };

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "\uF0C8",      monocle },
  { "\uF0C9",      deck },
  { "\uF58D",      grid },
  { "><>",      NULL },
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *brightness_up[] = { "brightness_up", NULL };
static const char *brightness_down[] = { "brightness_down", NULL };
static const char *volume_up[] = { "volume_up", NULL };
static const char *volume_down[] = { "volume_down", NULL };
static const char *volume_mute[] = { "volume_mute", NULL };
static const char *mount[] = { "dmenu_mount", "mount" };
static const char *unmount[] = { "dmenu_mount", "unmount" };

static const Key keys[] = {

  TAGKEYS(  XK_1,  0)
  TAGKEYS(  XK_2,  1)
  TAGKEYS(  XK_3,  2)
  TAGKEYS(  XK_4,  3)
  TAGKEYS(  XK_5,  4)
  TAGKEYS(  XK_6,  5)
  TAGKEYS(  XK_7,  6)

  /* modifier                     key                         function        argument */
  { MODKEY,                       XK_p,                       spawn,          {.v = dmenucmd } },
  { MODKEY|ShiftMask,             XK_Return,                  spawn,          {.v = termcmd } },
  { 0,                            XF86XK_MonBrightnessUp,     spawn,          {.v = brightness_up } },
  { 0,                            XF86XK_MonBrightnessDown,   spawn,          {.v = brightness_down } },
  { MODKEY|ShiftMask,             XK_m,                       spawn,          {.v = volume_mute } },
  { MODKEY|ShiftMask,             XK_comma,                   spawn,          {.v = volume_down } },
  { MODKEY|ShiftMask,             XK_period,                  spawn,          {.v = volume_up } },
  { MODKEY|ShiftMask,             XK_period,                  spawn,          {.v = volume_up } },
  { MODKEY,                       XK_8,                       spawn,          {.v = unmount } },
  { MODKEY,                       XK_9,                       spawn,          {.v = mount } },
  { MODKEY,                       XK_b,                       togglebar,      {0} },
  { MODKEY,                       XK_j,                       focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,                       focusstack,     {.i = -1 } },
  { MODKEY,                       XK_i,                       incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_d,                       incnmaster,     {.i = -1 } },
  { MODKEY,                       XK_h,                       setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,                       setmfact,       {.f = +0.05} },
  { MODKEY,                       XK_Return,                  zoom,           {0} },
  { MODKEY|ShiftMask,             XK_c,                       killclient,     {0} },
  { MODKEY,                       XK_space,                   setlayout,      {.v = &layouts[0]} },
  { MODKEY|ShiftMask,             XK_space,                   setlayout,      {.v = &layouts[1]} },
  { MODKEY,                       XK_Tab,                     setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_r,                       setlayout,      {.v = &layouts[3]} },
  { MODKEY|ShiftMask,             XK_f,                       togglefloating, {0} },
  { MODKEY,                       XK_0,                       view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,                       tag,            {.ui = ~0 } },
  { MODKEY,                       XK_w,                       focusmon,       {.i = -1 } },
  { MODKEY,                       XK_e,                       focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_w,                       tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_e,                       tagmon,         {.i = +1 } },
  { MODKEY,                       XK_x,                       togglesecondarybar, {0}},
  { MODKEY|ShiftMask,             XK_q,                       quit,           {0} },
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

