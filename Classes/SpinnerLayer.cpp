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

#include "SpinnerLayer.h"
#include "SimpleAudioEngine.h"

#include <iostream>

USING_NS_CC;

// on "init" you need to initialize your instance
bool SpinnerLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	m_nextResult = { nullptr, nullptr, nullptr };
	m_nextResultFigures = { FigureType::NONE, FigureType::NONE, FigureType::NONE };

	auto figureSprite = Sprite::create("Figures/1.png");
	m_figureHeight = figureSprite->getContentSize().height;
	m_figureWidth = figureSprite->getContentSize().width;

	return true;
}

void SpinnerLayer::setFigures(std::vector<FigureType>& figures)
{
	size_t i = 0;
	for (FigureType figure : figures) {
		const std::string figureFileName = "Figures/" + std::to_string(figure) + ".png";
		auto figureSprite = Sprite::create(figureFileName);
		
		figureSprite->setPosition(m_figureWidth * 0.5f,
			this->getContentSize().height - 20.0f - m_figureHeight * 0.5f - m_figureHeight * i);
		this->addChild(figureSprite);
		m_spritesFigures.push_back(figureSprite);
		++i;
	}
	m_figures = figures;
}

float SpinnerLayer::getFiguresHeight() const
{
	return m_figureHeight * m_figures.size();
}

void SpinnerLayer::prepareNextResult()
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 engine(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, m_figures.size()-1); // define the range

	if (m_nextResult[0] != nullptr) {
		for (size_t i = 0; i < m_nextResult.size(); ++i) {
			this->removeChild(m_nextResult[i]);
			m_nextResult[i] = nullptr;
			m_nextResultFigures[i] = FigureType::NONE;
		}
	}

	for (const auto spriteFigure : m_spritesFigures) {
		spriteFigure->setVisible(true);
	}

	int randomFigure = distr(engine);
	//CCLOG("%i", randomFigure);
	auto figureSpriteTop = Sprite::create("Figures/" + std::to_string(m_figures[randomFigure]) + ".png");
	figureSpriteTop->setPosition(m_figureWidth * 0.5f,
		this->getContentSize().height - 20.0f - m_figureHeight * 0.5f);
	figureSpriteTop->setVisible(false);
	this->addChild(figureSpriteTop);
	m_nextResult[0] = figureSpriteTop;
	m_nextResultFigures[0] = m_figures[randomFigure];

	randomFigure = (randomFigure + 1) % m_figures.size();

	auto figureSpriteMiddle = Sprite::create("Figures/" + std::to_string(m_figures[randomFigure]) + ".png");
	figureSpriteMiddle->setPosition(m_figureWidth * 0.5f,
		this->getContentSize().height - 20.0f - m_figureHeight * 0.5f - m_figureHeight);
	figureSpriteMiddle->setVisible(false);
	this->addChild(figureSpriteMiddle);
	m_nextResult[1] = figureSpriteMiddle;
	m_nextResultFigures[1] = m_figures[randomFigure];

	randomFigure = (randomFigure + 1) % m_figures.size();

	auto figureSpriteBottom = Sprite::create("Figures/" + std::to_string(m_figures[randomFigure]) + ".png");
	figureSpriteBottom->setPosition(m_figureWidth * 0.5f,
		this->getContentSize().height - 20.0f - m_figureHeight * 0.5f - m_figureHeight * 2.0f);
	figureSpriteBottom->setVisible(false);
	this->addChild(figureSpriteBottom);
	m_nextResult[2] = figureSpriteBottom;
	m_nextResultFigures[2] = m_figures[randomFigure];
}


void SpinnerLayer::showResult()
{
	for (const auto spriteFigure : m_spritesFigures) {
		spriteFigure->setVisible(false);
	}

	for (const auto spriteResult : m_nextResult) {
		spriteResult->setVisible(true);
	}
}

const std::vector<FigureType> SpinnerLayer::getResultFigures() const
{
	return m_nextResultFigures;
}