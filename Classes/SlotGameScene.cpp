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
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	m_spinButton = ui::Button::create("spin_normal.png", "spin_click.png", "spin_disable.png");
	//m_spinButton->setSize(Size(250, 50));
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

	if (m_spinButton == nullptr ||
		m_spinButton->getContentSize().width <= 0 ||
		m_spinButton->getContentSize().height <= 0)
	{
		problemLoading("problem with images");
	}
	else
	{
		//float width = m_spinButton->getContentSize().width;
		//float height = m_spinButton->getContentSize().height;
		float x = visibleSize.width * 0.5f;
		float y = 45 + m_spinButton->getContentSize().height / 2;
		m_spinButton->setPosition(Vec2(x, y));
	}

	this->addChild(m_spinButton, 1);

	// add "SlotGameScene" splash screen"
	auto sprite = Sprite::create("interface.png");
	if (sprite == nullptr)
	{
		problemLoading("'interface.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}
	return true;
}

void SlotGameScene::spinButtonClick()
{
	std::cout << "spin!" << std::endl;
	m_spinButton->setEnabled(false);
}
