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
#include "CraftArmorState.h"
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Engine/Options.h"
#include "../Engine/Action.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Base.h"
#include "../Savegame/Soldier.h"
#include "../Savegame/Craft.h"
#include "../Mod/Armor.h"
#include "SoldierArmorState.h"
#include "../Savegame/SavedGame.h"
#include "../Savegame/ItemContainer.h"
#include "../Mod/RuleInterface.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Craft Armor screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 * @param craft ID of the selected craft.
 */
CraftArmorState::CraftArmorState(Base *base, size_t craft) : _base(base), _craft(craft)
{
	// Create objects
	_window = new Window(this, 550, 170, 700, 393);
	//_btnOk = new TextButton(288, 16, 716, 576);
	_txtTitle = new Text(300, 17, 716, 399);
	_txtName = new Text(200, 11, 716, 416);
	_txtCraft = new Text(120, 11, 916, 416);
	_txtArmor = new Text(120, 11, 1036, 416);
	_lstSoldiers = new TextList(508, 125, 708, 428);

	// Set palette
	setInterface("hdcraftInfo");

	add(_window, "window", "hdcraftInfo");
	//add(_btnOk, "button", "hdcraftInfo");
	add(_txtTitle, "text1", "hdcraftInfo");
	add(_txtName, "text1", "hdcraftInfo");
	add(_txtCraft, "text1", "hdcraftInfo");
	add(_txtArmor, "text1", "hdcraftInfo");
	add(_lstSoldiers, "list", "hdcraftInfo");

	//centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK14.SCR"));
	_window->setThinBorder();

	//_btnOk->setText(tr("STR_OK"));
	//_btnOk->onMouseClick((ActionHandler)&CraftArmorState::btnOkClick);
	//_btnOk->onKeyboardPress((ActionHandler)&CraftArmorState::btnOkClick, Options::keyCancel);

	_txtTitle->setBig();
	_txtTitle->setText(tr("STR_SELECT_ARMOR"));

	_txtName->setText(tr("STR_NAME_UC"));

	_txtCraft->setText(tr("STR_CRAFT"));

	_txtArmor->setText(tr("STR_ARMOR"));

	_lstSoldiers->setColumns(3, 200, 120, 120);
	_lstSoldiers->setSelectable(true);
	_lstSoldiers->setBackground(_window);
	_lstSoldiers->setMargin(8);
	_lstSoldiers->setScrolling(true, 0);
	_lstSoldiers->onMousePress((ActionHandler)&CraftArmorState::lstSoldiersClick);

	Uint8 otherCraftColor = _game->getMod()->getInterface("craftArmor")->getElement("otherCraft")->color;
	int row = 0;
	Craft *c = _base->getCrafts()->at(_craft);
	for (std::vector<Soldier*>::iterator i = _base->getSoldiers()->begin(); i != _base->getSoldiers()->end(); ++i)
	{
		_lstSoldiers->addRow(3, (*i)->getName(true).c_str(), (*i)->getCraftString(_game->getLanguage()).c_str(), tr((*i)->getArmor()->getType()).c_str());

		Uint8 color;
		if ((*i)->getCraft() == c)
		{
			color = _lstSoldiers->getSecondaryColor();
		}
		else if ((*i)->getCraft() != 0)
		{
			color = otherCraftColor;
		}
		else
		{
			color = _lstSoldiers->getColor();
		}
		_lstSoldiers->setRowColor(row, color);
		row++;
	}
}

/**
 *
 */
CraftArmorState::~CraftArmorState()
{
}

/**
 * The soldier armors can change
 * after going into other screens.
 */
void CraftArmorState::init()
{
	State::init();
	int row = 0;
	for (std::vector<Soldier*>::iterator i = _base->getSoldiers()->begin(); i != _base->getSoldiers()->end(); ++i)
	{
		_lstSoldiers->setCellText(row, 2, tr((*i)->getArmor()->getType()));
		row++;
	}
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 
void CraftArmorState::btnOkClick(Action *)
{
	_game->popState();
}*/

/**
 * Shows the Select Armor window.
 * @param action Pointer to an action.
 */
void CraftArmorState::lstSoldiersClick(Action *action)
{
	Soldier *s = _base->getSoldiers()->at(_lstSoldiers->getSelectedRow());
	if (!(s->getCraft() && s->getCraft()->getStatus() == "STR_OUT"))
	{
		if (action->getDetails()->button.button == SDL_BUTTON_LEFT)
		{
			_game->pushState(new SoldierArmorState(_base, _lstSoldiers->getSelectedRow()));
		}
		else if (action->getDetails()->button.button == SDL_BUTTON_RIGHT)
		{
			SavedGame *save;
			save = _game->getSavedGame();
			Armor *a = _game->getMod()->getArmor(save->getLastSelectedArmor());
			if (save->getMonthsPassed() != -1)
			{
				if (_base->getStorageItems()->getItem(a->getStoreItem()) > 0 || a->getStoreItem() == Armor::NONE)
				{
					if (s->getArmor()->getStoreItem() != Armor::NONE)
					{
						_base->getStorageItems()->addItem(s->getArmor()->getStoreItem());
					}
					if (a->getStoreItem() != Armor::NONE)
					{
						_base->getStorageItems()->removeItem(a->getStoreItem());
					}

					s->setArmor(a);
					_lstSoldiers->setCellText(_lstSoldiers->getSelectedRow(), 2, tr(a->getType()));
				}
			}
			else
			{
				s->setArmor(a);
				_lstSoldiers->setCellText(_lstSoldiers->getSelectedRow(), 2, tr(a->getType()));
			}
		}
	}
}

}
