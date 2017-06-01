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
#include "SoldiersState.h"
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Engine/Options.h"
#include "../Geoscape/AllocatePsiTrainingState.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Base.h"
#include "../Savegame/Soldier.h"
#include "SoldierInfoState.h"
#include "SoldierDiaryOverviewState.h"
#include "SoldierDiaryMissionState.h"
#include "SoldierDiaryPerformanceState.h"
#include "SoldierMemorialState.h"
#include "../Engine/MultiState.h"

#include "../Savegame/Craft.h"
#include "../Mod/RuleCraft.h"
#include "../Savegame/ItemContainer.h"
#include "../Savegame/Vehicle.h"
#include "../Savegame/SavedGame.h"
#include "../Menu/ErrorMessageState.h"
#include "../Battlescape/InventoryState.h"
#include "../Battlescape/BattlescapeGenerator.h"
#include "../Savegame/SavedBattleGame.h"
#include "../Mod/RuleInterface.h"


namespace OpenXcom
{

/**
 * Initializes all the elements in the Soldiers screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 */
SoldiersState::SoldiersState(Base *base) : _base(base)
{
	bool isPsiBtnVisible = Options::anytimePsiTraining && _base->getAvailablePsiLabs() > 0;

	// Create objects
	_window = new Window(this, 550, 265, 700, 3);

	_txtTitle = new Text(545, 17, 705, 13);
	_txtName = new Text(150, 11, 710, 30);
	_txtRank = new Text(100, 11, 900, 30);
	_txtCraft = new Text(100, 11, 1040, 30);
	_lstSoldiers = new TextList(515, 250, 700, 45);

	// Set palette
	setInterface("soldierList");

	add(_window, "window", "soldierList");
	add(_txtTitle, "text1", "soldierList");
	add(_txtName, "text2", "soldierList");
	add(_txtRank, "text2", "soldierList");
	add(_txtCraft, "text2", "soldierList");
	add(_lstSoldiers, "list", "soldierList");

	//centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK02.SCR"));
	_window->setThinBorder();

	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setText(tr("STR_SOLDIER_LIST"));

	_txtName->setText(tr("STR_NAME_UC"));

	_txtRank->setText(tr("STR_RANK"));

	_txtCraft->setText(tr("STR_CRAFT"));

	_lstSoldiers->setColumns(3, 190, 140, 170);
	_lstSoldiers->setSelectable(true);
	_lstSoldiers->setBackground(_window);
	_lstSoldiers->setMargin(8);
	_lstSoldiers->onMouseClick((ActionHandler)&SoldiersState::lstSoldiersClick);
}

/**
 *
 */
SoldiersState::~SoldiersState()
{

}

/**
 * Updates the soldiers list
 * after going to other screens.
 */
void SoldiersState::init()
{
	State::init();
	unsigned int row = 0;
	_lstSoldiers->clearList();
	for (std::vector<Soldier*>::iterator i = _base->getSoldiers()->begin(); i != _base->getSoldiers()->end(); ++i)
	{
		_lstSoldiers->addRow(3, (*i)->getName(true).c_str(), tr((*i)->getRankString()).c_str(), (*i)->getCraftString(_game->getLanguage()).c_str());
		if ((*i)->getCraft() == 0)
		{
			_lstSoldiers->setRowColor(row, _lstSoldiers->getSecondaryColor());
		}
		row++;
	}
	if (row > 0 && _lstSoldiers->getScroll() >= row)
	{
		_lstSoldiers->scrollTo(0);
	}
}

/**
 * Shows the selected soldier's info.
 * @param action Pointer to an action.
 */
void SoldiersState::lstSoldiersClick(Action *)
{
	
	MultiState *state = new MultiState;
	SoldierInfoState *info = new SoldierInfoState(_base, _lstSoldiers->getSelectedRow());
	state->add(info);
	state->add(new SoldierInfoState(_base, _lstSoldiers->getSelectedRow()));
	state->add(new SoldierDiaryOverviewState(_base, _lstSoldiers->getSelectedRow(), info)); //trying to add the awards screen
//	state->add(new SoldierDiaryMissionState(_base, _lstSoldiers->getSelectedRow(), info));
//	state->add(new SoldierDiaryPerformanceState(_base, _lstSoldiers->getSelectedRow(), info));
	_game->pushState(state);

	/*
	 * Trying to put the inventory into the soldier info screen to 
	 * make an unified view of all soldier information and loadout
	 * The was Warboy did to the InventoryState
	 *

	Craft *craft = _base->getCrafts()->at(_craft);
	if (craft->getNumSoldiers() != 0)
	{
		SavedBattleGame *bgame = new SavedBattleGame();
		_game->getSavedGame()->setBattleGame(bgame);

		BattlescapeGenerator bgen = BattlescapeGenerator(_game);
		bgen.runInventory(craft);

		_game->popState();
		MultiState *state = new MultiState;
		state->add(new SoldierInfoState(_base, _lstSoldiers->getSelectedRow()));
		state->add(new InventoryState(false, 0));
		_game->pushState(state);
	}
	*/



	//_game->pushState(new SoldierInfoState(_base, _lstSoldiers->getSelectedRow()));
}

}
