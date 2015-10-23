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
#include "MainMenuState.h"
#include <sstream>
#include "../version.h"
#include "../Engine/Game.h"
#include "../Mod/Mod.h"
#include "../Engine/Language.h"
#include "../Engine/Screen.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "NewGameState.h"
#include "NewBattleState.h"
#include "ListLoadState.h"
#include "OptionsVideoState.h"
#include "../Engine/Screen.h"
#include "../Engine/Options.h"

namespace OpenXcom
{

void GoToMainMenuState::init()
{
	Screen::updateScale(Options::geoscapeScale, Options::geoscapeScale, Options::baseXGeoscape, Options::baseYGeoscape, true);
	_game->getScreen()->resetDisplay(false);
	_game->setState(new MainMenuState);
}

/**
 * Initializes all the elements in the Main Menu window.
 * @param game Pointer to the core game.
 */
MainMenuState::MainMenuState()
{
	// Create objects
	_window = new Window(this, 960, 600, 0, 0, POPUP_BOTH);
	_btnNewGame = new TextButton(120, 25, 340, 200);
	_btnNewBattle = new TextButton(120, 25, 500, 200);
	_btnLoad = new TextButton(120, 25, 340, 250);
	_btnOptions = new TextButton(120, 25, 500, 250);
	_btnQuit = new TextButton(200, 25, 380, 300);
	_txtTitle = new Text(300, 30, 340, 150);

	// Set palette
	setInterface("mainMenu");

	add(_window, "window", "mainMenu");
	add(_btnNewGame, "button", "mainMenu");
	add(_btnNewBattle, "button", "mainMenu");
	add(_btnLoad, "button", "mainMenu");
	add(_btnOptions, "button", "mainMenu");
	add(_btnQuit, "button", "mainMenu");
	add(_txtTitle, "text", "mainMenu");

	centerAllSurfaces();

	// Set up objects
	_window->setBackground(_game->getMod()->getSurface("HDBACK01.PNG"));

	_btnNewGame->setText(tr("STR_NEW_GAME"));
	_btnNewGame->onMouseClick((ActionHandler)&MainMenuState::btnNewGameClick);

	_btnNewBattle->setText(tr("STR_NEW_BATTLE"));
	_btnNewBattle->onMouseClick((ActionHandler)&MainMenuState::btnNewBattleClick);

	_btnLoad->setText(tr("STR_LOAD_SAVED_GAME"));
	_btnLoad->onMouseClick((ActionHandler)&MainMenuState::btnLoadClick);

	_btnOptions->setText(tr("STR_OPTIONS"));
	_btnOptions->onMouseClick((ActionHandler)&MainMenuState::btnOptionsClick);

	_btnQuit->setText(tr("STR_QUIT"));
	_btnQuit->onMouseClick((ActionHandler)&MainMenuState::btnQuitClick);

	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setBig();
	std::wostringstream title;
	title << tr("STR_OPENXCOM") << L"\x02";
	title << Language::utf8ToWstr(OPENXCOM_VERSION_SHORT) << Language::utf8ToWstr(OPENXCOM_VERSION_GIT);
	_txtTitle->setText(title.str());
}

/**
 *
 */
MainMenuState::~MainMenuState()
{

}

/**
 * Opens the New Game window.
 * @param action Pointer to an action.
 */
void MainMenuState::btnNewGameClick(Action *)
{
	_game->pushState(new NewGameState);
}

/**
 * Opens the New Battle screen.
 * @param action Pointer to an action.
 */
void MainMenuState::btnNewBattleClick(Action *)
{
	_game->pushState(new NewBattleState);
}

/**
 * Opens the Load Game screen.
 * @param action Pointer to an action.
 */
void MainMenuState::btnLoadClick(Action *)
{
	_game->pushState(new ListLoadState(OPT_MENU));
}

/**
 * Opens the Options screen.
 * @param action Pointer to an action.
 */
void MainMenuState::btnOptionsClick(Action *)
{
	Options::backupDisplay();
	_game->pushState(new OptionsVideoState(OPT_MENU));
}

/**
 * Quits the game.
 * @param action Pointer to an action.
 */
void MainMenuState::btnQuitClick(Action *)
{
	_game->quit();
}

/**
 * Updates the scale.
 * @param dX delta of X;
 * @param dY delta of Y;
 */
void MainMenuState::resize(int &dX, int &dY)
{
	dX = Options::baseXResolution;
	dY = Options::baseYResolution;
	Screen::updateScale(Options::geoscapeScale, Options::geoscapeScale, Options::baseXGeoscape, Options::baseYGeoscape, true);
	dX = Options::baseXResolution - dX;
	dY = Options::baseYResolution - dY;
	State::resize(dX, dY);
}
}
