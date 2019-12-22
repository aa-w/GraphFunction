/* CLEO GRAPH Tool
*  DEVELOPED BY ALEX WOODLEY 2019
*  This was made as a part time project if it doesn't work please don't hate me
*/

#include <SPI.h>
#include <CleO.h> //Both are required for CLEOs

//CLEO SCREEN SET UP
#define SCREENWIDTH 480
#define SCREENHEIGHT 320

#define XLEGENDBUFFERAMOUNT 10 //Legend Spacing Constants
#define YLEGENDBUFFERAMOUNT 10

int16_t TouchedTag = 255;
int16_t WaveformWidth = 300;
int16_t WaveformHeight = 250;
int16_t PlotAreaWidth = WaveformWidth - 25;
int16_t PlotAreaHeight = WaveformHeight - 25;
int16_t dur = 0;

//Graph GLOBALS

//XY Position that the chart will be drawn from
#define WAVEFORMCHARTX 0
#define WAVEFORMCHARTY 10

//Length of the waveform chart in data points
byte WaveformLength 20;
//height of waveform chart in data points (this may become scaling int he future)
byte WaveFormRange 20;
byte WaveFormRangeStart 0;
int16_t ShiftRegister [WaveformLength];
unsigned longCallDelay = 100; //Delay timer between XY calls

void setup()
{
  CleO.begin();
  CleO.DisplayRotate(2,0);

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

  //Gets the the current millisecond and casts it to an int
  double NewValue = ((millis() / 100U) % 10);
  int NewValue = (int) CurrentMiliSecond;

  CleO.Start();

  //These are the functions used for graphing waveforms
  BuildGraph(); //Builds the frame and scale around the graph
  BuildGrid(); //Builds a grid around the frame
  AddToGraph(NewValue); //Adds a value the graph ShiftRegister
  PlotGraph(); //Plots the graph values

  CleO.Show();
}

