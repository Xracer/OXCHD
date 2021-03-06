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
#include "PlaceFacilityState.h"
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "BaseView.h"
#include "../Savegame/Base.h"
#include "../Savegame/BaseFacility.h"
#include "../Mod/RuleBaseFacility.h"
#include "../Savegame/SavedGame.h"
#include "../Menu/ErrorMessageState.h"
#include "../Engine/Options.h"
#include "../Mod/RuleInterface.h"
#include <climits>

namespace OpenXcom
{

/**
 * Initializes all the elements in the Place Facility window.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 * @param rule Pointer to the facility ruleset to build.
 */
PlaceFacilityState::PlaceFacilityState(Base *base, RuleBaseFacility *rule) : _base(base), _rule(rule)
{
	_screen = false;

	int xPos = 700;
	int yPos = 3;

	// Create objects
	_window = new Window(this, 550, 160, 0 + xPos, 0 + yPos);
	_view = new BaseView(374, 374, 15, 150);
	_btnCancel = new TextButton(112, 16, 8 + xPos, 140 + yPos);
	_txtFacility = new Text(550, 11, 8 + xPos, 8 + yPos);
	_txtCost = new Text(130, 11, 5 + xPos, 21 + yPos);
	_numCost = new Text(130, 17, 5 + xPos, 34 + yPos);
	_txtTime = new Text(130, 11, 140 + xPos, 21 + yPos);
	_numTime = new Text(130, 17, 140 + xPos, 34 + yPos);
	_txtMaintenance = new Text(130, 11, 300 + xPos, 21 + yPos);
	_numMaintenance = new Text(130, 17, 300 + xPos, 34 + yPos);

	// Set palette
	setInterface("placeFacility");

	add(_window, "window", "placeFacility");
	add(_view, "baseView", "basescape");
	add(_btnCancel, "button", "placeFacility");
	add(_txtFacility, "text", "placeFacility");
	add(_txtCost, "text", "placeFacility");
	add(_numCost, "numbers", "placeFacility");
	add(_txtTime, "text", "placeFacility");
	add(_numTime, "numbers", "placeFacility");
	add(_txtMaintenance, "text", "placeFacility");
	add(_numMaintenance, "numbers", "placeFacility");

	//centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK01.SCR"));

	_view->setTexture(_game->getMod()->getSurfaceSet("BASEBITS.PCK"));
	_view->setBase(_base);
	_view->setSelectable(rule->getSize());
	_view->onMouseClick((ActionHandler)&PlaceFacilityState::viewClick);

	_btnCancel->setText(tr("STR_CANCEL"));
	_btnCancel->onMouseClick((ActionHandler)&PlaceFacilityState::btnCancelClick);
	_btnCancel->onKeyboardPress((ActionHandler)&PlaceFacilityState::btnCancelClick, Options::keyCancel);

	_txtFacility->setText(tr(_rule->getType()));

	_txtCost->setText(tr("STR_COST_UC"));

	_numCost->setBig();
	_numCost->setText(Text::formatFunding(_rule->getBuildCost()));

	_txtTime->setText(tr("STR_CONSTRUCTION_TIME_UC"));

	_numTime->setBig();
	_numTime->setText(tr("STR_DAY", _rule->getBuildTime()));

	_txtMaintenance->setText(tr("STR_MAINTENANCE_UC"));

	_numMaintenance->setBig();
	_numMaintenance->setText(Text::formatFunding(_rule->getMonthlyCost()));
}

/**
 *
 */
PlaceFacilityState::~PlaceFacilityState()
{

}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void PlaceFacilityState::btnCancelClick(Action *)
{
	_game->popState();
}

/**
 * Processes clicking on facilities.
 * @param action Pointer to an action.
 */
void PlaceFacilityState::viewClick(Action *)
{
	if (!_view->isPlaceable(_rule))
	{
		_game->pushState(new ErrorMessageState(tr("STR_CANNOT_BUILD_HERE"), _palette, _game->getMod()->getInterface("placeFacility")->getElement("errorMessage")->color, "BACK01.SCR", _game->getMod()->getInterface("placeFacility")->getElement("errorPalette")->color));
	}
	else if (_game->getSavedGame()->getFunds() < _rule->getBuildCost())
	{
		_game->popState();
		_game->pushState(new ErrorMessageState(tr("STR_NOT_ENOUGH_MONEY"), _palette, _game->getMod()->getInterface("placeFacility")->getElement("errorMessage")->color, "BACK01.SCR", _game->getMod()->getInterface("placeFacility")->getElement("errorPalette")->color));
	}
	else
	{
		BaseFacility *fac = new BaseFacility(_rule, _base);
		fac->setX(_view->getGridX());
		fac->setY(_view->getGridY());
		fac->setBuildTime(_rule->getBuildTime());
		_base->getFacilities()->push_back(fac);
		if (Options::allowBuildingQueue)
		{
			if (_view->isQueuedBuilding(_rule)) fac->setBuildTime(INT_MAX);
			_view->reCalcQueuedBuildings();
		}
		_game->getSavedGame()->setFunds(_game->getSavedGame()->getFunds() - _rule->getBuildCost());
		_game->popState();
	}
}

}
