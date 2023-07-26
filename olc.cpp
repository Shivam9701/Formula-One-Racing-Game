
#include <iostream>
#include "olcConsoleGameEngine.h"
using namespace std;

class OneLoneCoder_FormulaOLC : public olcConsoleGameEngine
{
public:
	OneLoneCoder_FormulaOLC()
	{
		m_sAppName = L"FORMULA ONE";
	}
	
private:

protected:
	//called by olcConsoleGameEngine
	//wraps up user input and displays it in console
	virtual bool OnUserCreate() {
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) {

		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);

		for (int y = 0; y < ScreenHeight() / 2; y++) {

			for (int x = 0; x < ScreenWidth(); x++) {
				float fMiddlePoint = 0.5f;
				float fRoadWidth = 0.6f;
				float fClipWidth = fRoadWidth * 0.15f;

				fRoadWidth *= 0.5f;//assume road is symmetrical about mid point

				int nLeftGrass = (fMiddlePoint - fRoadWidth - fClipWidth) * ScreenWidth();//left grass starts from x=0 till mid-roadwidth-clipwidth
				int nLeftClip = (fMiddlePoint - fRoadWidth) * ScreenWidth();
				int nRightClip = (fMiddlePoint + fRoadWidth) * ScreenWidth();
				int nRightGrass = (fMiddlePoint + fRoadWidth + fClipWidth) * ScreenWidth();//left grass starts from x=0 till mid-roadwidth-clipwidth


		
				int nRow = y + ScreenHeight() / 2;//since game in lower half of screen
				
				if (x >= 0 && x < nLeftGrass) 
					Draw(x, nRow, PIXEL_SOLID, FG_DARK_GREEN);
				if (x >= nLeftGrass && x < nLeftClip) 
					Draw(x, nRow, PIXEL_SOLID, FG_DARK_RED);
				if (x >= nLeftClip && x < nRightClip) 
					Draw(x, nRow, PIXEL_SOLID, FG_GREY);
				if (x >= nRightClip && x < nRightGrass)
					Draw(x, nRow, PIXEL_SOLID, FG_DARK_RED);
				if (x >= nRightGrass && x<ScreenWidth())
					Draw(x, nRow, PIXEL_SOLID, FG_DARK_GREEN);
				
				


			}
		}

		return true;
	}

};

int main()
{
	OneLoneCoder_FormulaOLC game;
	game.ConstructConsole(80, 25, 1, 1); //{width,height,pixelw,pixelh
	game.Start();

	return 0;
   
}


