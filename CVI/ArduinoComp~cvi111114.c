#include <analysis.h>
#include <ansi_c.h>
#include <rs232.h>
#include <cvirte.h>		
#include <userint.h>
#include "ArduinoComp.h"

static int panelHandle;
static int portVal;
static int xArr[200],yArr[200];
static int index=0;
static double xArrDouble[10], yArrDouble[10], yPolyData[100], polynom[7], polyError;



int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "ArduinoComp.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	CloseCom(portVal);
	return 0;
}

int CVICALLBACK PanelFunc (int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

void CVICALLBACK SerialFunc (int portNumber, int eventMask, void *callbackData)
{
	char message[100],dataToGraph[100];
	int bytesRead,colorNum; //color of graph line
	static int lines = 0;
	int check;
	while(GetInQLen (portVal) > 0) //if the input is not empty
	{
		bytesRead = ComRdTerm (portVal, message, 100, '\n'); //read com into messege string
		if(bytesRead>0)
		{
			//____KEY_____
			/*
			  # first character is a NUMBER means that its data to graph
				goes in to array.	format: (int , int)
			
			  # first character is a LETTER means its text to display
				format ( text )
			
			  # end character is * means its data from case 2
			
			  # end character is $ means its data from case 3
			
			*/
			
			message[bytesRead - 1] = '\0'; 		//trim the last character
			InsertTextBoxLine (panelHandle, PANEL_TEXTBOX, -1, message);
			lines++;
			SetCtrlAttribute (panelHandle, PANEL_TEXTBOX, ATTR_FIRST_VISIBLE_LINE, lines);
			

			check=message[0]; //determine what the first index hosts
			
		//________________Read the numders into arrays___________
			
			if(check>=48 && check<=57)	// ASCII for numbers 0-9
			{
				sscanf(message,"%d,%d",&xArr[index],&yArr[index]);

				index++;
			}
			
        //_____________________CASE 2 , 4 , 5_____________________________
		
			if(message[0]==42) // ASCII for '*'
			{
				SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON_4, ATTR_DIMMED, 0);//un-dim
				SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON_5, ATTR_DIMMED, 0);
				SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON_2, ATTR_DIMMED, 0);
				GetCtrlVal (panelHandle, PANEL_COLORNUM, &colorNum);
				GetCtrlVal (panelHandle, PANEL_STRING, dataToGraph);
				PlotXY (panelHandle, PANEL_GRAPH, xArr, yArr, 200, VAL_INTEGER, VAL_INTEGER, VAL_FAT_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, colorNum);
				PlotText (panelHandle, PANEL_GRAPH, 2080, yArr[index-5], dataToGraph, VAL_APP_META_FONT, VAL_BLUE, VAL_YELLOW);
				for(int j=0; j<200; j++)//reset arrays
				{
					xArr[j]=0;
					yArr[j]=0;
				}
				index=0;
			}
			
        //_____________________CASE 3_____________________________
			if(message[0]==36)		// ASCII for '$'
			{
				DeleteGraphPlot (panelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
				SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON_4, ATTR_DIMMED, 0);
				SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON_5, ATTR_DIMMED, 0);
				SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON_2, ATTR_DIMMED, 0);
				GetCtrlVal (panelHandle, PANEL_COLORNUM, &colorNum);
				
			
				PlotXY (panelHandle, PANEL_GRAPH, xArr, yArr, 10, VAL_INTEGER, VAL_INTEGER, VAL_SCATTER, VAL_X, VAL_SOLID, 1, colorNum);
				index=0;
				
				
				//run and delete error 
				//changing graph names and autoscale
				//return to default
				//adding target function from the user.
				
				for (int i=0; i<10; i++) //convert to double
				{
					xArrDouble[i]=(double)xArr[i];
					yArrDouble[i]=(double)yArr[i];
				}
				
				PolyFit (xArrDouble, yArrDouble, 10, 6, yPolyData, polynom, &polyError);
				
				double xPolyCheck[100],yPolyCheck[100];
				
				Ramp (100, xArrDouble[0], xArrDouble[9], xPolyCheck);
				
				PolyEv1D (xPolyCheck, 100, polynom, 7, yPolyCheck);
				PlotXY (panelHandle, PANEL_GRAPH, xPolyCheck, yPolyCheck, 100, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);

			}
			}
		}
	}




int CVICALLBACK Connect (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal (panelHandle, PANEL_NUMERIC, &portVal);
			OpenComConfig (portVal, "", 9600, 0, 8, 1, 512, 512);
			FlushInQ (portVal);
			InstallComCallback (portVal, LWRS_RXFLAG, 0, '\n', SerialFunc, 0);
			SetCtrlAttribute (panelHandle, PANEL_STRING, ATTR_DIMMED, 0);
			
			break;
	}
	return 0;
}

int CVICALLBACK sendInfo (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	char message[20];
	
	switch (event)
	{
		case EVENT_COMMIT:
			int check;
			
			GetCtrlVal(panelHandle, PANEL_STRING, message);
		//	check=message[0]; //determine what the first index hosts
		//	
		///*	switch (check)
		//	{
		//		case 4:
		//			sscanf(message,"%d,%d",&xArr[index],&yArr[index]);
		//			break;
		//	}
			
			ComWrt (portVal, message, strlen(message)+1);
			break;
	}
	return 0;
}

int CVICALLBACK clearGraph (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			
			DeleteGraphPlot (panelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
			SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON_4, ATTR_DIMMED, 1);
			SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON_5, ATTR_DIMMED, 1);
			SetCtrlAttribute (panelHandle, PANEL_COMMANDBUTTON_2, ATTR_DIMMED, 1);


			break;
	}
	return 0;
}

int CVICALLBACK zoomInCursor (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	double xMin,yMin;
	switch (event)
	{
		case EVENT_COMMIT:
		GetGraphCursor(panelHandle,PANEL_GRAPH,1,&xMin, &yMin);
        SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_XAXIS, VAL_MANUAL,xMin-10, xMin+10);
        SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, yMin-10, yMin+10);
        break;
	}
	return 0;
}

int CVICALLBACK restoreGraph (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_XAXIS, VAL_AUTOSCALE, 0, 2000);
         	SetAxisScalingMode (panelHandle, PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_AUTOSCALE, 0,700);

		break;
	}
	return 0;
}

int CVICALLBACK exportToPng (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			int graphBitmap;
			char dataToGraph[100];
			GetCtrlDisplayBitmap (panelHandle, PANEL_GRAPH, 1, &graphBitmap);
			GetCtrlVal (panelHandle, PANEL_STRING, dataToGraph);
			SaveBitmapToPNGFile (graphBitmap, dataToGraph);
			DiscardBitmap (graphBitmap);
			MessagePopup ("Message", "PNG File Saved");
			break;
	}
	return 0;
}
