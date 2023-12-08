#include "Animation.h"

Animation::Animation()
{
	currentTime = 0;
	currentFrame = 0;
	timeLength = 200;
}

Animation::~Animation()
{
	if (animations.size() > 0)
		animations.clear();
	if (frames.size() > 0)
		frames.clear();
}

int Animation::getCurrentFrame()
{
	return currentFrame;
}

vector<Frame>& Animation::getAnimation()
{
	if (animNum < animations.size())
	{
		return animations[animNum];
	}
	else
	{
		return animations[animations.size() - 1];
	}
}

void Animation::setAnimNum(int newAnimNum)
{
	animNum = newAnimNum;
}

void Animation::addFrame(float frameX, float frameY, int spriteWidth, int spriteHeigth, int textureWidth, int textureHeigth)
{
	Frame frame;

	frame.coordinates[0].U = ((frameX) / textureWidth);
	frame.coordinates[0].V = ((spriteHeigth + frameY) / textureHeigth);

	frame.coordinates[1].U = (((frameX)+spriteWidth) / textureWidth);
	frame.coordinates[1].V = ((spriteHeigth + frameY) / textureHeigth);

	frame.coordinates[2].U = (((frameX)+spriteWidth) / textureWidth);
	frame.coordinates[2].V = ((frameY) / textureHeigth);

	frame.coordinates[3].U = ((frameX) / textureWidth);
	frame.coordinates[3].V = ((frameY) / textureHeigth);
	
	frames.push_back(frame);
}

void Animation::addAnimation(float timeToAnimate)
{
	timeLength = timeToAnimate;

	if (frames.size() > 0)
	{
		animations.push_back(frames);
		frames.clear();
	}
}

void Animation::update(Timer& timer)
{
	currentTime += (timer.getDT());

	while (currentTime >= timeLength)
	{
		currentTime -= timeLength;
	}

	float frameLength = timeLength / animations[animNum].size();
	currentFrame = static_cast<int>(currentTime / frameLength);
}