#pragma once
#include "Exports.h"
#include "Frame.h"
#include "Timer.h"
#include <vector>
#include <algorithm>

using namespace std;

class AQUISTAPACE_API Animation {
private:
	int currentFrame;
	float currentTime;
	float timeLength;
	vector<Frame> frames;
	vector<vector<Frame>> animations;
	int animNum = 0;
public:
	Animation();
	~Animation();
	int getCurrentFrame();
	vector<Frame>& getAnimation();	
	void setAnimNum(int newAnimNum);
	void addFrame(float frameX, float frameY, int spriteWidth, int spriteHeigth, int textureWidth, int textureHeigth);
	void addAnimation(float timeToAnimate);
	void update(Timer& time);
};
