//////////// Graph Tool Test program
#include <SPI.h>
#include <CleO.h>

#define SCREENWIDTH 480
#define SCREENHEIGHT 320

//WAVEFORM GLOBALS
//#define WAVEFORMWIDTH 200 //waveform bounds
//#define WAVEFORMHEIGHT 200 //waveform bounds
//#define WAVEFORMCHARTX (0.1 * SCREENWIDTH) //point waveform is drawn from
//#define WAVEFORMCHARTY (0.1 * SCREENHEIGHT)

#define WAVEFORMCHARTX 0
#define WAVEFORMCHARTY 35

#define WAVEFORMLENGTH 10

int16_t TouchedTag;
int16_t WaveFormWidth = 300;
int16_t WaveFormHeight = 250;
int16_t dur = 0;
int pincount = 0;
char CharOutput [12];

int ShiftRegister [WAVEFORMLENGTH];
long ShiftRegisterTimer = millis() + 1000;

void setup() {

  CleO.begin();
  CleO.DisplayRotate(2, 0);
  CleO.SetBackgroundcolor(BLACK);
  pinMode(5, INPUT_PULLUP);

  Serial.begin(9600);
  WaveformSetup();
}

void loop()
{

  //Update touch
  int16_t Tempx, Tempy, Tempdur, TempTouchedTag;
  CleO.TouchCoordinates(Tempx, Tempy, Tempdur, TempTouchedTag);
  
  if (Tempdur == 1)
  {
    WaveFormWidth = Tempx;
    WaveFormHeight = Tempy;
    dur = Tempdur;
    TouchedTag = TempTouchedTag;
  }
  
  CleO.Start();
  
  BuildGraph();
  PlotValue(1,1);
  PlotValue(2,2);
  PlotValue(3,3);
  PlotValue(4,4);
  PlotValue(5,5);
  PlotValue(6,6);
  PlotValue(7,5);
  PlotValue(8,8);
  CleO.Show();

}

void BuildGraph()
{
  int ScaleXPosition = WaveFormWidth / WAVEFORMLENGTH;
  int ScaleYPosition = WaveFormHeight / WAVEFORMLENGTH;
  char Value [1];
  char TestVal2 [2];
  char TestVal [2];
  //Outer region with scale
  CleO.RectangleCorner(1);
  CleO.RectangleColor(LIGHTGREY);
  CleO.RectangleXY(WAVEFORMCHARTX, WAVEFORMCHARTY, WaveFormWidth, WaveFormHeight);
  CleO.RectangleColor(WHITE);
  CleO.RectangleXY((WAVEFORMCHARTX + 25), WAVEFORMCHARTY, GraphRegionX(), GraphRegionY());

  //Serial.println(WaveFormWidth);
  //Serial.println(WaveFormHeight);
  //Layout text legend

  //Plot Scaled values
  int XPositionCoords = 0 - XPositionCoords;
  int YPositionCoords = 0 - YPositionCoords;

  //X position Placement
  for (int PositionPlacement = 0; PositionPlacement != WAVEFORMLENGTH; PositionPlacement++)
  {
    itoa(PositionPlacement, Value, 10);
    if (ScaleXPosition > 8)
    {
      CleO.StringExt(FONT_SANS_3, ((WAVEFORMCHARTX + 25) + XPositionCoords), (WaveFormHeight + 20), BLACK, MM, 0, 0, Value);
    }
    else
    {
      if (PositionPlacement == 0 || PositionPlacement == (WAVEFORMLENGTH - 1))
      {
        CleO.StringExt(FONT_SANS_3, ((WAVEFORMCHARTX + 25) + XPositionCoords), (WaveFormHeight + 20), BLACK, MM, 0, 0, Value);
      }
    }
    XPositionCoords = ScaleXPosition + XPositionCoords;
  }

  //Y position Placement
  for (int PositionPlacement = 0; PositionPlacement != WAVEFORMLENGTH; PositionPlacement++)
  {
    itoa(PositionPlacement, Value, 10);

    if (ScaleYPosition > 8)
    {
      CleO.StringExt(FONT_SANS_3, (WAVEFORMCHARTX + 10), (WaveFormHeight - YPositionCoords), BLACK, MM, 0, 0, Value);
    }
    else
    {
      if (PositionPlacement == 0 || PositionPlacement == (WAVEFORMLENGTH - 1))
      {
        CleO.StringExt(FONT_SANS_3, (WAVEFORMCHARTX + 10), (WaveFormHeight - YPositionCoords), BLACK, MM, 0, 0, Value);
      }
    }
    YPositionCoords = ScaleYPosition + YPositionCoords;
  }

}

int GraphRegionX()
{
  return WaveFormWidth - 25;
}

int GraphRegionY() 
{
  return WaveFormHeight - 25;
}

void WaveformSetup() //Called once at setup used to set the parameters of the graph
{

}

void SetWaveformValue (int Value)
{
  
}

void PlotValue(int x, int y)
{
  CleO.PointColor(BLACK);
  CleO.PointRadius(3);
  CleO.Point(((WaveFormWidth / WAVEFORMLENGTH) * x),((WaveFormHeight / WAVEFORMLENGTH) * y));
  //Serial.println(x);
  //Serial.println(y);
  //Serial.println((GraphRegionX() / WAVEFORMLENGTH) * x);
  //Serial.println((GraphRegionY() / WAVEFORMLENGTH) * y);
}

void PlotGrid()
{
  //CleO.Line((0 * SCREENWIDTH), (0.985 * SCREENHEIGHT), (((CurrentSecond / 10) + (CurrentMiliSecond / 100)) * SCREENWIDTH), (0.985 * SCREENHEIGHT));
}
  
void UpdateShiftRegister()
{
  if(ShiftRegisterTimer > millis)
  {
  for(byte index = 0; index != WAVEFORMLENGTH; index++)
  {
  ShiftRegister[index + 1] = ShiftRegister [index];
  }
  }
  
}
