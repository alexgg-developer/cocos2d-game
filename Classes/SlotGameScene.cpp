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

#include "SlotGameScene.h"
#include "SimpleAudioEngine.h"

#include <iostream>

USING_NS_CC;

Scene* SlotGameScene::createScene()
{
	return SlotGameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SlotGameSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SlotGameScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto figureSprite = Sprite::create("Figures/1.png");
	m_figureHeight = figureSprite->getContentSize().height;
	m_figureWidth = figureSprite->getContentSize().width;

	m_counter = 0;
	TTFConfig ttfConfig("fonts/arial.ttf", 18);
	m_counterLabel = Label::createWithTTF(ttfConfig, "0 credits", TextHAlignment::CENTER, 200.0f);
	m_counterLabel->setPosition(visibleSize.width - 230.0f, 45.0f);
	m_counterLabel->setTextColor(Color4B::RED);
	addChild(m_counterLabel, 3);

	m_spinButton = ui::Button::create("spin_normal.png", "spin_click.png", "spin_disable.png");
	m_spinButton->setContentSize(Size(250, 50));
	m_spinButton->ignoreContentAdaptWithSize(false);
	m_spinButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			this->spinButtonClick();
			break;
		default:
			break;
		}
	});

	float x = visibleSize.width * 0.5f;
	float y = 45.0f + m_spinButton->getContentSize().height / 2;
	m_spinButton->setPosition(Vec2(x, y));
	this->addChild(m_spinButton, 2);

	auto sprite = Sprite::create("interface.png");
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(sprite, 1);

	initSpinners();
	initCreditTable();

	return true;
}

void SlotGameScene::spinButtonClick()
{
	m_spinButton->setEnabled(false);
	clearMarkPrizes();
	float timeSpinning = m_slotsLayer->spin();
	auto delay = DelayTime::create(timeSpinning);
	auto enableButtonFunction = CallFunc::create(std::bind(&ui::Button::setEnabled, m_spinButton, true));
	auto sequence = Sequence::createWithTwoActions(delay, enableButtonFunction);
	this->runAction(sequence);

	auto calculatePrizesFunction = CallFunc::create(std::bind(&SlotGameScene::calculatePrizes, this));
	auto markPrizesFunction = CallFunc::create(std::bind(&SlotGameScene::markPrizes, this));
	auto sequencePrizes = Sequence::create(delay->clone(), calculatePrizesFunction, markPrizesFunction, nullptr);
	this->runAction(sequencePrizes);
}

void SlotGameScene::initSpinners()
{
	m_slotsLayer = SlotsLayer::create();
	this->addChild(m_slotsLayer, 0);

	std::vector<FigureType> spinner1{ FigureType::ORANGE, FigureType::BELL, FigureType::WATERMELON, FigureType::CHERRY, FigureType::PRUNE, FigureType::LEMON,
		FigureType::GRAPES, FigureType::PRUNE, FigureType::BELL, FigureType::BELL, FigureType::ORANGE, FigureType::GRAPES };
	m_slotsLayer->addSpinner(spinner1);

	std::vector<FigureType> spinner2{ FigureType::WATERMELON, FigureType::CHERRY, FigureType::BELL, FigureType::PRUNE, FigureType::CHERRY, FigureType::GRAPES,
		FigureType::ORANGE, FigureType::LEMON, FigureType::LEMON, FigureType::LEMON, FigureType::CHERRY, FigureType::LEMON, FigureType::PRUNE, FigureType::LEMON,
		FigureType::CHERRY };
	m_slotsLayer->addSpinner(spinner2);

	std::vector<FigureType> spinner3{ FigureType::GRAPES, FigureType::WATERMELON, FigureType::PRUNE, FigureType::GRAPES, FigureType::BELL, FigureType::LEMON,
		FigureType::CHERRY, FigureType::BELL, FigureType::BELL, FigureType::BELL, FigureType::ORANGE, FigureType::ORANGE, FigureType::GRAPES };
	m_slotsLayer->addSpinner(spinner3);

	std::vector<FigureType> spinner4{ FigureType::LEMON, FigureType::PRUNE, FigureType::PRUNE, FigureType::LEMON, FigureType::GRAPES, FigureType::ORANGE,
		FigureType::WATERMELON, FigureType::WATERMELON, FigureType::BELL, FigureType::CHERRY, FigureType::CHERRY, FigureType::LEMON, FigureType::ORANGE,
		FigureType::PRUNE, FigureType::LEMON };
	m_slotsLayer->addSpinner(spinner4);

	std::vector<FigureType> spinner5{ FigureType::GRAPES, FigureType::CHERRY, FigureType::BELL, FigureType::WATERMELON, FigureType::ORANGE, FigureType::ORANGE,
		FigureType::PRUNE, FigureType::PRUNE, FigureType::ORANGE, FigureType::ORANGE, FigureType::GRAPES, FigureType::BELL, FigureType::WATERMELON,
		FigureType::CHERRY };
	m_slotsLayer->addSpinner(spinner5);
}

