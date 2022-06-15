/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: PanelFunc */
#define  PANEL_COMMANDBUTTON              2       /* control type: command, callback function: Connect */
#define  PANEL_TEXTBOX                    3       /* control type: textBox, callback function: (none) */
#define  PANEL_STRING                     4       /* control type: string, callback function: sendInfo */
#define  PANEL_GRAPH                      5       /* control type: graph, callback function: (none) */
#define  PANEL_COMMANDBUTTON_3            6       /* control type: command, callback function: clearGraph */
#define  PANEL_NUMERIC                    7       /* control type: numeric, callback function: (none) */
#define  PANEL_COMMANDBUTTON_6            8       /* control type: command, callback function: runCalibrationTest */
#define  PANEL_COMMANDBUTTON_2            9       /* control type: command, callback function: exportToPng */
#define  PANEL_TEXTMSG_2                  10      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG                    11      /* control type: textMsg, callback function: (none) */
#define  PANEL_COLORNUM                   12      /* control type: color, callback function: (none) */
#define  PANEL_COMMANDBUTTON_4            13      /* control type: command, callback function: zoomInCursor */
#define  PANEL_DECORATION_2               14      /* control type: deco, callback function: (none) */
#define  PANEL_COMMANDBUTTON_5            15      /* control type: command, callback function: restoreGraph */
#define  PANEL_DECORATION                 16      /* control type: deco, callback function: (none) */
#define  PANEL_NUMERIC_3                  17      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_2                  18      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK clearGraph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Connect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exportToPng(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PanelFunc(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK restoreGraph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK runCalibrationTest(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK sendInfo(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK zoomInCursor(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
