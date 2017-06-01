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
#include "ResearchState.h"
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
#include "NewResearchListState.h"
#include "../Savegame/ResearchProject.h"
#include "../Mod/RuleResearch.h"
#include "ResearchInfoState.h"

namespace OpenXcom
{

/**
 * Initializes all the elements in the Research screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 */
ResearchState::ResearchState(Base *base) : _base(base)
{
	// Create objects
	_window = new Window(this, 550, 220, 700, 3);
	//_btnNew = new TextButton(148, 16, 708, 196);
	//_btnOk = new TextButton(148, 16, 864, 196);
	_txtTitle = new Text(540, 17, 705, 8);
	_txtAvailable = new Text(150, 11, 710, 24);
	_txtAllocated = new Text(150, 11, 1010, 24);
	_txtSpace = new Text(300, 11, 710, 37);
	_txtProject = new Text(200, 17, 710, 50);
	_txtScientists = new Text(200, 17, 910, 50);
	_txtProgress = new Text(84, 11, 1100, 50);
	_lstResearch = new TextList(530, 812, 710, 65);

	// Set palette
	setInterface("researchMenu");

	add(_window, "window", "researchMenu");
	//add(_btnNew, "button", "researchMenu");
	//add(_btnOk, "button", "researchMenu");
	add(_txtTitle, "text", "researchMenu");
	add(_txtAvailable, "text", "researchMenu");
	add(_txtAllocated, "text", "researchMenu");
	add(_txtSpace, "text", "researchMenu");
	add(_txtProject, "text", "researchMenu");
	add(_txtScientists, "text", "researchMenu");
	add(_txtProgress, "text", "researchMenu");
	add(_lstResearch, "list", "researchMenu");

	//centerAllSurfaces();

	// Set up objects
	_window->setThinBorder();
	_window->setBackground(_game->getMod()->getSurface("BACK05.SCR"));

/*	_btnNew->setText(tr("STR_NEW_PROJECT"));
	_btnNew->onMouseClick((ActionHandler)&ResearchState::btnNewClick);

	_btnOk->setText(tr("STR_OK"));
	_btnOk->onMouseClick((ActionHandler)&ResearchState::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&ResearchState::btnOkClick, Options::keyCancel);
*/

	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setText(tr("STR_CURRENT_RESEARCH"));

	_txtProject->setWordWrap(true);
	_txtProject->setText(tr("STR_RESEARCH_PROJECT"));

	_txtScientists->setWordWrap(true);
	_txtScientists->setText(tr("STR_SCIENTISTS_ALLOCATED_UC"));

	_txtProgress->setText(tr("STR_PROGRESS"));

	_lstResearch->setColumns(3, 252, 138, 100);
	_lstResearch->setSelectable(true);
	_lstResearch->setBackground(_window);
	_lstResearch->setMargin(2);
	_lstResearch->setWordWrap(true);
	_lstResearch->onMouseClick((ActionHandler)&ResearchState::onSelectProject);
	fillProjectList();
}

/**
 *
 */
ResearchState::~ResearchState()
{
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 
void ResearchState::btnOkClick(Action *)
{
	_game->popState();
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 
void ResearchState::btnNewClick(Action *)
{
	_game->pushState(new NewResearchListState(_base));
}*/

/**
 * Displays the list of possible ResearchProjects.
 * @param action Pointer to an action.
 */
void ResearchState::onSelectProject(Action *)
{
	const std::vector<ResearchProject *> & baseProjects(_base->getResearch());
	_game->pushState(new ResearchInfoState(_base, baseProjects[_lstResearch->getSelectedRow()]));
}

/**
 * Updates the research list
 * after going to other screens.
 */
void ResearchState::init()
{
	State::init();
	fillProjectList();
}

/**
 * Fills the list with Base ResearchProjects. Also updates count of available lab space and available/allocated scientists.
 */
void ResearchState::fillProjectList()
{
	const std::vector<ResearchProject *> & baseProjects(_base->getResearch());
	_lstResearch->clearList();
	for (std::vector<ResearchProject *>::const_iterator iter = baseProjects.begin(); iter != baseProjects.end(); ++iter)
	{
		std::wostringstream sstr;
		sstr << (*iter)->getAssigned();
		const RuleResearch *r = (*iter)->getRules();

		std::wstring wstr = tr(r->getName());
		_lstResearch->addRow(3, wstr.c_str(), sstr.str().c_str(), tr((*iter)->getResearchProgress()).c_str());
	}
	_txtAvailable->setText(tr("STR_SCIENTISTS_AVAILABLE").arg(_base->getAvailableScientists()));
	_txtAllocated->setText(tr("STR_SCIENTISTS_ALLOCATED").arg(_base->getAllocatedScientists()));
	_txtSpace->setText(tr("STR_LABORATORY_SPACE_AVAILABLE").arg(_base->getFreeLaboratories()));
}

}
