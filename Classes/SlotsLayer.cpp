/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "SlotsLayer.h"
#include "Figure.h"

#include "SimpleAudioEngine.h"

#include <iostream>

USING_NS_CC;


const float SlotsLayer::MARGIN_BETWEEN_SPINNERS = 240.0f;

bool SlotsLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	return true;
}

std::vector<FigurePrize> SlotsLayer::getPrizes()
{
	return m_prizes;
}

void SlotsLayer::addSpinner(std::vector<FigureType>& figures)
{
	SpinnerLayer* newSlotSpinLayer = SpinnerLayer::create();
	newSlotSpinLayer->setFigures(figures);
	newSlotSpinLayer->setPosition(50.0f + MARGIN_BETWEEN_SPINNERS * m_slotsSpinLayers.size(), 0.0f);
	m_slotsSpinLayers.push_back(newSlotSpinLayer);
	this->addChild(newSlotSpinLayer);
}

float SlotsLayer::spin()
{
	
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine(rd()); // seed the generator
	std::uniform_real_distribution<> distr(2.0f, 4.0f); // define the range
	float timeSpinning = distr(engine);
	const float delayTime = 0.1f;
	for (size_t i = 0; i < m_slotsSpinLayers.size(); ++i) {
		spinActions(m_slotsSpinLayers[i], timeSpinning, delayTime * i);
	}
	auto calculatePrizeFunction = CallFunc::create(std::bind(&SlotsLayer::calculatePrize, this));
	auto totalTime = timeSpinning + delayTime * (m_slotsSpinLayers.size() - 1);
	auto initialDelay = DelayTime::create(totalTime);
	this->runAction(Sequence::create(initialDelay->clone(), calculatePrizeFunction, nullptr));
	return totalTime;
}

void SlotsLayer::spinActions(SpinnerLayer * const spinner, float timeSpinning, float delayTime)
{
	auto position = spinner->getPosition();
	float figuresHeight = spinner->getFiguresHeight();
	//float timeSpinning = 1.0f;
	const float timeOneSpin = 0.05f;
	size_t totalSpins = static_cast<size_t> (timeSpinning / (timeOneSpin * 2.0f));
	auto initialDelay = DelayTime::create(delayTime);
	auto moveDown = MoveBy::create(timeOneSpin, Vec2(0, figuresHeight));
	auto moveUp = moveDown->reverse();

	auto spinnerSequence = Sequence::create(moveDown, moveUp, nullptr);
	auto repeatSpinnerSequence = Repeat::create(spinnerSequence->clone(), totalSpins);
	auto callFunction = CallFunc::create(std::bind(&SpinnerLayer::showResult, spinner));
	auto totalSequence = Sequence::create(initialDelay, repeatSpinnerSequence, callFunction, nullptr);
	spinner->runAction(totalSequence);

	auto prepareNextResultFunction = CallFunc::create(std::bind(&SpinnerLayer::prepareNextResult, spinner));
	this->runAction(Sequence::create(initialDelay->clone(), prepareNextResultFunction, nullptr));
}

void SlotsLayer::calculatePrize()
{
	std::vector<std::vector<FigureType>> results;
	for (const auto spinnerLayer : m_slotsSpinLayers) {
		results.push_back(spinnerLayer->getResultFigures());
	}

	/*std::vector<std::string> rows;
	for (size_t j = 0; j < results[0].size(); ++j) {
		std::string row = std::to_string(results[0][j]);
		for (size_t i = 1; i < results.size(); ++i) {
			row = row + " " + std::to_string(results[i][j]);
		}
		rows.push_back(row);
	}*/

	m_prizes.clear();
	for (size_t j = 0; j < results[0].size(); ++j) {
		FigurePrize currentPrize;
		currentPrize.type = results[0][j];
		currentPrize.positions.push_back(std::make_pair(0, j));
		for (size_t i = 1; i < results.size(); ++i) {
			if (currentPrize.type != results[i][j]) {
				//save if there is prize (more or equal 2 continous pattern)
				if (currentPrize.positions.size() >= 2) {
					m_prizes.push_back(currentPrize);
				}
				currentPrize.type = results[i][j];
				currentPrize.positions.clear();
			}
			//if there is more than 4 counts as 4.
			if (currentPrize.positions.size() < 5) {
				currentPrize.positions.push_back(std::make_pair(i, j));
			}
		}
		//Finishing case without change of pattern
		if (currentPrize.positions.size() >= 2) {
			m_prizes.push_back(currentPrize);
		}
	}

}




