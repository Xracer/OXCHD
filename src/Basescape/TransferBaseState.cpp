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
#include "TransferBaseState.h"
#include <sstream>
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Engine/Options.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/SavedGame.h"
#include "../Savegame/Base.h"
#include "../Savegame/Region.h"
#include "../Mod/RuleRegion.h"
#include "TransferItemsState.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Select Destination Base window.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 */
TransferBaseState::TransferBaseState(Base *base) : _base(base)
{
	// Create objects
	int xPos = 700;
	int yPos = 206;

	_window = new Window(this, 570, 200, 0 + xPos, 0 + yPos);
	//_btnCancel = new TextButton(264, 16, 8 + xPos, 116 + yPos);
	_txtTitle = new Text(550, 17, 5 + xPos, 8 + yPos);
	_txtFunds = new Text(250, 11, 5 + xPos, 25 + yPos);
	_txtName = new Text(200, 17, 5 + xPos, 38 + yPos);
	_txtArea = new Text(200, 17, 225 + xPos, 38 + yPos);
	_lstBases = new TextList(550, 160, 5 + xPos, 57 + yPos);

	// Set palette
	setInterface("transferBaseSelect");

	add(_window, "window", "transferBaseSelect");
	//add(_btnCancel, "button", "transferBaseSelect");
	add(_txtTitle, "text", "transferBaseSelect");
	add(_txtFunds, "text", "transferBaseSelect");
	add(_txtName, "text", "transferBaseSelect");
	add(_txtArea, "text", "transferBaseSelect");
	add(_lstBases, "list", "transferBaseSelect");

	//centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK13.SCR"));

	//_btnCancel->setText(tr("STR_CANCEL"));
	//_btnCancel->onMouseClick((ActionHandler)&TransferBaseState::btnCancelClick);
	//_btnCancel->onKeyboardPress((ActionHandler)&TransferBaseState::btnCancelClick, Options::keyCancel);

	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setText(tr("STR_SELECT_DESTINATION_BASE"));

	_txtFunds->setText(tr("STR_CURRENT_FUNDS").arg(Text::formatFunding(_game->getSavedGame()->getFunds())));

	_txtName->setText(tr("STR_NAME"));
	_txtName->setBig();

	_txtArea->setText(tr("STR_AREA"));
	_txtArea->setBig();

	_lstBases->setColumns(2, 220, 150);
	_lstBases->setSelectable(true);
	_lstBases->setBackground(_window);
	_lstBases->setMargin(2);
	_lstBases->onMouseClick((ActionHandler)&TransferBaseState::lstBasesClick);

	int row = 0;
	for (std::vector<Base*>::iterator i = _game->getSavedGame()->getBases()->begin(); i != _game->getSavedGame()->getBases()->end(); ++i)
	{
		if ((*i) != _base)
		{
			// Get area
			std::wstring area;
			for (std::vector<Region*>::iterator j = _game->getSavedGame()->getRegions()->begin(); j != _game->getSavedGame()->getRegions()->end(); ++j)
			{
				if ((*j)->getRules()->insideRegion((*i)->getLongitude(), (*i)->getLatitude()))
				{
					area = tr((*j)->getRules()->getType());
					break;
				}
			}
			std::wostringstream ss;
			ss << L'\x01' << area;
			_lstBases->addRow(2, (*i)->getName().c_str(), ss.str().c_str());
			_bases.push_back(*i);
			row++;
		}
	}
}

/**
 *
 */
TransferBaseState::~TransferBaseState()
{
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 *
void TransferBaseState::btnCancelClick(Action *)
{
	_game->popState();
}
*/

/**
 * Shows the Transfer screen for the selected base.
 * @param action Pointer to an action.
 */
void TransferBaseState::lstBasesClick(Action *)
{
	_game->pushState(new TransferItemsState(_base, _bases[_lstBases->getSelectedRow()]));
}

}
