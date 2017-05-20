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
#include "BaseInfoState.h"
#include <sstream>
#include "../Engine/Game.h"
#include "../Engine/Action.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Engine/Options.h"
#include "../Interface/Bar.h"
#include "../Interface/TextButton.h"
#include "../Interface/Text.h"
#include "../Interface/TextEdit.h"
#include "../Interface/Window.h"
#include "../Engine/Surface.h"
#include "MiniBaseView.h"
#include "../Savegame/SavedGame.h"
#include "../Savegame/Base.h"
#include "MonthlyCostsState.h"
#include "TransfersState.h"
#include "StoresState.h"
#include "BasescapeState.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Base Info screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 * @param state Pointer to the Basescape state.
 */
BaseInfoState::BaseInfoState(Base *base, BasescapeState *state) : _base(base), _state(state)
{
	// Create objects
	_window = new Window(this, 550, 203, 700, 3, POPUP_BOTH);
	_bg = new Surface(400, 203, 700, 3);
	//_edtBase = new TextEdit(this, 127, 16, 8, 8);

	_txtPersonnel = new Text(300, 11, 708, 8);
	_txtSoldiers = new Text(125, 11, 708, 21);
	_numSoldiers = new Text(40, 11, 868, 21);
	_barSoldiers = new Bar(150, 9, 908, 22);
	_txtEngineers = new Text(125, 11, 708, 34);
	_numEngineers = new Text(40, 11, 868, 34);
	_barEngineers = new Bar(150, 9, 908, 35);
	_txtScientists = new Text(125, 11, 708, 47);
	_numScientists = new Text(40, 11, 868, 47);
	_barScientists = new Bar(150, 9, 908, 48);

	_txtSpace = new Text(300, 11, 708, 62);
	_txtQuarters = new Text(130, 11, 708, 76);
	_numQuarters = new Text(40, 11, 868, 76);
	_barQuarters = new Bar(150, 9, 908, 77);
	_txtStores = new Text(130, 11, 708, 89);
	_numStores = new Text(40, 11, 868, 89);
	_barStores = new Bar(150, 9, 908, 90);
	_txtLaboratories = new Text(130, 11, 708, 102);
	_numLaboratories = new Text(40, 11, 868, 102);
	_barLaboratories = new Bar(150, 9, 908, 103);
	_txtWorkshops = new Text(130, 11, 708, 115);
	_numWorkshops = new Text(40, 11, 868, 115);
	_barWorkshops = new Bar(150, 9, 908, 116);
	if (Options::storageLimitsEnforced)
	{
		_txtContainment = new Text(130, 11, 708, 128);
		_numContainment = new Text(40, 11, 868, 128);
		_barContainment = new Bar(150, 9, 908, 129);
	}
	_txtHangars = new Text(130, 11, 708, 141);
	_numHangars = new Text(40, 11, 868, 141);
	_barHangars = new Bar(150, 9, 908, 142);

	_txtDefense = new Text(130, 11, 708, 159);
	_numDefense = new Text(40, 11, 868, 159);
	_barDefense = new Bar(150, 9, 908, 160);
	_txtShortRange = new Text(130, 11, 708, 178);
	_numShortRange = new Text(40, 11, 868, 178);
	_barShortRange = new Bar(150, 9, 908, 179);
	_txtLongRange = new Text(130, 11, 708, 191);
	_numLongRange = new Text(40, 11, 868, 191);
	_barLongRange = new Bar(150, 9, 908, 192);

	// Set palette
	setInterface("hdbaseInfo");

	add(_bg);
	//add(_edtBase, "text1", "baseInfo");

	add(_txtPersonnel, "text1", "hdbaseInfo");
	add(_txtSoldiers, "text2", "hdbaseInfo");
	add(_numSoldiers, "numbers", "hdbaseInfo");
	add(_barSoldiers, "personnelBars", "hdbaseInfo");
	add(_txtEngineers, "text2", "hdbaseInfo");
	add(_numEngineers, "numbers", "hdbaseInfo");
	add(_barEngineers, "personnelBars", "hdbaseInfo");
	add(_txtScientists, "text2", "hdbaseInfo");
	add(_numScientists, "numbers", "hdbaseInfo");
	add(_barScientists, "personnelBars", "hdbaseInfo");

	add(_txtSpace, "text1", "hdbaseInfo");
	add(_txtQuarters, "text2", "hdbaseInfo");
	add(_numQuarters, "numbers", "hdbaseInfo");
	add(_barQuarters, "facilityBars", "hdbaseInfo");
	add(_txtStores, "text2", "hdbaseInfo");
	add(_numStores, "numbers", "hdbaseInfo");
	add(_barStores, "facilityBars", "hdbaseInfo");
	add(_txtLaboratories, "text2", "hdbaseInfo");
	add(_numLaboratories, "numbers", "hdbaseInfo");
	add(_barLaboratories, "facilityBars", "hdbaseInfo");
	add(_txtWorkshops, "text2", "hdbaseInfo");
	add(_numWorkshops, "numbers", "hdbaseInfo");
	add(_barWorkshops, "facilityBars", "hdbaseInfo");
	if (Options::storageLimitsEnforced)
	{
		add(_txtContainment, "text2", "hdbaseInfo");
		add(_numContainment, "numbers", "hdbaseInfo");
		add(_barContainment, "facilityBars", "hdbaseInfo");
	}
	add(_txtHangars, "text2", "hdbaseInfo");
	add(_numHangars, "numbers", "hdbaseInfo");
	add(_barHangars, "facilityBars", "hdbaseInfo");

	add(_txtDefense, "text2", "hdbaseInfo");
	add(_numDefense, "numbers", "hdbaseInfo");
	add(_barDefense, "defenceBar", "hdbaseInfo");
	add(_txtShortRange, "text2", "hdbaseInfo");
	add(_numShortRange, "numbers", "hdbaseInfo");
	add(_barShortRange, "detectionBars", "hdbaseInfo");
	add(_txtLongRange, "text2", "hdbaseInfo");
	add(_numLongRange, "numbers", "hdbaseInfo");
	add(_barLongRange, "detectionBars", "hdbaseInfo");

	// Set up objects
	_window->setThinBorder();
	std::ostringstream ss;
	if (Options::storageLimitsEnforced)
	{
		ss << "ALT";
	}
	ss << "BACK07ALT.PNG";
	_game->getMod()->getSurface(ss.str())->blit(_bg);

	//_edtBase->setBig();
	//_edtBase->onChange((ActionHandler)&BaseInfoState::edtBaseChange);

	_txtPersonnel->setText(tr("STR_PERSONNEL_AVAILABLE_PERSONNEL_TOTAL"));

	_txtSoldiers->setText(tr("STR_SOLDIERS"));

	_barSoldiers->setScale(1.0);

	_txtEngineers->setText(tr("STR_ENGINEERS"));

	_barEngineers->setScale(1.0);

	_txtScientists->setText(tr("STR_SCIENTISTS"));

	_barScientists->setScale(1.0);


	_txtSpace->setText(tr("STR_SPACE_USED_SPACE_AVAILABLE"));

	_txtQuarters->setText(tr("STR_LIVING_QUARTERS_PLURAL"));

	_barQuarters->setScale(0.5);

	_txtStores->setText(tr("STR_STORES"));

	_barStores->setScale(0.5);

	_txtLaboratories->setText(tr("STR_LABORATORIES"));

	_barLaboratories->setScale(0.5);

	_txtWorkshops->setText(tr("STR_WORK_SHOPS"));

	_barWorkshops->setScale(0.5);

	if (Options::storageLimitsEnforced)
	{
		_txtContainment->setText(tr("STR_ALIEN_CONTAINMENT"));

		_barContainment->setScale(0.5);
	}

	_txtHangars->setText(tr("STR_HANGARS"));

	_barHangars->setScale(18.0);


	_txtDefense->setText(tr("STR_DEFENSE_STRENGTH"));

	_barDefense->setScale(0.125);

	_txtShortRange->setText(tr("STR_SHORT_RANGE_DETECTION"));

	_barShortRange->setScale(25.0);

	_txtLongRange->setText(tr("STR_LONG_RANGE_DETECTION"));

	_barLongRange->setScale(25.0);
}

