/*
 * Copyright 2010-2015 OpenXcom Developers.
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
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Craft.h"
#include "../Mod/RuleCraft.h"
#include "../Savegame/Base.h"
#include "../Menu/ErrorMessageState.h"
#include "CraftInfoState.h"
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
CraftsState::CraftsState(Base *base) : _base(base)
{
	// Create objects
	_window = new Window(this, 450, 203, 700, 3);
	_btnOk = new TextButton(288, 16, 716, 176);
	_txtTitle = new Text(350, 17, 716, 8);
	_txtBase = new Text(350, 17, 716, 24);
	_txtName = new Text(100, 11, 716, 40);
	_txtStatus = new Text(50, 11, 820, 40);
	_txtWeapon = new Text(130, 11, 890, 40);
	_txtCrew = new Text(60, 11, 970, 40);
	_txtHwp = new Text(30, 11, 1038, 40);
	_lstCrafts = new TextList(400, 130, 708, 58);

	// Set palette
	setInterface("craftSelect");

	add(_window, "window", "craftSelect");
	add(_btnOk, "button", "craftSelect");
	add(_txtTitle, "text", "craftSelect");
	add(_txtBase, "text", "craftSelect");
	add(_txtName, "text", "craftSelect");
	add(_txtStatus, "text", "craftSelect");
	add(_txtWeapon, "text", "craftSelect");
	add(_txtCrew, "text", "craftSelect");
	add(_txtHwp, "text", "craftSelect");
	add(_lstCrafts, "list", "craftSelect");

	//centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK14.SCR"));

	_btnOk->setText(tr("STR_OK"));
	_btnOk->onMouseClick((ActionHandler)&CraftsState::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&CraftsState::btnOkClick, Options::keyCancel);

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
	_lstCrafts->setColumns(5, 104, 90, 70, 70, 30);
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
		_game->pushState(new ErrorMessageState(tr("STR_STORAGE_EXCEEDED").arg(_base->getName()).c_str(), _palette, _game->getMod()->getInterface("craftSelect")->getElement("errorMessage")->color, "BACK01.SCR", _game->getMod()->getInterface("craftSelect")->getElement("errorPalette")->color));
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
		_game->pushState(new CraftInfoState(_base, _lstCrafts->getSelectedRow()));
	}
}

}
