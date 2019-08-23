#include <windows.h>
#include "Fill.h"
#include <bits/stdc++.h> 
using namespace std; 
void fill();

void setupMenus(HWND hwnd)
{
  HMENU hmenu, hsubmenu;
  hmenu=GetMenu(hwnd);
  hsubmenu=GetSubMenu(hmenu, 0);

  switch (gDrawData.drawMode)
  {
    case READY_MODE :
      // enable 'Draw Polygon', disable 'Filling' menu
      EnableMenuItem(hsubmenu,ID_DRAW_POLY,
                     MF_BYCOMMAND|MF_ENABLED);
      EnableMenuItem(hsubmenu,ID_FILL,
                     MF_BYCOMMAND|MF_GRAYED);
      break;
    case DRAW_MODE :
    case FILL_MODE :
    case FILLED_MODE :
      // disable 'Draw Polygon', 'Filling' menu 
      EnableMenuItem(hsubmenu,ID_DRAW_POLY,
                     MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hsubmenu,ID_FILL,
                     MF_BYCOMMAND|MF_GRAYED);
      break;
    case DRAWN_MODE :
      // enable 'Filling' menus, disable 'Draw Polygon' menu
      EnableMenuItem(hsubmenu,ID_DRAW_POLY,
                     MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hsubmenu,ID_FILL,
                     MF_BYCOMMAND|MF_ENABLED);
      break;
  }
}

void performFilling(HWND hwnd)
{
  setDrawMode(FILL_MODE, hwnd);
  SelectObject(gDrawData.hdcMem, gDrawData.hFillPen);
  fill();
  reDraw(hwnd);
  setDrawMode(FILLED_MODE, hwnd);
}

void processCommand(int cmd, HWND hwnd)
{
  switch(cmd)
  {
    case ID_FILL:
      performFilling(hwnd);
      break;
    default:
      processCommonCommand(cmd, hwnd);
      break;
  }
}

float area(int x1, int y1, int x2, int y2, int x3, int y3) 
{ 
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0); 
} 
  
bool isInsideTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y) 
{    
   float A = area (x1, y1, x2, y2, x3, y3);   
   float A1 = area (x, y, x2, y2, x3, y3);   
   float A2 = area (x1, y1, x, y, x3, y3);  
   float A3 = area (x1, y1, x2, y2, x, y);     
   return (A == A1 + A2 + A3); 
} 

void fill()
{
	bool insideTriangle[3];
	long X_MIN = gDrawData.cornerPts[0].x;
	long X_MAX = gDrawData.cornerPts[0].x;
	long Y_MIN = gDrawData.cornerPts[0].y;
	long Y_MAX = gDrawData.cornerPts[0].y;
	for (int i = 1; i < gDrawData.nCornerPts; i++) 
  	{
    	X_MIN = min(gDrawData.cornerPts[i].x ,X_MIN);
    	X_MAX = max(gDrawData.cornerPts[i].x ,X_MAX);   
		Y_MIN = min(gDrawData.cornerPts[i].y ,Y_MIN);
		Y_MAX = max(gDrawData.cornerPts[i].y ,Y_MAX);
	}
	
	for(int y = Y_MIN; y <= Y_MAX; y++){	
		for (int x = X_MIN; x <= X_MAX; x++){
			COLORREF color = GetPixel(gDrawData.hdcMem, x, y);
			// check if it is boundary
			// update the gDrawData
			if (isInsideTriangle(gDrawData.cornerPts[0].x, gDrawData.cornerPts[0].y, gDrawData.cornerPts[1].x, gDrawData.cornerPts[1].y, gDrawData.cornerPts[2].x, gDrawData.cornerPts[2].y, x, y))		insideTriangle[0] = true;
			else insideTriangle[0] = false;
			if (isInsideTriangle(gDrawData.cornerPts[3].x, gDrawData.cornerPts[3].y, gDrawData.cornerPts[4].x, gDrawData.cornerPts[4].y, gDrawData.cornerPts[5].x, gDrawData.cornerPts[5].y, x, y))		insideTriangle[1] = true;
			else insideTriangle[1] = false;
			if (isInsideTriangle(gDrawData.cornerPts[6].x, gDrawData.cornerPts[6].y, gDrawData.cornerPts[7].x, gDrawData.cornerPts[7].y, gDrawData.cornerPts[8].x, gDrawData.cornerPts[8].y, x, y))		insideTriangle[2] = true;
			else insideTriangle[2] = false;

			if (insideTriangle[0] && insideTriangle[1] && insideTriangle[2])	SetPixel(gDrawData.hdcMem, x, y, RGB(255, 0, 0));
			else if (insideTriangle[0] && insideTriangle[1] )	SetPixel(gDrawData.hdcMem, x, y, RGB(0, 0, 255));
			else if (insideTriangle[1] && insideTriangle[2] )	SetPixel(gDrawData.hdcMem, x, y, RGB(0, 255, 0));
			else if (insideTriangle[0] && insideTriangle[2] )	SetPixel(gDrawData.hdcMem, x, y, RGB(255, 255, 0));
			else if (insideTriangle[0] || insideTriangle[1] || insideTriangle[2])	SetPixel(gDrawData.hdcMem, x, y, RGB(0, 255, 255));
			else SetPixel(gDrawData.hdcMem, x, y, RGB(255, 255, 255));
		}								
			
	}	
}

