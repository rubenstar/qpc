/*****************************************************************************
* Product:  Initialization for emWin/uC/GUI, Win32 simulation VC++
* Last updated for version 7.1.3
* Last updated on  2022-11-16
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <www.gnu.org/licenses>.
*
* Contact information:
* <www.state-machine.com/licensing>
*****************************************************************************/
#include "GUI.h"
#include "LCD_SIM.h"

void SIM_X_Init() {
    if (((LCD_GetDevCap(LCD_DEVCAP_XSIZE) * LCD_GetXMag()) == 320)
        && ((LCD_GetDevCap(LCD_DEVCAP_YSIZE) * LCD_GetYMag()) == 240)
        && (LCD_GetNumLayers() == 1))
    {
        SIM_GUI_SetLCDPos(132, 46); /* position of the LCD in the bitmap */
        SIM_GUI_SetTransColor(0xFF0000);       /* transparent color */
        SIM_GUI_SetLCDColorBlack(0, 0x000000); /* "black" color */
        SIM_GUI_SetLCDColorWhite(0, 0xC0C0C0); /* "white" color */
    }
}
