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
#include "CraftsState.h"
#include <sstream>
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Engine/Options.h"
#include "../Engine/MultiState.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Craft.h"
#include "../Mod/RuleCraft.h"
#include "../Savegame/Base.h"
#include "../Menu/ErrorMessageState.h"
#include "BasescapeState.h"
#include "CraftInfoState.h"
#include "CraftSoldiersState.h"
#include "CraftWeaponsState.h"
#include "CraftEquipmentState.h"
#include "CraftArmorState.h"
#include "SellState.h"
#include "../Savegame/SavedGame.h"
#include "../Mod/RuleInterface.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Equip Craft screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 */
CraftsState::CraftsState(Base *base, Globe *globe) : _base(base), _globe(globe)
{
	// Create objects
	_window = new Window(this, 550, 110, 700, 3);
	//_btnOk = new TextButton(288, 16, 716, 176);
	_txtTitle = new Text(350, 17, 716, 9);
	_txtBase = new Text(350, 17, 716, 25);
	_txtName = new Text(110, 11, 716, 40);
	_txtStatus = new Text(100, 11, 826, 40);
	_txtWeapon = new Text(90, 11, 910, 40);
	_txtCrew = new Text(80, 11, 1020, 40);
	_txtHwp = new Text(50, 11, 1095, 40);
	_lstCrafts = new TextList(508, 90, 708, 52);

	// Set palette
	setInterface("hdcraftInfo");

	add(_window, "window", "hdcraftInfo");
	//add(_btnOk, "button", "hdcraftInfo");
	add(_txtTitle, "title", "hdcraftInfo");
	add(_txtBase, "text1", "hdcraftInfo");
	add(_txtName, "text1", "hdcraftInfo");
	add(_txtStatus, "text1", "hdcraftInfo");
	add(_txtWeapon, "text1", "hdcraftInfo");
	add(_txtCrew, "text1", "hdcraftInfo");
	add(_txtHwp, "text1", "hdcraftInfo");
	add(_lstCrafts, "list", "hdcraftInfo");

	//centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK14.SCR"));
	_window->setThinBorder();

	/*
	_btnOk->setText(tr("STR_OK"));
	_btnOk->onMouseClick((ActionHandler)&CraftsState::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&CraftsState::btnOkClick, Options::keyCancel);
	*/
	
	_txtTitle->setBig();
	_txtTitle->setText(tr("STR_INTERCEPTION_CRAFT"));

	_txtBase->setBig();
	_txtBase->setText(tr("STR_BASE_").arg(_base->getName()));

	_txtName->setText(tr("STR_NAME_UC"));

	_txtStatus->setText(tr("STR_STATUS"));

	_txtWeapon->setText(tr("STR_WEAPON_SYSTEMS"));
	_txtWeapon->setWordWrap(true);

	_txtCrew->setText(tr("STR_CREW"));

	_txtHwp->setText(tr("STR_HWPS"));
	_lstCrafts->setColumns(5, 110, 100, 90, 80, 40);
	_lstCrafts->setSelectable(true);
	_lstCrafts->setBackground(_window);
	_lstCrafts->setMargin(8);
	_lstCrafts->onMouseClick((ActionHandler)&CraftsState::lstCraftsClick);
}

/**
 *
 */
CraftsState::~CraftsState()
{

}

/**
 * The soldier names can change
 * after going into other screens.
 */
void CraftsState::init()
{
	State::init();
	_lstCrafts->clearList();
	for (std::vector<Craft*>::iterator i = _base->getCrafts()->begin(); i != _base->getCrafts()->end(); ++i)
	{
		std::wostringstream ss, ss2, ss3;
		ss << (*i)->getNumWeapons() << "/" << (*i)->getRules()->getWeapons();
		ss2 << (*i)->getNumSoldiers();
		ss3 << (*i)->getNumVehicles();
		_lstCrafts->addRow(5, (*i)->getName(_game->getLanguage()).c_str(), tr((*i)->getStatus()).c_str(), ss.str().c_str(), ss2.str().c_str(), ss3.str().c_str());
	}
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void CraftsState::btnOkClick(Action *)
{
	_game->popState();

	if (_game->getSavedGame()->getMonthsPassed() > -1 && Options::storageLimitsEnforced && _base->storesOverfull())
	{
		_game->pushState(new SellState(_base));
		_game->pushState(new ErrorMessageState(tr("STR_STORAGE_EXCEEDED").arg(_base->getName()), _palette, _game->getMod()->getInterface("craftSelect")->getElement("errorMessage")->color, "BACK01.SCR", _game->getMod()->getInterface("craftSelect")->getElement("errorPalette")->color));
	}
}

/**
 * Shows the selected craft's info.
 * @param action Pointer to an action.
 */
void CraftsState::lstCraftsClick(Action *)
{

	if (_base->getCrafts()->at(_lstCrafts->getSelectedRow())->getStatus() != "STR_OUT")
	{
		_game->popState();
		MultiState *state = new MultiState;
		state->add(new BasescapeState(_base, _globe));
		state->add(new CraftsState(_base, _globe));
		state->add(new CraftInfoState(_base, _lstCrafts->getSelectedRow()));
		if (_base->getCrafts()->at(_lstCrafts->getSelectedRow())->getNumSoldiers() > 0) // need to create a condition to show this states only if the craft is a transport
		{
			state->add(new CraftSoldiersState(_base, _lstCrafts->getSelectedRow()));
			state->add(new CraftEquipmentState(_base, _lstCrafts->getSelectedRow()));
			state->add(new CraftArmorState(_base, _lstCrafts->getSelectedRow()));
		}

		_game->pushState(state);

		//if (_base->getCrafts()->getNumSoldiers() > 0)
		//_game->pushState(new CraftInfoState(_base, _lstCrafts->getSelectedRow()));
	}
}

}