void BuildGraph()
{
  //REMOVE POSTION COORDS BECAUSE THEY ARENT NEEDED THIS CAN ALL BE CALCULATED PROGRAMATICALLY
  //gets the coord spacing for each data point
  int ScaleXPosition = WaveformWidth / WaveformLength;
  int ScaleYPosition = WaveformHeight / WaveFormRange;
  int XPositionCoords = 0;
  int YPositionCoords = 0;

  char LegendValue [1]; //Buffer used to plot each legend point
  CleO.RectangleCorner(1);
  CleO.RectangleColor(BLACK);
  CleO.RectangleXY((WAVEFORMCHARTX - 1), (WAVEFORMCHARTY - 1), (WaveFormWidth + 2), (WaveFormHeight + 2); //Border
  CleO.RectangleColor(LIGHTGREY);
  CleO.RectangleXY(WAVEFORMCHARTX, WAVEFORMCHARTY, WaveFormWidth, WaveFormHeight); //Legend Space

  //Select the for loops to use based on the space between legend numbers

  if (ScaleXPosition > 15) //Plots all values
  {
    for (int PositionPlacement = 0; PositionPlacement <= WaveformLength; PositionPlacement++)
    {
      itoa(PositionPlacement, Value, 10);
      CleO.StringExt(FONT_SANS_3, (PlotAreaWidth + XPositionCoords), (PlotAreaHeight + XLEGENDBUFFERAMOUNT), BLACK, MM, 0, 0, Value);
      XPositionCoords = XPositionCoords + ScaleXPosition;
    }
  }
  else if(ScaleXPosition < 15 && ScaleXPosition <= 12) //Plots every other values (Only Even and End)
  {
    /* Already plotted in the first execution of the for loop
    PositionPlacement = 0;
    itoa(PositionPlacement, Value, 10);
    CleO.StringExt(FONT_SANS_3, (PlotAreaWidth), (PlotAreaHeight + XLEGENDBUFFERAMOUNT), BLACK, MM, 0, 0, Value);
    XPositionCoords = ScaleXPosition * 2;
    */
    //refactor this
    //test before halfing amount of executions
    for(int PositionPlacement = 0; PositionPlacement <= WaveFormLength; PositionPlacement = PositionPlacement + 2)
    {
      if((PositionPlacement / 2) == 0)
      {
        itoa(PositionPlacement, Value, 10);
        CleO.StringExt(FONT_SANS_3, (PlotAreaWidth + XPositionCoords), (PlotAreaHeight + XLEGENDBUFFERAMOUNT), BLACK, MM, 0, 0, Value);
      }
      else if(PositionPlacement == WaveFormLength) //Always plots the end point (ScalePostion * WaveFormLength)
      {
        itoa(PositionPlacement, Value, 10);
        CleO.StringExt(FONT_SANS_3, (PlotAreaWidth + (ScaleXPosition * WaveFormLength)), (PlotAreaHeight + XLEGENDBUFFERAMOUNT), BLACK, MM, 0, 0, Value);
      }
      XPositionCoords = XPositionCoords + ScaleXPosition;
    }
    //add end number for execution
  }
  else if(ScaleXPosition < 12 && ScaleXPosition > 7) //Plots the end and middle values
  {
    //First Value
    PositionPlacement = 0;
    itoa(PositionPlacement, Value, 10);
    CleO.StringExt(FONT_SANS_3, PlotAreaWidth, (PlotAreaHeight + XLEGENDBUFFERAMOUNT), BLACK, MM, 0, 0, Value);

    //Middle Value
    char MiddleValue [3];
    double MiddlePositionPlacement = WaveformLength / 2; //May need checking for correct casting
    itoa(MiddlePositionPlacement, MiddleValue, 10)
    CleO.StringExt(FONT_SANS_3, (PlotAreaWidth + (ScaleXPosition * MiddlePositionPlacement)), PlotAreaHeight + XLEGENDBUFFERAMOUNT, BLACK, MM, 0, 0, MiddleValue);

    //End value
    itoa(WaveformLength, Value, 10);
    CleO.StringExt(FONT_SANS_3, (PlotAreaWidth + (ScaleXPosition * WaveformLength)), PlotAreaHeight + XLEGENDBUFFERAMOUNT, BLACK, MM, 0, 0, Value);
  }
  else //Just plots the end values
  {
    //First Value
    PositionPlacement = 0;
    itoa(PositionPlacement, Value, 10);
    CleO.StringExt(FONT_SANS_3, PlotAreaWidth, (PlotAreaHeight + XLEGENDBUFFERAMOUNT), BLACK, MM, 0, 0, Value);

    //End value
    itoa(WaveformLength, Value, 10);
    CleO.StringExt(FONT_SANS_3, (ScaleXPosition * WaveformLength), PlotAreaWidth + XLEGENDBUFFERAMOUNT, BLACK, MM, 0, 0, Value);
  }

  ///////////////////////// ------------- Plotting the Y Legend ------------- /////////////////////////

  if(ScaleYPosition > 15) //Plots all points
  {
    for (int PositionPlacement = 0; PositionPlacement <= WaveFormRange; PositionPlacement++)
    {
      itoa((WaveFormRangeStart + PositionPlacement), Value, 10);
      CleO.StringExt(FONT_SANS_3, (PlotAreaWidth - YLEGENDBUFFERAMOUNT), (PlotAreaHeight + YPositionCoords), BLACK, MM, 0, 0, Value);
      YPositionCoords = YPositionCoords + ScaleYPosition;
    }
  }
  else if(ScaleYPosition < 15 && ScaleYPosition <= 12) //Plots end values and even values
  {
    for(int PositionPlacement = 0; PositionPlacement <= WaveFormRange; PositionPlacement = PositionPlacement + 2)
    {
      if((PositionPlacement / 2) == 0)
      {
        itoa((WaveFormRangeStart + PositionPlacement), Value, 10);
        CleO.StringExt(FONT_SANS_3, (PlotAreaWidth - YLEGENDBUFFERAMOUNT), (PlotAreaHeight + YPositionCoords), BLACK, MM, 0, 0, Value);
      }
      else if(PositionPlacement == WaveFormLength) //Always plots the end point (ScalePostion * WaveFormLength)
      {
        itoa((WaveFormRangeStart + PositionPlacement), Value, 10);
        CleO.StringExt(FONT_SANS_3, (PlotAreaWidth - YLEGENDBUFFERAMOUNT), (PlotAreaHeight + YPositionCoords), BLACK, MM, 0, 0, Value);
      }
      YPositionCoords = YPositionCoords + ScaleYPosition;
    }
  }
  else if(ScaleYPosition < 12 && ScaleYPosition > 7) //Plots middle and end values
  {
    //First Value
    PositionPlacement = 0;
    itoa((WaveFormRangeStart + PositionPlacement), Value, 10);
    CleO.StringExt(FONT_SANS_3, (PlotAreaWidth - YLEGENDBUFFERAMOUNT), PlotAreaHeight, BLACK, MM, 0, 0, Value);

    //Middle Value
    char MiddleValue [3];
    //Finding the middle position of the range

    int MiddlePositionPlacement = ((WaveFormRangeStart + (WaveFormRange / 2)); //TEST THIS CONVERSION BUT SHOULD BE OK
    itoa((WaveFormRangeStart + PositionPlacement), MiddleValue, 10)
    CleO.StringExt(FONT_SANS_3, (PlotAreaWidth - YLEGENDBUFFERAMOUNT), (PlotAreaHeight + (MiddlePositionPlacement * ScaleYPosition)), BLACK, MM, 0, 0, MiddleValue);

    //End value
    itoa((WaveFormRangeStart + WaveFormRange), Value, 10);
    CleO.StringExt(FONT_SANS_3, (PlotAreaWidth - YLEGENDBUFFERAMOUNT), (PlotAreaHeight + (MiddlePositionPlacement * ScaleYPosition)), BLACK, MM, 0, 0, Value);
  }
  else //Just plots the end values
  {
    //First Value
    PositionPlacement = 0;
    itoa((PositionPlacement + WaveFormRangeStart), Value, 10);
    CleO.StringExt(FONT_SANS_3, (PlotAreaWidth - YLEGENDBUFFERAMOUNT), PlotAreaHeight, BLACK, MM, 0, 0, Value);

    //End value //THIS END SECTION NEEDS FIXING
    itoa((WaveFormRangeStart + WaveformLength), Value, 10);
    CleO.StringExt(FONT_SANS_3, (ScaleXPosition * WaveformLength), PlotAreaWidth + XLEGENDBUFFERAMOUNT, BLACK, MM, 0, 0, Value);
  }

}

void BuildGrid() //unfinished grid function
{
  //X lines
  //will need to match scale in index amount
  for(byte index = 0; index <= WAVEFORMLENGTH; index++)
  {
    //Get CleO Line thickness
    CleO.LineColor(Black);
    CleO.Line((PlotAreaHeight), );
  }

  //Y lines
}

void AddToGraph(int NewValue)
{
  for (int index = WAVEFORMLENGTH; index != 0; index--)
  {
    ShiftRegister[index] = ShiftRegister[index - 1];
  }
  ShiftRegister[0] = NewValue;
}

void PlotGraph()
{
  CleO.LineColor(BLUE);

  for (byte index = 0; index < WAVEFORMLENGTH; index++)
  {
    //CleO.Point(((WaveFormWidth / WAVEFORMLENGTH) * index), ((WaveFormHeight / WAVEFORMLENGTH) * ShiftRegister[index]));
    if (index != 0 && index != WAVEFORMLENGTH)
    {
      CleO.Line((((WaveformWidth / WAVEFORMLENGTH) * (index - 1)) + 25), (((WaveformHeight / WAVEFORMLENGTH) * ShiftRegister[index - 1]) + 25), (((WaveformWidth / WAVEFORMLENGTH) * index) + 25), (((WaveformHeight / WAVEFORMLENGTH) * ShiftRegister[index]) + 25));
    }
  }
}
