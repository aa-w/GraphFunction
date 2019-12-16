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
#define WAVEFORMCHARTY 10

#define WAVEFORMLENGTH 20

int16_t TouchedTag;
int16_t WaveFormWidth = 300;
int16_t WaveFormHeight = 250;
int16_t dur = 0;

char CharOutput [12];

int LoopVal = 0;
int16_t ShiftRegister [WAVEFORMLENGTH];
unsigned long ShiftRegisterTimer;

void setup()
{

  CleO.begin();
  CleO.DisplayRotate(2, 0);
  CleO.SetBackgroundcolor(WHITE);
  pinMode(5, INPUT_PULLUP);

  Serial.begin(19200);

  CleO.PointColor(BLUE);
  CleO.PointRadius(3);
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

  //unsigned long CurrentSecond = ((millis() / 1000U) % 10);



  //Serial.println(millis());


  BuildGraph();

  
    if (ShiftRegisterTimer < millis())
    {
    UpdateShiftRegister();

    ShiftRegisterTimer = millis() + 50;
    }
  


  /*
  if (LoopVal != 10)
  {
    
    LoopVal++;
  }
  else
  {
    LoopVal = 0;
  }
  */
  
  //UpdateShiftRegister(LoopVal);

  PlotShiftRegister();

  //AnimationLoop();
  CleO.Show();


}

/*
  void AnimationLoop() //Displays the frame refresh rate
  {
  CleO.LineColor(RED);

  //unsigned long millisx = millis();
  double CurrentSecond = ((millis() / 1000U) % 10); //Gets the seconds in millis
  double CurrentMiliSecond = ((millis() / 100U) % 10);

  CleO.Line((0 * SCREENWIDTH), (0.985 * SCREENHEIGHT), (((CurrentSecond / 10) + (CurrentMiliSecond / 100)) * SCREENWIDTH), (0.985 * SCREENHEIGHT));
  }
*/

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
  for (int PositionPlacement = 0; PositionPlacement != (WAVEFORMLENGTH + 1); PositionPlacement++)
  {
    itoa(PositionPlacement, Value, 10);
    if (ScaleXPosition > 8)
    {
      CleO.StringExt(FONT_SANS_3, ((WAVEFORMCHARTX + 25) + XPositionCoords), (WaveFormHeight - 15), BLACK, MM, 0, 0, Value);
    }
    else
    {
      if (PositionPlacement == 0 || PositionPlacement == (WAVEFORMLENGTH - 1))
      {
        CleO.StringExt(FONT_SANS_3, ((WAVEFORMCHARTX + 25) + XPositionCoords), (WaveFormHeight - 15), BLACK, MM, 0, 0, Value);
      }
    }
    XPositionCoords = ScaleXPosition + XPositionCoords;
  }

  //Y position Placement
  for (int PositionPlacement = 0; PositionPlacement != (WAVEFORMLENGTH + 1); PositionPlacement++)
  {
    itoa(PositionPlacement, Value, 10);

    if (ScaleYPosition > 8)
    {
      CleO.StringExt(FONT_SANS_3, (WAVEFORMCHARTX + 20), (WaveFormHeight - YPositionCoords), BLACK, MM, 0, 0, Value);
    }
    else
    {
      if (PositionPlacement == 0 || PositionPlacement == (WAVEFORMLENGTH - 1))
      {
        CleO.StringExt(FONT_SANS_3, (WAVEFORMCHARTX + 20), (WaveFormHeight - YPositionCoords), BLACK, MM, 0, 0, Value);
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

void PlotShiftRegister()
{
  CleO.PointColor(RED);
  CleO.PointRadius(3);
  for (byte index = 0; index < WAVEFORMLENGTH; index++)
  {
    //CleO.Point(((WaveFormWidth / WAVEFORMLENGTH) * index), ((WaveFormHeight / WAVEFORMLENGTH) * ShiftRegister[index]));
    if (index != 0 && index != WAVEFORMLENGTH)
    {
      CleO.LineColor(BLUE);
      CleO.Line((((WaveFormWidth / WAVEFORMLENGTH) * (index - 1)) + 25), (((WaveFormHeight / WAVEFORMLENGTH) * ShiftRegister[index - 1]) + 25), (((WaveFormWidth / WAVEFORMLENGTH) * index) + 25), (((WaveFormHeight / WAVEFORMLENGTH) * ShiftRegister[index]) + 25));
    }
  }
}

void PlotValue(int x, int y)
{
  CleO.PointColor(RED);
  CleO.PointRadius(3);

}

void PlotGrid()
{
  //CleO.Line((0 * SCREENWIDTH), (0.985 * SCREENHEIGHT), (((CurrentSecond / 10) + (CurrentMiliSecond / 100)) * SCREENWIDTH), (0.985 * SCREENHEIGHT));
}

void UpdateShiftRegister()
{

  //double CurrentSecond = ((millis() / 1000U) % 10); //Gets the seconds in millis
  double CurrentMiliSecond = ((millis() / 100U) % 10);
  int NewValue = (int) CurrentMiliSecond;
  for (int index = WAVEFORMLENGTH; index != 0; index--)
  {
    ShiftRegister[index] = ShiftRegister[index - 1];
  }

  ShiftRegister[0] = NewValue;
  Serial.println(NewValue);
}
