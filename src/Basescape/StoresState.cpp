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
#include "StoresState.h"
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
#include "../Mod/RuleItem.h"
#include "../Savegame/ItemContainer.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Stores window.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 */
StoresState::StoresState(Base *base) : _base(base)
{
	// Create objects
	_window = new Window(this, 550, 305, 700, 440, POPUP_BOTH);
	_txtTitle = new Text(440, 17, 705, 443);
	_txtItem = new Text(142, 11, 705, 462);
	_txtQuantity = new Text(88, 11, 895, 462);
	_txtSpaceUsed = new Text(74, 11, 1020, 462);
	_lstStores = new TextList(508, 230, 705, 476);

	// Set palette
	setInterface("hdbaseInfo");

	add(_window, "window", "hdbaseInfo");
	add(_txtTitle, "title", "hdbaseInfo");
	add(_txtItem, "text1", "hdbaseInfo");
	add(_txtQuantity, "text1", "hdbaseInfo");
	add(_txtSpaceUsed, "text1", "hdbaseInfo");
	add(_lstStores, "list", "hdbaseInfo");

	//centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK13.SCR"));
	_window->setThinBorder();

	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_LEFT);
	_txtTitle->setText(tr("STR_STORES"));

	_txtItem->setText(tr("STR_ITEM"));

	_txtQuantity->setText(tr("STR_QUANTITY_UC"));

	_txtSpaceUsed->setText(tr("STR_SPACE_USED_UC"));

	_lstStores->setColumns(3, 220, 120, 40);
	_lstStores->setSelectable(true);
	_lstStores->setBackground(_window);
	_lstStores->setMargin(2);

	const std::vector<std::string> &items = _game->getMod()->getItemsList();
	for (std::vector<std::string>::const_iterator i = items.begin(); i != items.end(); ++i)
	{
		int qty = _base->getStorageItems()->getItem(*i);
		if (qty > 0)
		{
			RuleItem *rule = _game->getMod()->getItem(*i, true);
			std::wostringstream ss, ss2;
			ss << qty;
			ss2 << qty * rule->getSize();
			_lstStores->addRow(3, tr(*i).c_str(), ss.str().c_str(), ss2.str().c_str());
		}
	}
}

/**
 *
 */
StoresState::~StoresState()
{

}

}
