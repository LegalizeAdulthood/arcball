#pragma once

typedef enum
{
    WINSHUT = 0,
    MOUSEX,
    MOUSEY,
    LEFTMOUSE,
    LEFTCTRLKEY,
    RIGHTCTRLKEY,
    LEFTSHIFTKEY,
    RIGHTSHIFTKEY,
    CAPSLOCKKEY,
    INPUTCHANGE,
    REDRAW,
} Device;

void qdevice(int);
int qtest();
Device qread(short *);
