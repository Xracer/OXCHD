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
#include "MonthlyCostsState.h"
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
#include "../Savegame/SavedGame.h"
#include "../Mod/RuleCraft.h"
#include "../Mod/RuleSoldier.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Monthly Costs screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 */
MonthlyCostsState::MonthlyCostsState(Base *base) : _base(base)
{
	// Create objects
	_window = new Window(this, 550, 172, 700, 210, POPUP_BOTH);
	_txtTitle = new Text(440, 17, 705, 216);
	_txtRental = new Text(170, 11, 705, 233);
	_txtCost = new Text(120, 11, 865, 233); //875
	_txtQuantity = new Text(80, 11, 975, 233); //995
	_txtTotal = new Text(80, 11, 1070, 233);
	_lstCrafts = new TextList(508, 50, 705, 245);
	_txtSalaries = new Text(150, 11, 705, 289);
	_txtIncome = new Text(150, 11, 705, 350);
	_txtMaintenance = new Text(150, 11, 705, 365);
	_lstSalaries = new TextList(508, 40, 705, 300);
	_lstMaintenance = new TextList(508, 11, 705, 338);
	_lstTotal = new TextList(200, 11, 900, 385); ///170, ...120, 80, 80

	// Set palette
	setInterface("hdbaseInfo");

	add(_window, "window", "hdbaseInfo");
	add(_txtTitle, "title", "hdbaseInfo");
	add(_txtCost, "text1", "hdbaseInfo");
	add(_txtQuantity, "text1", "hdbaseInfo");
	add(_txtTotal, "text1", "hdbaseInfo");
	add(_txtRental, "text1", "hdbaseInfo");
	add(_lstCrafts, "list", "hdbaseInfo");
	add(_txtSalaries, "text1", "hdbaseInfo");
	add(_lstSalaries, "list", "hdbaseInfo");
	add(_lstMaintenance, "text1", "hdbaseInfo");
	add(_txtIncome, "list", "hdbaseInfo");
	add(_txtMaintenance, "list", "hdbaseInfo");
	add(_lstTotal, "numbers", "hdbaseInfo");

	//centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK13.SCR"));
	_window->setThinBorder();

	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_LEFT);
	_txtTitle->setText(tr("STR_MONTHLY_COSTS"));

	_txtCost->setText(tr("STR_COST_PER_UNIT"));

	_txtQuantity->setText(tr("STR_QUANTITY"));

	_txtTotal->setText(tr("STR_TOTAL"));

	_txtRental->setText(tr("STR_CRAFT_RENTAL"));

	_txtSalaries->setText(tr("STR_SALARIES"));

	std::wostringstream ss;
	ss << tr("STR_INCOME") << L" = " << Text::formatFunding(_game->getSavedGame()->getCountryFunding());
	_txtIncome->setText(ss.str());

	std::wostringstream ss2;
	ss2 << tr("STR_MAINTENANCE") << L" = " << Text::formatFunding(_game->getSavedGame()->getBaseMaintenance());
	_txtMaintenance->setText(ss2.str());

	_lstCrafts->setColumns(4, 170, 120, 80, 80);
	_lstCrafts->setDot(true);

	const std::vector<std::string> &crafts = _game->getMod()->getCraftsList();
	for (std::vector<std::string>::const_iterator i = crafts.begin(); i != crafts.end(); ++i)
	{
		RuleCraft *craft = _game->getMod()->getCraft(*i);
		if (craft->getRentCost() != 0 && _game->getSavedGame()->isResearched(craft->getRequirements()))
		{
			std::wostringstream ss3;
			ss3 << _base->getCraftCount(*i);
			_lstCrafts->addRow(4, tr(*i).c_str(), Text::formatFunding(craft->getRentCost()).c_str(), ss3.str().c_str(), Text::formatFunding(_base->getCraftCount(*i) * craft->getRentCost()).c_str());
		}
	}

	_lstSalaries->setColumns(4, 170, 120, 80, 80);
	_lstSalaries->setDot(true);

	const std::vector<std::string> &soldiers = _game->getMod()->getSoldiersList();
	for (std::vector<std::string>::const_iterator i = soldiers.begin(); i != soldiers.end(); ++i)
	{
		RuleSoldier *soldier = _game->getMod()->getSoldier(*i);
		if (soldier->getBuyCost() != 0 && _game->getSavedGame()->isResearched(soldier->getRequirements()))
		{
			std::wostringstream ss4;
			ss4 << _base->getSoldierCount(*i);
			std::string name = (*i);
			if (soldiers.size() == 1)
			{
				name = "STR_SOLDIERS";
			}
			_lstSalaries->addRow(4, tr(name).c_str(), Text::formatFunding(soldier->getSalaryCost()).c_str(), ss4.str().c_str(), Text::formatFunding(_base->getSoldierCount(*i) * soldier->getSalaryCost()).c_str());
		}
	}
	std::wostringstream ss5;
	ss5 << _base->getTotalEngineers();
	_lstSalaries->addRow(4, tr("STR_ENGINEERS").c_str(), Text::formatFunding(_game->getMod()->getEngineerCost()).c_str(), ss5.str().c_str(), Text::formatFunding(_base->getTotalEngineers() * _game->getMod()->getEngineerCost()).c_str());
	std::wostringstream ss6;
	ss6 << _base->getTotalScientists();
	_lstSalaries->addRow(4, tr("STR_SCIENTISTS").c_str(), Text::formatFunding(_game->getMod()->getScientistCost()).c_str(), ss6.str().c_str(), Text::formatFunding(_base->getTotalScientists() * _game->getMod()->getScientistCost()).c_str());

	_lstMaintenance->setColumns(2, 370, 70);
	_lstMaintenance->setDot(true);
	std::wostringstream ss7;
	ss7 << L'\x01' << Text::formatFunding(_base->getFacilityMaintenance());
	_lstMaintenance->addRow(2, tr("STR_BASE_MAINTENANCE").c_str(), ss7.str().c_str());

	_lstTotal->setColumns(2, 45, 70);
	_lstTotal->setDot(true);
	_lstTotal->addRow(2, tr("STR_TOTAL").c_str(), Text::formatFunding(_base->getMonthlyMaintenace()).c_str());
}

/**
 *
 */
MonthlyCostsState::~MonthlyCostsState()
{

}


}
