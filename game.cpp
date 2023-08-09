
#include <iostream>
#include<string>
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
	float fSpeed = 0.0f;

	float fCurvature = 0.0f; // curvature of track at any given time

	float fTrackCurvature = 0.0f; //curvature of single particle moving along track

	float fPlayerCurvature = 0.0f; //curvature of player dependent upon th input he gives

	float fTrackDistance = 0.0f;
	 
	float fCurrentLapTime = 0.0f;
	vector<pair<float, float>> vecTrack; //curvature,distance
	list<float> listLapTimes;

protected:

	//called by olcConsoleGameEngine
	//wraps up user input and displays it in console

	virtual bool OnUserCreate() {

		vecTrack.push_back({ 0.0f, 50.0f }); // initial/final track is 0 curvature for 10 units
		vecTrack.push_back({ 0.0f, 200.0f });
		vecTrack.push_back({ 1.0f, 300.0f });
		vecTrack.push_back({ 0.0f, 100.0f });
		vecTrack.push_back({ -1.0f, 200.0f });
		vecTrack.push_back({ 0.0f, 100.0f });
		vecTrack.push_back({ -1.0f, 200.0f });
		vecTrack.push_back({ 0.0f, 100.0f });
		vecTrack.push_back({ 1.0f, 500.0f });
		vecTrack.push_back({ 0.0f, 200.0f });
		vecTrack.push_back({ -1.0f, 100.0f });
		vecTrack.push_back({ 0.0f, 200.0f });
		vecTrack.push_back({ 1.0f, 300.0f });
		vecTrack.push_back({ 0.0f, 100.0f });

		for (auto t : vecTrack)
			fTrackDistance += t.second;
		
		listLapTimes = { 0,0,0,0,0 };
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) {

		// if up arrow pressed increase speed by acc * time (acc=2)
		if (m_keys[VK_UP].bHeld)
			fSpeed += 2.0f * fElapsedTime;
		else
			fSpeed -= 1.0f * fElapsedTime; // speed down at 1 acc
		
		int nCarDirection = 0;

		//if left arrow is pressed add player curvature
		if (m_keys[VK_LEFT].bHeld) {
			nCarDirection = -1;
			fPlayerCurvature -= 0.85f * fElapsedTime;
		}

		// for right key pressed- add curvature
		if (m_keys[VK_RIGHT].bHeld) {
			nCarDirection = 1;
			fPlayerCurvature += 0.85f * fElapsedTime;
		}

	    //slow down car if player is out of track

		if (fabs(fPlayerCurvature - fTrackCurvature) >= 0.8f)
			fSpeed -= 5.0f * fElapsedTime;

		// clamp speed to 0 ,1
		if (fSpeed < 0.0f) fSpeed = 0.0f;
		if (fSpeed > 1.0f)fSpeed = 1.0f;

		fDistance += (70.0f * fSpeed) * fElapsedTime;

		//Get Point on Track
		float fOffset = 0;
		int nTrackSection = 0;

		//time the lap
		fCurrentLapTime += fElapsedTime;

		// if track already covered loop around the track
		if (fDistance >= fTrackDistance) {
			fDistance -= fTrackDistance;
			listLapTimes.push_front(fCurrentLapTime);
			listLapTimes.pop_back();
			fCurrentLapTime = 0.0f;
		}

		// find which track you are at using fDistance (the distance already travelled)

		while (nTrackSection < vecTrack.size() && fOffset <= fDistance) {

			fOffset += vecTrack[nTrackSection].second;
			nTrackSection++;
		}

		float fTargetCurvature = vecTrack[nTrackSection - 1].first;

		//interpolate fCurvature (curv. at any point) to targetCurvature slowly using elapsed time for better smooth turning transition

		// if speed =0 no change in curvature else slowly interpolate to fcurvature
		float fTrackCurveDiff = (fTargetCurvature - fCurvature) * fElapsedTime * fSpeed;
		fCurvature += fTrackCurveDiff;

		fTrackCurvature += (fCurvature)* fElapsedTime* fSpeed;


		//// Draw Sky - light blue and dark blue
		for (int y = 0; y < ScreenHeight() / 2; y++)
			for (int x = 0; x < ScreenWidth(); x++) {
				if (y<ScreenHeight()/4)
				Draw(x, y, PIXEL_SOLID, FG_DARK_BLUE);
		     else
				Draw(x, y, PIXEL_SOLID, FG_BLUE);
			}


		// Draw Scenery - hills are a rectified sine wave, where the phase is adjusted by the
		// accumulated track curvature
		for (int x = 0; x < ScreenWidth(); x++)
		{
			int nHillHeight = (int)(fabs(sinf(x * 0.01f + fTrackCurvature) * 16.0f));
			for (int y = (ScreenHeight() / 2) - nHillHeight; y < ScreenHeight() / 2; y++)
				Draw(x, y, PIXEL_SOLID, FG_DARK_YELLOW);
		}
		
		for (int y = 0; y < ScreenHeight() / 2; y++) {

			for (int x = 0; x < ScreenWidth(); x++) {

				int nRow = y + ScreenHeight() / 2;//since game in lower half of screen

				float fPerspective = float(y) / (ScreenHeight() / 2.0f);//0 at top 1 at bottom

				//change middle point to  suit perspective the function is choen after some hit and trial cases

				float fMiddlePoint = 0.5f + fCurvature * powf((1.0f - fPerspective), 3);
				;
				float fRoadWidth = 0.1f + 0.8f * fPerspective; // at top 10% of screen and bottom 90%
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

				// Start finish straight changes the road colour to inform the player lap is reset
				int nRoadColour = (nTrackSection - 1) == 0 ? FG_WHITE : FG_GREY;


				//Draw The Track
				if (x >= 0 && x < nLeftGrass)
					Draw(x, nRow, PIXEL_SOLID, nGrassColour);
				if (x >= nLeftGrass && x < nLeftClip)
					Draw(x, nRow, PIXEL_SOLID, nClipColour);
				if (x >= nLeftClip && x < nRightClip)
					Draw(x, nRow, PIXEL_SOLID, nRoadColour);
				if (x >= nRightClip && x < nRightGrass)
					Draw(x, nRow, PIXEL_SOLID, nClipColour);
				if (x >= nRightGrass && x < ScreenWidth())
					Draw(x, nRow, PIXEL_SOLID, nGrassColour);

			}
		}

		//draw car
		fCarPos = fPlayerCurvature - fTrackCurvature;
		int nCarPos = ScreenWidth() / 2 + ((int)(ScreenWidth() * fCarPos) / 2.0f) - 7;

		

		// Draw a car that represents what the player is doing. Apologies for the quality
		// of the sprite... :-(
		switch (nCarDirection)
		{
		case 0:
			DrawStringAlpha(nCarPos, 80, L"   ||####||   ", 0x0011);
			DrawStringAlpha(nCarPos, 81, L"      ##      ", 0x1001);
			DrawStringAlpha(nCarPos, 82, L"     ####     ", 0x0101);
			DrawStringAlpha(nCarPos, 83, L"     ####     ", 0x0101);
			DrawStringAlpha(nCarPos, 84, L"|||  ####  |||", 0x0001);
			DrawStringAlpha(nCarPos, 85, L"|||########|||", 0x0001);
			DrawStringAlpha(nCarPos, 86, L"|||  ####  |||", 0x0001);
			break;

		case +1:
			DrawStringAlpha(nCarPos, 80, L"      //####//", 0x0011);
			DrawStringAlpha(nCarPos, 81, L"         ##   ", 0x1001);
			DrawStringAlpha(nCarPos, 82, L"       ####   ", 0x0101);
			DrawStringAlpha(nCarPos, 83, L"      ####    ", 0x0101);
			DrawStringAlpha(nCarPos, 84, L"///  ####//// ", 0x0001);
			DrawStringAlpha(nCarPos, 85, L"//#######///O ", 0x0001);
			DrawStringAlpha(nCarPos, 86, L"/// #### //// ", 0x0001);
			break;

		case -1:
			DrawStringAlpha(nCarPos, 80, L"\\\\####\\\\      ", 0x0011);
			DrawStringAlpha(nCarPos, 81, L"   ##         ", 0x1001);
			DrawStringAlpha(nCarPos, 82, L"   ####       ", 0x0101);
			DrawStringAlpha(nCarPos, 83, L"    ####      ", 0x0101);
			DrawStringAlpha(nCarPos, 84, L" \\\\\\\\####  \\\\\\", 0x0001);
			DrawStringAlpha(nCarPos, 85, L" O\\\\\\#######\\\\", 0x0001);
			DrawStringAlpha(nCarPos, 86, L" \\\\\\\\ #### \\\\\\", 0x0001);
			break;
		}


		//draw stats
		DrawString(0, 0, L"Distance: " + to_wstring(fDistance));
		DrawString(0, 1, L"Target Curvature: " + to_wstring(fCurvature));
		DrawString(0, 2, L"Player Curvature: " + to_wstring(fPlayerCurvature));
		DrawString(0, 3, L"Player Speed    : " + to_wstring(fSpeed));
		DrawString(0, 4, L"Track Curvature : " + to_wstring(fTrackCurvature));

		//lambda function [capture extern] for time
		auto disp_time = [](float t) {
			int nMinutes = t / 60.0f;
			int nSeconds = t - (nMinutes * 60);
			int nMilliSeconds = (t - (float)nSeconds) * 1000.0f;
			return to_wstring(nMinutes) + L"." + to_wstring(nSeconds) + L":" + to_wstring(nMilliSeconds);
		};

		DrawString(10, 8, disp_time(fCurrentLapTime));

		//display last 5 lap times
		int j = 10;
		for (auto l : listLapTimes)
		{
			DrawString(10, j, disp_time(l));
			j++;
		}

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