
#include <iostream>
#include "olcConsoleGameEngine.h"
using namespace std;

class FormulaOne : public olcConsoleGameEngine
{
public:
	FormulaOne()
	{
		m_sAppName = L"FORMULA ONE";
	}
	
private:
	float fCarPos = 0.0f;
	float fDistance = 0.0f;

protected:
	//called by olcConsoleGameEngine
	//wraps up user input and displays it in console
	virtual bool OnUserCreate() {
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) {

		if (m_keys[VK_UP].bHeld)
			fDistance += 80.0f * fElapsedTime;

		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);

		for (int y = 0; y < ScreenHeight() / 2; y++) {

			for (int x = 0; x < ScreenWidth(); x++) {
		
				int nRow = y + ScreenHeight() / 2;//since game in lower half of screen
				
				float fPerspective = float(y) / (ScreenHeight() / 2.0f);//0 at top 1 at bottom

				float fMiddlePoint = 0.5f;
				float fRoadWidth = 0.1f +0.8f*fPerspective; // at top 10% of screen and bottom 90%
				float fClipWidth = fRoadWidth * 0.15f;

				fRoadWidth *= 0.5f;//assume road is symmetrical about mid point

				int nLeftGrass = (fMiddlePoint - fRoadWidth - fClipWidth) * ScreenWidth();//left grass starts from x=0 till mid-roadwidth-clipwidth
				int nLeftClip = (fMiddlePoint - fRoadWidth) * ScreenWidth();
				int nRightClip = (fMiddlePoint + fRoadWidth) * ScreenWidth();
				int nRightGrass = (fMiddlePoint + fRoadWidth + fClipWidth) * ScreenWidth();//left grass starts from x=0 till mid-roadwidth-clipwidth


				// Using periodic oscillatory functions to give lines, where the phase is controlled
				// by the distance around the track. These take some fine tuning to give the right "feel
				int nGrassColour = sinf(20.0f * powf(1.0f - fPerspective, 3) + fDistance * 0.1f) > 0.0f ? FG_GREEN : FG_DARK_GREEN;
				int nClipColour = sinf(80.0f * powf(1.0f - fPerspective, 2) + fDistance) > 0.0f ? FG_RED : FG_WHITE;
				
				if (x >= 0 && x < nLeftGrass) 
					Draw(x, nRow, PIXEL_SOLID, nGrassColour);
				if (x >= nLeftGrass && x < nLeftClip) 
					Draw(x, nRow, PIXEL_SOLID, nClipColour);
				if (x >= nLeftClip && x < nRightClip) 
					Draw(x, nRow, PIXEL_SOLID, FG_GREY);
				if (x >= nRightClip && x < nRightGrass)
					Draw(x, nRow, PIXEL_SOLID, nClipColour);
				if (x >= nRightGrass && x<ScreenWidth())
					Draw(x, nRow, PIXEL_SOLID, nGrassColour);

			}
		}

		//draw car

		int nCarPos = ScreenWidth() / 2 + ((int)(ScreenWidth() * fCarPos) / 2.0f)-7;

		DrawStringAlpha(nCarPos, 80, L"   ||####||   ", 0x0011);
		DrawStringAlpha(nCarPos, 81, L"      ##      ", 0x1001);
		DrawStringAlpha(nCarPos, 82, L"     ####     ", 0x0101);
		DrawStringAlpha(nCarPos, 83, L"     ####     ", 0x0101);
		DrawStringAlpha(nCarPos, 84, L"|||  ####  |||", 0x0001);
		DrawStringAlpha(nCarPos, 85, L"|||########|||", 0x0001);
		DrawStringAlpha(nCarPos, 86, L"|||  ####  |||", 0x0001);
		
		return true;
	}
	
};

int main()
{
	FormulaOne game;
	game.ConstructConsole(160, 100, 4, 4); //{width,height,pixelw,pixelh
	game.Start();

	return 0;
   
}