/**
 *
 */
BaseInfoState::~BaseInfoState()
{

}

/**
 * The player can change the selected base.
 */
void BaseInfoState::init()
{
	State::init();
	//_edtBase->setText(_base->getName());

	std::wostringstream ss;
	ss << _base->getAvailableSoldiers() << ":" << _base->getTotalSoldiers();
	_numSoldiers->setText(ss.str());

	_barSoldiers->setMax(_base->getTotalSoldiers());
	_barSoldiers->setValue(_base->getAvailableSoldiers());

	std::wostringstream ss2;
	ss2 << _base->getAvailableEngineers() << ":" << _base->getTotalEngineers();
	_numEngineers->setText(ss2.str());

	_barEngineers->setMax(_base->getTotalEngineers());
	_barEngineers->setValue(_base->getAvailableEngineers());

	std::wostringstream ss3;
	ss3 << _base->getAvailableScientists() << ":" << _base->getTotalScientists();
	_numScientists->setText(ss3.str());

	_barScientists->setMax(_base->getTotalScientists());
	_barScientists->setValue(_base->getAvailableScientists());


	std::wostringstream ss4;
	ss4 << _base->getUsedQuarters() << ":" << _base->getAvailableQuarters();
	_numQuarters->setText(ss4.str());

	_barQuarters->setMax(_base->getAvailableQuarters());
	_barQuarters->setValue(_base->getUsedQuarters());

	std::wostringstream ss5;
	ss5 << (int)floor(_base->getUsedStores() + 0.05) << ":" << _base->getAvailableStores();
	_numStores->setText(ss5.str());

	_barStores->setMax(_base->getAvailableStores());
	_barStores->setValue((int)floor(_base->getUsedStores() + 0.05));

	std::wostringstream ss6;
	ss6 << _base->getUsedLaboratories() << ":" << _base->getAvailableLaboratories();
	_numLaboratories->setText(ss6.str());

	_barLaboratories->setMax(_base->getAvailableLaboratories());
	_barLaboratories->setValue(_base->getUsedLaboratories());

	std::wostringstream ss7;
	ss7 << _base->getUsedWorkshops() << ":" << _base->getAvailableWorkshops();
	_numWorkshops->setText(ss7.str());

	_barWorkshops->setMax(_base->getAvailableWorkshops());
	_barWorkshops->setValue(_base->getUsedWorkshops());

	if (Options::storageLimitsEnforced)
	{
		std::wostringstream ss72;
		ss72 << _base->getUsedContainment() << ":" << _base->getAvailableContainment();
		_numContainment->setText(ss72.str());

		_barContainment->setMax(_base->getAvailableContainment());
		_barContainment->setValue(_base->getUsedContainment());
	}

	std::wostringstream ss8;
	ss8 << _base->getUsedHangars() << ":" << _base->getAvailableHangars();
	_numHangars->setText(ss8.str());

	_barHangars->setMax(_base->getAvailableHangars());
	_barHangars->setValue(_base->getUsedHangars());


	std::wostringstream ss9;
	ss9 << _base->getDefenseValue();
	_numDefense->setText(ss9.str());

	_barDefense->setMax(_base->getDefenseValue());
	_barDefense->setValue(_base->getDefenseValue());

	std::wostringstream ss10;
	ss10 << _base->getShortRangeDetection();
	_numShortRange->setText(ss10.str());

	_barShortRange->setMax(_base->getShortRangeDetection());
	_barShortRange->setValue(_base->getShortRangeDetection());

	std::wostringstream ss11;
	ss11 << _base->getLongRangeDetection();
	_numLongRange->setText(ss11.str());

	_barLongRange->setMax(_base->getLongRangeDetection());
	_barLongRange->setValue(_base->getLongRangeDetection());
}

/**
 * Changes the base name.
 * @param action Pointer to an action.

void BaseInfoState::edtBaseChange(Action *)
{
	_base->setName(_edtBase->getText());
} */

/**
 * Selects a new base to display.
 * @param action Pointer to an action.
 */
void BaseInfoState::handleKeyPress(Action *action)
{
	if (action->getDetails()->type == SDL_KEYDOWN)
	{
		SDLKey baseKeys[] = {Options::keyBaseSelect1,
			                 Options::keyBaseSelect2,
			                 Options::keyBaseSelect3,
			                 Options::keyBaseSelect4,
			                 Options::keyBaseSelect5,
			                 Options::keyBaseSelect6,
			                 Options::keyBaseSelect7,
			                 Options::keyBaseSelect8};
		int key = action->getDetails()->key.keysym.sym;
		for (size_t i = 0; i < _game->getSavedGame()->getBases()->size(); ++i)
		{
			if (key == baseKeys[i])
			{
			//	_mini->setSelectedBase(i);
				_base = _game->getSavedGame()->getBases()->at(i);
				_state->setBase(_base);
				init();
				break;
			}
		}
	}
}

}
