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
#include "SoldierMemorialState.h"
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
#include "../Savegame/Soldier.h"
#include "../Savegame/SoldierDeath.h"
#include "../Savegame/GameTime.h"
#include "SoldierInfoState.h"
#include "../Menu/StatisticsState.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Soldier Memorial screen.
 * @param game Pointer to the core game.
 */
SoldierMemorialState::SoldierMemorialState()
{

	// Create objects
	_window = new Window(this, 550, 265, 700, 271);
	//_btnStatistics = new TextButton(148, 16, 8, 176);
	_txtTitle = new Text(545, 17, 705, 274); 
	_txtName = new Text(150, 11, 710, 308);
	_txtRank = new Text(100, 11, 900, 308);
	_txtDate = new Text(100, 11, 1040, 308);
	_txtRecruited = new Text(160, 11, 710, 294);
	_txtLost = new Text(150, 11, 900, 294);
	_lstSoldiers = new TextList(515, 250, 700, 323);

	// Set palette
	setInterface("soldierMemorial");

	add(_window, "window", "soldierList");
	//add(_btnStatistics, "button", "soldierMemorial");
	add(_txtTitle, "text", "soldierMemorial");
	add(_txtName, "text", "soldierMemorial");
	add(_txtRank, "text", "soldierMemorial");
	add(_txtDate, "text", "soldierMemorial");
	add(_txtRecruited, "text", "soldierMemorial");
	add(_txtLost, "text", "soldierMemorial");
	add(_lstSoldiers, "list", "soldierMemorial");

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK02.SCR"));
	_window->setThinBorder();

	//_btnStatistics->setText(tr("STR_STATISTICS"));
	//_btnStatistics->onMouseClick((ActionHandler)&SoldierMemorialState::btnStatisticsClick);

	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setText(tr("STR_MEMORIAL"));

	_txtName->setText(tr("STR_NAME_UC"));

	_txtRank->setText(tr("STR_RANK"));

	_txtDate->setText(tr("STR_DATE_UC"));

	size_t lost = _game->getSavedGame()->getDeadSoldiers()->size();
	size_t recruited = lost;
	for (std::vector<Base*>::iterator i = _game->getSavedGame()->getBases()->begin(); i != _game->getSavedGame()->getBases()->end(); ++i)
	{
		recruited += (*i)->getTotalSoldiers();
	}

	_txtRecruited->setText(tr("STR_SOLDIERS_RECRUITED_UC").arg(recruited));

	_txtLost->setText(tr("STR_SOLDIERS_LOST_UC").arg(lost));

	_lstSoldiers->setColumns(5, 114, 88, 30, 25, 35);
	_lstSoldiers->setSelectable(true);
	_lstSoldiers->setBackground(_window);
	_lstSoldiers->setMargin(8);
	_lstSoldiers->onMouseClick((ActionHandler)&SoldierMemorialState::lstSoldiersClick);

	for (std::vector<Soldier*>::reverse_iterator i = _game->getSavedGame()->getDeadSoldiers()->rbegin(); i != _game->getSavedGame()->getDeadSoldiers()->rend(); ++i)
	{
		SoldierDeath *death = (*i)->getDeath();

		std::wostringstream saveDay, saveMonth, saveYear;
		saveDay << death->getTime()->getDayString(_game->getLanguage());
		saveMonth << tr(death->getTime()->getMonthString());
		saveYear << death->getTime()->getYear();
		_lstSoldiers->addRow(5, (*i)->getName().c_str(), tr((*i)->getRankString()).c_str(), saveDay.str().c_str(), saveMonth.str().c_str(), saveYear.str().c_str());
	}
}

/**
 *
 */
SoldierMemorialState::~SoldierMemorialState()
{

}

/**
* Shows the Statistics screen.
* @param action Pointer to an action.

void SoldierMemorialState::btnStatisticsClick(Action *)
{
	_game->pushState(new StatisticsState);
}*/

/**
 * Shows the selected soldier's info.
 * @param action Pointer to an action.
 */
void SoldierMemorialState::lstSoldiersClick(Action *)
{
	_game->pushState(new SoldierInfoState(0, _lstSoldiers->getSelectedRow()));
}

}
