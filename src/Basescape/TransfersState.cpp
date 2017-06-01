/*
 * Copyright 2010-2017 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "TransfersState.h"
#include <sstream>
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Engine/Options.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Base.h"
#include "../Savegame/Transfer.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Transfers window.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 */
TransfersState::TransfersState(Base *base) : _base(base)
{
	_screen = false;
	int xPos = 700;
	int yPos = 3;

	// Create objects
	_window = new Window(this, 570, 200, 0 + xPos, 0 + yPos, POPUP_BOTH);
	_txtTitle = new Text(550, 17, 5 + xPos, 7 + yPos);
	_txtItem = new Text(114, 11, 5 + xPos, 26 + yPos);
	_txtQuantity = new Text(65, 11, 195 + xPos, 26 + yPos);
	_txtArrivalTime = new Text(118, 11, 310 + xPos, 26 + yPos);
	_lstTransfers = new TextList(550, 112, 5 + xPos, 40 + yPos);

	// Set palette
	setInterface("hdbaseInfo");

	add(_window, "window", "hdbaseInfo");
	add(_txtTitle, "title", "hdbaseInfo");
	add(_txtItem, "text1", "hdbaseInfo");
	add(_txtQuantity, "text1", "hdbaseInfo");
	add(_txtArrivalTime, "text1", "hdbaseInfo");
	add(_lstTransfers, "list", "hdbaseInfo");

	//centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK13.SCR"));
	_window->setThinBorder();

	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setText(tr("STR_TRANSFERS"));

	_txtItem->setText(tr("STR_ITEM"));

	_txtQuantity->setText(tr("STR_QUANTITY_UC"));

	_txtArrivalTime->setText(tr("STR_ARRIVAL_TIME_HOURS"));

	_lstTransfers->setColumns(3, 220, 120, 40);
	_lstTransfers->setSelectable(true);
	_lstTransfers->setBackground(_window);
	_lstTransfers->setMargin(2);

	for (std::vector<Transfer*>::iterator i = _base->getTransfers()->begin(); i != _base->getTransfers()->end(); ++i)
	{
		std::wostringstream ss, ss2;
		ss << (*i)->getQuantity();
		ss2 << (*i)->getHours();
		_lstTransfers->addRow(3, (*i)->getName(_game->getLanguage()).c_str(), ss.str().c_str(), ss2.str().c_str());
	}
}

/**
 *
 */
TransfersState::~TransfersState()
{

}

}
