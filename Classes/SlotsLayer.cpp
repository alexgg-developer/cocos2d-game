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

void SlotsLayer::addSpinner(const std::vector<FigureType>& figures)
{
	SpinnerLayer* newSlotSpinLayer = SpinnerLayer::create();
	newSlotSpinLayer->addFigures(figures);
	m_slotsSpinLayers.push_back(newSlotSpinLayer);
	this->addChild(newSlotSpinLayer);
}

void SlotsLayer::spin()
{
	auto position = m_slotsSpinLayers[0]->getPosition();
	m_slotsSpinLayers[0]->setPosition(position.x , position.y + 212.0f);
}
