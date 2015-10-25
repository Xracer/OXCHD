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
	_window = new Window(this, 450, 172, 700, 210, POPUP_BOTH);
	_txtTitle = new Text(440, 15, 705, 220);
	_txtCost = new Text(80, 11, 855, 235);
	_txtQuantity = new Text(55, 11, 945, 235);
	_txtTotal = new Text(60, 11, 1010, 235);
	_txtRental = new Text(150, 11, 705, 235);
	_txtSalaries = new Text(150, 11, 705, 280);
	_txtIncome = new Text(150, 11, 710, 350);
	_txtMaintenance = new Text(150, 11, 705, 365);
	_lstCrafts = new TextList(400, 30, 705, 250);
	_lstSalaries = new TextList(400, 40, 705, 295);
	_lstMaintenance = new TextList(400, 11, 705, 335);
	_lstTotal = new TextList(130, 11, 905, 365);

	// Set palette
	setInterface("costsInfo");

	add(_window, "window", "costsInfo");
	add(_txtTitle, "text1", "costsInfo");
	add(_txtCost, "text1", "costsInfo");
	add(_txtQuantity, "text1", "costsInfo");
	add(_txtTotal, "text1", "costsInfo");
	add(_txtRental, "text1", "costsInfo");
	add(_lstCrafts, "list", "costsInfo");
	add(_txtSalaries, "text1", "costsInfo");
	add(_lstSalaries, "list", "costsInfo");
	add(_lstMaintenance, "text1", "costsInfo");
	add(_txtIncome, "list", "costsInfo");
	add(_txtMaintenance, "list", "costsInfo");
	add(_lstTotal, "text2", "costsInfo");

	//centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("BACK13.SCR"));
	_window->setThinBorder();

	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setText(tr("STR_MONTHLY_COSTS"));

	_txtCost->setText(tr("STR_COST_PER_UNIT"));

	_txtQuantity->setText(tr("STR_QUANTITY"));

	_txtTotal->setText(tr("STR_TOTAL"));

	_txtRental->setText(tr("STR_CRAFT_RENTAL"));

	_txtSalaries->setText(tr("STR_SALARIES"));

	std::wostringstream ss;
	ss << tr("STR_INCOME") << L"=" << Text::formatFunding(_game->getSavedGame()->getCountryFunding());
	_txtIncome->setText(ss.str());

	std::wostringstream ss2;
	ss2 << tr("STR_MAINTENANCE") << L"=" << Text::formatFunding(_game->getSavedGame()->getBaseMaintenance());
	_txtMaintenance->setText(ss2.str());

	_lstCrafts->setColumns(4, 160, 110, 60, 70);
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

	_lstSalaries->setColumns(4, 160, 110, 60, 70);
	_lstSalaries->setDot(true);

	const std::vector<std::string> &soldiers = _game->getMod()->getSoldiersList();
	for (std::vector<std::string>::const_iterator i = soldiers.begin(); i != soldiers.end(); ++i)
	{
		RuleSoldier *soldier = _game->getMod()->getSoldier(*i);
		std::wostringstream ss4;
		ss4 << _base->getSoldierCount(*i);
		std::string name = (*i);
		if (soldiers.size() == 1)
		{
			name = "STR_SOLDIERS";
		}
		_lstSalaries->addRow(4, tr(name).c_str(), Text::formatFunding(soldier->getSalaryCost()).c_str(), ss4.str().c_str(), Text::formatFunding(_base->getSoldierCount(*i) * soldier->getSalaryCost()).c_str());
	}	
	std::wostringstream ss5;
	ss5 << _base->getTotalEngineers();
	_lstSalaries->addRow(4, tr("STR_ENGINEERS").c_str(), Text::formatFunding(_game->getMod()->getEngineerCost()).c_str(), ss5.str().c_str(), Text::formatFunding(_base->getTotalEngineers() * _game->getMod()->getEngineerCost()).c_str());
	std::wostringstream ss6;
	ss6 << _base->getTotalScientists();
	_lstSalaries->addRow(4, tr("STR_SCIENTISTS").c_str(), Text::formatFunding(_game->getMod()->getScientistCost()).c_str(), ss6.str().c_str(), Text::formatFunding(_base->getTotalScientists() * _game->getMod()->getScientistCost()).c_str());

	_lstMaintenance->setColumns(2, 330, 70);
	_lstMaintenance->setDot(true);
	std::wostringstream ss7;
	ss7 << L'\x01' << Text::formatFunding(_base->getFacilityMaintenance());
	_lstMaintenance->addRow(2, tr("STR_BASE_MAINTENANCE").c_str(), ss7.str().c_str());

	_lstTotal->setColumns(2, 44, 55);
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