void SlotGameScene::initCreditTable()
{
	m_creditTable[FigureType::BELL]			= { 50, 75, 100 };
	m_creditTable[FigureType::WATERMELON]	= { 20, 30, 60 };
	m_creditTable[FigureType::GRAPES]		= { 10, 20, 50 };
	m_creditTable[FigureType::PRUNE]		= { 10, 20, 40 };
	m_creditTable[FigureType::ORANGE]		= { 10, 15, 30 };
	m_creditTable[FigureType::LEMON]		= { 5, 10, 20 };
	m_creditTable[FigureType::CHERRY]		= { 2, 5, 10 };
}

void SlotGameScene::calculatePrizes()
{
	const auto& prizes = m_slotsLayer->getPrizes();
	for (const auto& prize : prizes) {
		size_t currentCredits = m_creditTable[prize.type][prize.positions.size() - 2];
		m_counter += currentCredits;
	}

	m_counterLabel->setString(std::to_string(m_counter) + " credits");
}



void SlotGameScene::markPrizes()
{
	const auto& prizes = m_slotsLayer->getPrizes();
		
	Color4F transparent(1, 1, 1, 0);
	Vec2 rectangle[4];
	for (const auto& prize : prizes) {
		auto firstPosition = prize.positions[0];
		auto lastPosition = prize.positions[prize.positions.size() - 1];
		size_t positions = lastPosition.first - firstPosition.first;

		auto rectNode = DrawNode::create();
		auto nodeWidth = m_figureWidth + SlotsLayer::MARGIN_BETWEEN_SPINNERS * positions;
		float halfWidth = nodeWidth * 0.5f;
		float halfHeight = m_figureHeight * 0.5f;

		rectangle[0] = Vec2(-halfWidth, -halfHeight);
		rectangle[1] = Vec2(halfWidth, -halfHeight);
		rectangle[2] = Vec2(halfWidth, halfHeight);
		rectangle[3] = Vec2(-halfWidth, halfHeight);		

		rectNode->setAnchorPoint(Vec2(0, 0.5f));
		rectNode->drawPolygon(rectangle, 4, transparent, 2, Color4F::RED);
		auto xRect = 50.0f + halfWidth + SlotsLayer::MARGIN_BETWEEN_SPINNERS * firstPosition.first;
		auto yRect = this->getContentSize().height - 20.0f - halfHeight - m_figureHeight * firstPosition.second;
		rectNode->setPosition(xRect, yRect);
		this->addChild(rectNode, 1);
		m_prizeRects.push_back(rectNode);

		auto credits = m_creditTable[prize.type][prize.positions.size() - 2];
		TTFConfig ttfConfig("fonts/arial.ttf", 18);
		auto prizeLabel = Label::createWithTTF(ttfConfig, std::to_string(credits) + " credits", TextHAlignment::CENTER, 200.0f);
		prizeLabel->setPosition(xRect + SlotsLayer::MARGIN_BETWEEN_SPINNERS, yRect + halfHeight + 10.0f);
		prizeLabel->setTextColor(Color4B::RED);
		this->addChild(prizeLabel, 1);
		m_prizeLabels.push_back(prizeLabel);
	}	
}

void SlotGameScene::clearMarkPrizes()
{
	for (auto prizeRect : m_prizeRects) {
		this->removeChild(prizeRect);
	}
	m_prizeRects.clear();

	for (auto prizeLabel : m_prizeLabels) {
		this->removeChild(prizeLabel);
	}
	m_prizeLabels.clear();
}