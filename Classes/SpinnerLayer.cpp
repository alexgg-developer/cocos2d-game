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

	auto figureSprite = Sprite::create("Figures/1.png");
	m_figureHeight = figureSprite->getContentSize().height;
	m_figureWidth = figureSprite->getContentSize().width;

	return true;
}

void SpinnerLayer::addFigures(const std::vector<FigureType>& figures)
{
	size_t i = 0;
	for (FigureType figure : figures) {
		const std::string figureFileName = "Figures/" + std::to_string(figure) + ".png";
		auto figureSprite = Sprite::create(figureFileName);
		figureSprite->setPosition(50.0f + m_figureWidth * 0.5f,
			this->getContentSize().height - 20.0f - m_figureHeight * 0.5f - m_figureHeight * i);
		this->addChild(figureSprite);
		++i;
	}
}

