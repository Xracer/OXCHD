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

	// Create objects
	_window = new Window(this, 550, 150, 700, 588, POPUP_BOTH);
	_txtTitle = new Text(440, 17, 705, 594);
	_txtItem = new Text(114, 11, 705, 615);
	_txtQuantity = new Text(65, 11, 895, 615);
	_txtArrivalTime = new Text(118, 11, 1010, 615);
	_lstTransfers = new TextList(273, 112, 705, 629);

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
	_txtTitle->setAlign(ALIGN_LEFT);
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
