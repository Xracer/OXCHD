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
#include "ManufactureState.h"
#include <sstream>
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Engine/Language.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Base.h"
#include "../Savegame/SavedGame.h"
#include "../Mod/RuleManufacture.h"
#include "../Savegame/Production.h"
#include "NewManufactureListState.h"
#include "ManufactureInfoState.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Manufacture screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 */
ManufactureState::ManufactureState(Base *base) : _base(base)
{
	// Create objects
	int yPos = 3;
	int xPos = 700;
	
	_window = new Window(this, 570, 200, 0 + xPos, 0 + yPos, POPUP_BOTH);
	//_btnNew = new TextButton(148, 16, 8 + xPos, 179 + yPos);
	//_btnOk = new TextButton(148, 16, 164 + xPos, 179 + yPos);
	_txtTitle = new Text(550, 17, 5 + xPos, 8 + yPos);
	_txtAvailable = new Text(250, 11, 8 + xPos, 27 + yPos);
	_txtAllocated = new Text(250, 11, 255 + xPos, 27 + yPos);
	_txtSpace = new Text(250, 11, 8 + xPos, 40 + yPos);
	_txtFunds = new Text(250, 11, 255 + xPos, 40 + yPos);
	_txtItem = new Text(80, 11, 8 + xPos, 55 + yPos);
	_txtEngineers = new Text(120, 18, 120 + xPos, 55 + yPos);
	_txtProduced = new Text(100, 18, 245 + xPos, 55 + yPos);
	_txtCost = new Text(130, 27, 345 + xPos, 55 + yPos);
	_txtTimeLeft = new Text(100, 27, 455 + xPos, 55 + yPos);
	_lstManufacture = new TextList(550, 180, 8 + xPos, 83 + yPos);

	// Set palette
	setInterface("manufactureMenu");

	add(_window, "window", "manufactureMenu");
	//add(_btnNew, "button", "manufactureMenu");
	//add(_btnOk, "button", "manufactureMenu");
	add(_txtTitle, "text1", "manufactureMenu");
	add(_txtAvailable, "text1", "manufactureMenu");
	add(_txtAllocated, "text1", "manufactureMenu");
	add(_txtSpace, "text1", "manufactureMenu");
	add(_txtFunds, "text1", "manufactureMenu");
	add(_txtItem, "text2", "manufactureMenu");
	add(_txtEngineers, "text2", "manufactureMenu");
	add(_txtProduced, "text2", "manufactureMenu");
	add(_txtCost, "text2", "manufactureMenu");
	add(_txtTimeLeft, "text2", "manufactureMenu");
	add(_lstManufacture, "list", "manufactureMenu");

	//centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK17.SCR"));

	//_btnNew->setText(tr("STR_NEW_PRODUCTION"));
	//_btnNew->onMouseClick((ActionHandler)&ManufactureState::btnNewProductionClick);

	//_btnOk->setText(tr("STR_OK"));
	//_btnOk->onMouseClick((ActionHandler)&ManufactureState::btnOkClick);
	//_btnOk->onKeyboardPress((ActionHandler)&ManufactureState::btnOkClick, Options::keyCancel);

	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setText(tr("STR_CURRENT_PRODUCTION"));

	_txtFunds->setText(tr("STR_CURRENT_FUNDS").arg(Text::formatFunding(_game->getSavedGame()->getFunds())));

	_txtItem->setText(tr("STR_ITEM"));

	_txtEngineers->setText(tr("STR_ENGINEERS__ALLOCATED"));
	_txtEngineers->setWordWrap(true);

	_txtProduced->setText(tr("STR_UNITS_PRODUCED"));
	_txtProduced->setWordWrap(true);

	_txtCost->setText(tr("STR_COST__PER__UNIT"));
	_txtCost->setWordWrap(true);

	_txtTimeLeft->setText(tr("STR_DAYS_HOURS_LEFT"));
	_txtTimeLeft->setWordWrap(true);

	_lstManufacture->setColumns(5, 115, 90, 90, 90, 48);
	_lstManufacture->setAlign(ALIGN_RIGHT);
	_lstManufacture->setAlign(ALIGN_LEFT, 0);
	_lstManufacture->setSelectable(true);
	_lstManufacture->setBackground(_window);
	_lstManufacture->setMargin(2);
	_lstManufacture->setWordWrap(true);
	_lstManufacture->onMouseClick((ActionHandler)&ManufactureState::lstManufactureClick);
	fillProductionList();
}

/**
 *
 */
ManufactureState::~ManufactureState()
{

}

/**
 * Updates the production list
 * after going to other screens.
 */
void ManufactureState::init()
{
	State::init();
	fillProductionList();
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void ManufactureState::btnOkClick(Action *)
{
	_game->popState();
}

/**
 * Opens the screen with the list of possible productions.
 * @param action Pointer to an action.
 */
void ManufactureState::btnNewProductionClick(Action *)
{
	_game->pushState(new NewManufactureListState(_base));
}

/**
 * Fills the list of base productions.
 */
void ManufactureState::fillProductionList()
{
	const std::vector<Production *> productions(_base->getProductions());
	_lstManufacture->clearList();
	for (std::vector<Production *>::const_iterator iter = productions.begin(); iter != productions.end(); ++iter)
	{
		std::wostringstream s1;
		s1 << (*iter)->getAssignedEngineers();
		std::wostringstream s2;
		s2 << (*iter)->getAmountProduced() << "/";
		if ((*iter)->getInfiniteAmount()) s2 << Language::utf8ToWstr("∞");
		else s2 << (*iter)->getAmountTotal();
		if ((*iter)->getSellItems()) s2 << " $";
		std::wostringstream s3;
		s3 << Text::formatFunding((*iter)->getRules()->getManufactureCost());
		std::wostringstream s4;
		if ((*iter)->getInfiniteAmount())
		{
			s4 << Language::utf8ToWstr("∞");
		}
		else if ((*iter)->getAssignedEngineers() > 0)
		{
			int timeLeft = (*iter)->getAmountTotal() * (*iter)->getRules()->getManufactureTime() - (*iter)->getTimeSpent();
			int numEffectiveEngineers = (*iter)->getAssignedEngineers();
			// ensure we round up since it takes an entire hour to manufacture any part of that hour's capacity
			int hoursLeft = (timeLeft + numEffectiveEngineers - 1) / numEffectiveEngineers;
			int daysLeft = hoursLeft / 24;
			int hours = hoursLeft % 24;
			s4 << daysLeft << "/" << hours;
		}
		else
		{

			s4 << L"-";
		}
		_lstManufacture->addRow(5, tr((*iter)->getRules()->getName()).c_str(), s1.str().c_str(), s2.str().c_str(), s3.str().c_str(), s4.str().c_str());
	}
	_txtAvailable->setText(tr("STR_ENGINEERS_AVAILABLE").arg(_base->getAvailableEngineers()));
	_txtAllocated->setText(tr("STR_ENGINEERS_ALLOCATED").arg(_base->getAllocatedEngineers()));
	_txtSpace->setText(tr("STR_WORKSHOP_SPACE_AVAILABLE").arg(_base->getFreeWorkshops()));
}

/**
 * Opens the screen displaying production settings.
 * @param action Pointer to an action.
 */
void ManufactureState::lstManufactureClick(Action *)
{
	const std::vector<Production*> productions(_base->getProductions());
	_game->pushState(new ManufactureInfoState(_base, productions[_lstManufacture->getSelectedRow()]));
}

}
