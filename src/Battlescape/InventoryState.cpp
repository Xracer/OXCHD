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
#include "InventoryState.h"
#include "Inventory.h"
#include "../Engine/Game.h"
#include "../Engine/FileMap.h"
#include "../Mod/Mod.h"
#include "../Engine/LocalizedText.h"
#include "../Engine/CrossPlatform.h"
#include "../Engine/Screen.h"
#include "../Engine/Palette.h"
#include "../Engine/Surface.h"
#include "../Interface/Text.h"
#include "../Interface/BattlescapeButton.h"
#include "../Engine/Action.h"
#include "../Engine/InteractiveSurface.h"
#include "../Engine/Sound.h"
#include "../Engine/SurfaceSet.h"
#include "../Interface/Text.h"
#include "../Interface/BattlescapeButton.h"
#include "../Interface/Text.h"
#include "../Interface/BattlescapeButton.h"
#include "../Interface/Bar.h"
#include "../Savegame/SavedGame.h"
#include "../Savegame/Tile.h"
#include "../Savegame/SavedBattleGame.h"
#include "../Savegame/BattleUnit.h"
#include "../Savegame/Soldier.h"
#include "../Mod/RuleItem.h"
#include "../Mod/RuleInventory.h"
#include "../Mod/Armor.h"
#include "../Engine/Options.h"
#include "UnitInfoState.h"
#include "BattlescapeState.h"
#include "TileEngine.h"
#include "../Mod/Mod.h"
#include "../Mod/RuleInterface.h"

namespace OpenXcom
{

static const int _templateBtnX = 1252;
static const int _createTemplateBtnY = 592;
static const int _applyTemplateBtnY  = 617;

/**
 * Initializes all the elements in the Inventory screen.
 * @param game Pointer to the core game.
 * @param tu Does Inventory use up Time Units?
 * @param parent Pointer to parent Battlescape.
 */
InventoryState::InventoryState(bool tu, BattlescapeState *parent) : _tu(tu), _parent(parent)
{
	_battleGame = _game->getSavedGame()->getSavedBattle();

	if (Options::maximizeInfoScreens)
	{
		Options::baseXResolution = Screen::ORIGINAL_WIDTH;
		Options::baseYResolution = Screen::ORIGINAL_HEIGHT;
		_game->getScreen()->resetDisplay(false);
	}
	else if (!_battleGame->getTileEngine())
	{
		Screen::updateScale(Options::battlescapeScale, Options::battlescapeScale, Options::baseXBattlescape, Options::baseYBattlescape, true);
		_game->getScreen()->resetDisplay(false);
	}

	// Create objects (size x, size y, coordinate x, coordinate y)
	_bg = new Surface(1280, 800, 0, 0);
	_soldier = new Surface(121, 215, 109, 115);
	_rank = new Surface(26, 23, 5, 5); //added for the rank icon
	_txtName = new Text(400, 17, 40, 10);
	_txtItem = new Text(160, 15, 8, 310);
	_txtAmmo = new Text(80, 24, 8, 432);
	_btnPrev = new BattlescapeButton(23, 22, 1180, 5);
	_btnOk = new BattlescapeButton(35, 22, 1208, 5);
	_btnNext = new BattlescapeButton(23, 22, 1248, 5);
	_btnUnload = new BattlescapeButton(32, 25, 17, 366);
	_btnGround = new BattlescapeButton(32, 15, 1244, 673);
//	_btnRank = new BattlescapeButton(26, 23, 0, 0);
	_btnCreateTemplate = new BattlescapeButton(32, 22, _templateBtnX, _createTemplateBtnY);
	_btnApplyTemplate = new BattlescapeButton(32, 22, _templateBtnX, _applyTemplateBtnY);
	_selAmmo = new Surface(RuleInventory::HAND_W * RuleInventory::SLOT_W, RuleInventory::HAND_H * RuleInventory::SLOT_H, 8, 325);
	_inv = new Inventory(_game, 1280, 800, 0, 0, _parent == 0);
	_txtRank = new Text(100, 14, 10, 38); // added
	_txtMissions = new Text(100, 14, 135, 38); // added
	_txtKills = new Text(100, 14, 255, 38); // added
	_txtStatus = new Text(100, 14, 380, 38); // added
	_txtCraft = new Text(150, 14, 505, 38); // added
	_txtArmor = new Text(150, 14, 630, 38); // added

	_txtTimeUnits = new Text(120, 14, 803, 87);
	_numTimeUnits = new Text(18, 14, 936, 87);
	_barTimeUnits = new Bar(200, 8, 955, 87);

	_txtEnergy = new Text(120, 14, 803, 102);
	_numEnergy = new Text(18, 14, 936, 102);
	_barEnergy = new Bar(200, 8, 955, 102);

	_txtHealth = new Text(120, 14, 803, 117);
	_numHealth = new Text(18, 14, 936, 117);
	_barHealth = new Bar(200, 8, 955, 117);

	_txtFatalWounds = new Text(120, 14, 803, 132);
	_numFatalWounds = new Text(18, 14, 936, 132);
	_barFatalWounds = new Bar(200, 8, 955, 132);

	_txtBravery = new Text(120, 14, 803, 147);
	_numBravery = new Text(18, 14, 936, 147);
	_barBravery = new Bar(200, 8, 955, 147);

	_txtMorale = new Text(120, 14, 803, 162);
	_numMorale = new Text(18, 14, 936, 162);
	_barMorale = new Bar(200, 8, 955, 162);

	_txtReactions = new Text(120, 14, 803, 177);
	_numReactions = new Text(18, 14, 936, 177);
	_barReactions = new Bar(200, 8, 955, 177);

	_txtFiring = new Text(120, 14, 803, 192);
	_numFiring = new Text(18, 14, 936, 192);
	_barFiring = new Bar(200, 8, 955, 192);

	_txtThrowing = new Text(120, 14, 803, 207);
	_numThrowing = new Text(18, 14, 936, 207);
	_barThrowing = new Bar(200, 8, 955, 207);

	_txtMelee = new Text(120, 14, 803, 222);
	_numMelee = new Text(18, 14, 936, 222);
	_barMelee = new Bar(200, 8, 955, 222);

	_txtStrength = new Text(120, 14, 803, 237);
	_numStrength = new Text(18, 14, 936, 237);
	_barStrength = new Bar(200, 8, 955, 237);

	_txtPsiStrength = new Text(120, 14, 803, 252);
	_numPsiStrength = new Text(18, 14, 936, 252);
	_barPsiStrength = new Bar(200, 8, 955, 252);

	_txtPsiSkill = new Text(120, 14, 803, 267);
	_numPsiSkill = new Text(18, 14, 936, 267);
	_barPsiSkill = new Bar(200, 8, 955, 267);

	_txtFrontArmor = new Text(120, 14, 803, 297);
	_numFrontArmor = new Text(18, 14, 936, 297);
	_barFrontArmor = new Bar(200, 8, 955, 297);

	_txtLeftArmor = new Text(120, 14, 803, 312);
	_numLeftArmor = new Text(18, 14, 936, 312);
	_barLeftArmor = new Bar(200, 8, 955, 312);

	_txtRightArmor = new Text(120, 14, 803, 327);
	_numRightArmor = new Text(18, 14, 936, 327);
	_barRightArmor = new Bar(200, 8, 955, 327);

	_txtRearArmor = new Text(120, 14, 803, 342);
	_numRearArmor = new Text(18, 14, 936, 342);
	_barRearArmor = new Bar(200, 8, 955, 342);

	_txtUnderArmor = new Text(120, 14, 803, 357);
	_numUnderArmor = new Text(18, 14, 936, 357);
	_barUnderArmor = new Bar(200, 8, 955, 357);

	if (!_mindProbe)
	{
		_btnPrev = new BattlescapeButton(14, 18, 2, 2);
		_btnNext = new BattlescapeButton(14, 18, 304, 2);
	}
	// Set palette
	setPalette("PAL_BATTLESCAPE");

	add(_bg);

	// Set up objects
	_game->getMod()->getSurface("TAC01.SCR")->blit(_bg);

	add(_soldier);
	add(_rank, "rank", "inventory", _bg);
	add(_txtName, "textName", "inventory", _bg);

	add(_txtItem, "textItem", "inventory", _bg);
	add(_txtAmmo, "textAmmo", "inventory", _bg);
	add(_btnOk, "buttonOK", "inventory", _bg);
	add(_btnPrev, "buttonPrev", "inventory", _bg);
	add(_btnNext, "buttonNext", "inventory", _bg);
	add(_btnUnload, "buttonUnload", "inventory", _bg);
	add(_btnGround, "buttonGround", "inventory", _bg);
//	add(_btnRank, "rank", "inventory", _bg);
	add(_btnCreateTemplate, "buttonCreate", "inventory", _bg);
	add(_btnApplyTemplate, "buttonApply", "inventory", _bg);
	add(_selAmmo);
	add(_inv);

	// Objects for information under name field
	add(_txtRank); 
	add(_txtCraft); 
	add(_txtMissions); 
	add(_txtKills); 
	add(_txtStatus); 
	add(_txtArmor); 

	//Adding all Items for Soldier Info
	add(_txtTimeUnits);
	add(_numTimeUnits);
	add(_barTimeUnits, "barTUs", "stats", 0);

	add(_txtEnergy);
	add(_numEnergy);
	add(_barEnergy, "barEnergy", "stats", 0);

	add(_txtHealth);
	add(_numHealth);
	add(_barHealth, "barHealth", "stats", 0);

	add(_txtFatalWounds);
	add(_numFatalWounds);
	add(_barFatalWounds, "barWounds", "stats", 0);

	add(_txtBravery);
	add(_numBravery);
	add(_barBravery, "barBravery", "stats", 0);

	add(_txtMorale);
	add(_numMorale);
	add(_barMorale, "barMorale", "stats", 0);

	add(_txtReactions);
	add(_numReactions);
	add(_barReactions, "barReactions", "stats", 0);

	add(_txtFiring);
	add(_numFiring);
	add(_barFiring, "barFiring", "stats", 0);

	add(_txtThrowing);
	add(_numThrowing);
	add(_barThrowing, "barThrowing", "stats", 0);

	add(_txtMelee);
	add(_numMelee);
	add(_barMelee, "barMelee", "stats", 0);

	add(_txtStrength);
	add(_numStrength);
	add(_barStrength, "barStrength", "stats", 0);

	add(_txtPsiStrength);
	add(_numPsiStrength);
	add(_barPsiStrength, "barPsiStrength", "stats", 0);

	add(_txtPsiSkill);
	add(_numPsiSkill);
	add(_barPsiSkill, "barPsiSkill", "stats", 0);

	add(_txtFrontArmor);
	add(_numFrontArmor);
	add(_barFrontArmor, "barFrontArmor", "stats", 0);

	add(_txtLeftArmor);
	add(_numLeftArmor);
	add(_barLeftArmor, "barLeftArmor", "stats", 0);

	add(_txtRightArmor);
	add(_numRightArmor);
	add(_barRightArmor, "barRightArmor", "stats", 0);

	add(_txtRearArmor);
	add(_numRearArmor);
	add(_barRearArmor, "barRearArmor", "stats", 0);

	add(_txtUnderArmor);
	add(_numUnderArmor);
	add(_barUnderArmor, "barUnderArmor", "stats", 0);

	//centerAllSurfaces();

	// Set up objects
	_game->getMod()->getSurface("HDTAC01.PNG")->blit(_bg);

	_txtName->setColor(Palette::blockOffset(4));
	_txtName->setBig();
	_txtName->setHighContrast(true);

	_txtItem->setHighContrast(true);

	_txtAmmo->setColor(Palette::blockOffset(4));
	_txtAmmo->setSecondaryColor(Palette::blockOffset(1));
	_txtAmmo->setAlign(ALIGN_LEFT);
	_txtAmmo->setHighContrast(true);

	_btnOk->onMouseClick((ActionHandler)&InventoryState::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&InventoryState::btnOkClick, Options::keyCancel);
	_btnOk->onKeyboardPress((ActionHandler)&InventoryState::btnOkClick, Options::keyBattleInventory);
	_btnOk->setTooltip("STR_OK");
	_btnOk->onMouseIn((ActionHandler)&InventoryState::txtTooltipIn);
	_btnOk->onMouseOut((ActionHandler)&InventoryState::txtTooltipOut);

	_btnPrev->onMouseClick((ActionHandler)&InventoryState::btnPrevClick);
	_btnPrev->onKeyboardPress((ActionHandler)&InventoryState::btnPrevClick, Options::keyBattlePrevUnit);
	_btnPrev->setTooltip("STR_PREVIOUS_UNIT");
	_btnPrev->onMouseIn((ActionHandler)&InventoryState::txtTooltipIn);
	_btnPrev->onMouseOut((ActionHandler)&InventoryState::txtTooltipOut);

	_btnNext->onMouseClick((ActionHandler)&InventoryState::btnNextClick);
	_btnNext->onKeyboardPress((ActionHandler)&InventoryState::btnNextClick, Options::keyBattleNextUnit);
	_btnNext->setTooltip("STR_NEXT_UNIT");
	_btnNext->onMouseIn((ActionHandler)&InventoryState::txtTooltipIn);
	_btnNext->onMouseOut((ActionHandler)&InventoryState::txtTooltipOut);

	_btnUnload->onMouseClick((ActionHandler)&InventoryState::btnUnloadClick);
	_btnUnload->setTooltip("STR_UNLOAD_WEAPON");
	_btnUnload->onMouseIn((ActionHandler)&InventoryState::txtTooltipIn);
	_btnUnload->onMouseOut((ActionHandler)&InventoryState::txtTooltipOut);

	_btnGround->onMouseClick((ActionHandler)&InventoryState::btnGroundClick);
	_btnGround->setTooltip("STR_SCROLL_RIGHT");
	_btnGround->onMouseIn((ActionHandler)&InventoryState::txtTooltipIn);
	_btnGround->onMouseOut((ActionHandler)&InventoryState::txtTooltipOut);


	_btnCreateTemplate->onMouseClick((ActionHandler)&InventoryState::btnCreateTemplateClick);
	_btnCreateTemplate->onKeyboardPress((ActionHandler)&InventoryState::btnCreateTemplateClick, Options::keyInvCreateTemplate);
	_btnCreateTemplate->setTooltip("STR_CREATE_INVENTORY_TEMPLATE");
	_btnCreateTemplate->onMouseIn((ActionHandler)&InventoryState::txtTooltipIn);
	_btnCreateTemplate->onMouseOut((ActionHandler)&InventoryState::txtTooltipOut);

	_btnApplyTemplate->onMouseClick((ActionHandler)&InventoryState::btnApplyTemplateClick);
	_btnApplyTemplate->onKeyboardPress((ActionHandler)&InventoryState::btnApplyTemplateClick, Options::keyInvApplyTemplate);
	_btnApplyTemplate->onKeyboardPress((ActionHandler)&InventoryState::onClearInventory, Options::keyInvClear);
	_btnApplyTemplate->setTooltip("STR_APPLY_INVENTORY_TEMPLATE");
	_btnApplyTemplate->onMouseIn((ActionHandler)&InventoryState::txtTooltipIn);
	_btnApplyTemplate->onMouseOut((ActionHandler)&InventoryState::txtTooltipOut);

	Uint8 color = _game->getMod()->getInterface("stats")->getElement("text")->color;
	Uint8 color2 = _game->getMod()->getInterface("stats")->getElement("text")->color2;

	_txtTimeUnits->setColor(color);
	_txtTimeUnits->setHighContrast(true);
	_txtTimeUnits->setText(tr("STR_TIME_UNITS"));

	_numTimeUnits->setColor(color2);
	_numTimeUnits->setHighContrast(true);

	_barTimeUnits->setScale(1.0);

	_txtEnergy->setColor(color);
	_txtEnergy->setHighContrast(true);
	_txtEnergy->setText(tr("STR_ENERGY"));

	_numEnergy->setColor(color2);
	_numEnergy->setHighContrast(true);

	_barEnergy->setScale(1.0);

	_txtHealth->setColor(color);
	_txtHealth->setHighContrast(true);
	_txtHealth->setText(tr("STR_HEALTH"));

	_numHealth->setColor(color2);
	_numHealth->setHighContrast(true);

	_barHealth->setScale(1.0);

	_txtFatalWounds->setColor(color);
	_txtFatalWounds->setHighContrast(true);
	_txtFatalWounds->setText(tr("STR_FATAL_WOUNDS"));

	_numFatalWounds->setColor(color2);
	_numFatalWounds->setHighContrast(true);

	_barFatalWounds->setScale(1.0);

	_txtBravery->setColor(color);
	_txtBravery->setHighContrast(true);
	_txtBravery->setText(tr("STR_BRAVERY"));

	_numBravery->setColor(color2);
	_numBravery->setHighContrast(true);

	_barBravery->setScale(1.0);

	_txtMorale->setColor(color);
	_txtMorale->setHighContrast(true);
	_txtMorale->setText(tr("STR_MORALE"));

	_numMorale->setColor(color2);
	_numMorale->setHighContrast(true);

	_barMorale->setScale(1.0);

	_txtReactions->setColor(color);
	_txtReactions->setHighContrast(true);
	_txtReactions->setText(tr("STR_REACTIONS"));

	_numReactions->setColor(color2);
	_numReactions->setHighContrast(true);

	_barReactions->setScale(1.0);

	_txtFiring->setColor(color);
	_txtFiring->setHighContrast(true);
	_txtFiring->setText(tr("STR_FIRING_ACCURACY"));

	_numFiring->setColor(color2);
	_numFiring->setHighContrast(true);

	_barFiring->setScale(1.0);

	_txtThrowing->setColor(color);
	_txtThrowing->setHighContrast(true);
	_txtThrowing->setText(tr("STR_THROWING_ACCURACY"));

	_numThrowing->setColor(color2);
	_numThrowing->setHighContrast(true);

	_barThrowing->setScale(1.0);

	_txtMelee->setColor(color);
	_txtMelee->setHighContrast(true);
	_txtMelee->setText(tr("STR_MELEE_ACCURACY"));

	_numMelee->setColor(color2);
	_numMelee->setHighContrast(true);

	_barMelee->setScale(1.0);

	_txtStrength->setColor(color);
	_txtStrength->setHighContrast(true);
	_txtStrength->setText(tr("STR_STRENGTH"));

	_numStrength->setColor(color2);
	_numStrength->setHighContrast(true);

	_barStrength->setScale(1.0);

	_txtPsiStrength->setColor(color);
	_txtPsiStrength->setHighContrast(true);
	_txtPsiStrength->setText(tr("STR_PSIONIC_STRENGTH"));

	_numPsiStrength->setColor(color2);
	_numPsiStrength->setHighContrast(true);

	_barPsiStrength->setScale(1.0);

	_txtPsiSkill->setColor(color);
	_txtPsiSkill->setHighContrast(true);
	_txtPsiSkill->setText(tr("STR_PSIONIC_SKILL"));

	_numPsiSkill->setColor(color2);
	_numPsiSkill->setHighContrast(true);

	_barPsiSkill->setScale(1.0);

	_txtFrontArmor->setColor(color);
	_txtFrontArmor->setHighContrast(true);
	_txtFrontArmor->setText(tr("STR_FRONT_ARMOR_UC"));

	_numFrontArmor->setColor(color2);
	_numFrontArmor->setHighContrast(true);

	_barFrontArmor->setScale(1.0);

	_txtLeftArmor->setColor(color);
	_txtLeftArmor->setHighContrast(true);
	_txtLeftArmor->setText(tr("STR_LEFT_ARMOR_UC"));

	_numLeftArmor->setColor(color2);
	_numLeftArmor->setHighContrast(true);

	_barLeftArmor->setScale(1.0);

	_txtRightArmor->setColor(color);
	_txtRightArmor->setHighContrast(true);
	_txtRightArmor->setText(tr("STR_RIGHT_ARMOR_UC"));

	_numRightArmor->setColor(color2);
	_numRightArmor->setHighContrast(true);

	_barRightArmor->setScale(1.0);

	_txtRearArmor->setColor(color);
	_txtRearArmor->setHighContrast(true);
	_txtRearArmor->setText(tr("STR_REAR_ARMOR_UC"));

	_numRearArmor->setColor(color2);
	_numRearArmor->setHighContrast(true);

	_barRearArmor->setScale(1.0);

	_txtUnderArmor->setColor(color);
	_txtUnderArmor->setHighContrast(true);
	_txtUnderArmor->setText(tr("STR_UNDER_ARMOR_UC"));

	_numUnderArmor->setColor(color2);
	_numUnderArmor->setHighContrast(true);

	_barUnderArmor->setScale(1.0);

	if (!_mindProbe)
	{
		_btnPrev = new BattlescapeButton(14, 18, 2, 2);
		_btnNext = new BattlescapeButton(14, 18, 304, 2);
	}

	// only use copy/paste buttons in setup (i.e. non-tu) mode
	if (_tu)
	{
		_btnCreateTemplate->setVisible(false);
		_btnApplyTemplate->setVisible(false);
	}
	else
	{
		_updateTemplateButtons(true);
	}

	_inv->draw();
	_inv->setTuMode(_tu);
	_inv->setSelectedUnit(_game->getSavedGame()->getSavedBattle()->getSelectedUnit());
	_inv->onMouseClick((ActionHandler)&InventoryState::invClick, 0);
	_inv->onMouseOver((ActionHandler)&InventoryState::invMouseOver);
	_inv->onMouseOut((ActionHandler)&InventoryState::invMouseOut);

	_currentTooltip = "";

	//Setup object for the top row labels of stats
	_txtRank->setColor(color);
	_txtRank->setSecondaryColor(color2);
	_txtRank->setHighContrast(true);

	_txtCraft->setColor(color);
	_txtCraft->setSecondaryColor(color2);
	_txtCraft->setHighContrast(true);

	_txtMissions->setColor(color);
	_txtMissions->setSecondaryColor(color2);
	_txtMissions->setHighContrast(true);

	_txtKills->setColor(color);
	_txtKills->setSecondaryColor(color2);
	_txtKills->setHighContrast(true);

	_txtStatus->setColor(color);
	_txtStatus->setSecondaryColor(color2);
	_txtStatus->setHighContrast(true);

	_txtArmor->setColor(color);
	_txtArmor->setSecondaryColor(color2);
	_txtArmor->setHighContrast(true);
}

static void _clearInventoryTemplate(std::vector<EquipmentLayoutItem*> &inventoryTemplate)
{
	for (std::vector<EquipmentLayoutItem*>::iterator eraseIt = inventoryTemplate.begin();
		 eraseIt != inventoryTemplate.end();
		 eraseIt = inventoryTemplate.erase(eraseIt))
	{
		delete *eraseIt;
	}
}

/**
 *
 */
InventoryState::~InventoryState()
{
	_clearInventoryTemplate(_curInventoryTemplate);

	if (_battleGame->getTileEngine())
	{
		if (Options::maximizeInfoScreens)
		{
			Screen::updateScale(Options::battlescapeScale, Options::battlescapeScale, Options::baseXBattlescape, Options::baseYBattlescape, true);
			_game->getScreen()->resetDisplay(false);
		}
		Tile *inventoryTile = _battleGame->getSelectedUnit()->getTile();
		_battleGame->getTileEngine()->applyGravity(inventoryTile);
		_battleGame->getTileEngine()->calculateTerrainLighting(); // dropping/picking up flares
		_battleGame->getTileEngine()->recalculateFOV();
	}
	else
	{
		Screen::updateScale(Options::geoscapeScale, Options::geoscapeScale, Options::baseXGeoscape, Options::baseYGeoscape, true);
		_game->getScreen()->resetDisplay(false);
	}
}

/**
 * Updates all soldier stats when the soldier changes.
 */
void InventoryState::init()
{
	State::init();
	BattleUnit *unit = _battleGame->getSelectedUnit();

	// no selected unit, close inventory
	if (unit == 0)
	{
		btnOkClick(0);
		return;
	}
	// skip to the first unit with inventory
	if (!unit->hasInventory())
	{
		if (_parent)
		{
			_parent->selectNextPlayerUnit(false, false, true);
		}
		else
		{
			_battleGame->selectNextPlayerUnit(false, false, true);
		}
		// no available unit, close inventory
		if (_battleGame->getSelectedUnit() == 0 || !_battleGame->getSelectedUnit()->hasInventory())
		{
			// starting a mission with just vehicles
			btnOkClick(0);
			return;
		}
		else
		{
			unit = _battleGame->getSelectedUnit();
		}
	}

	unit->setCache(0);
	_soldier->clear();


	_txtName->setBig();
	_txtName->setText(unit->getName(_game->getLanguage()));
	_inv->setSelectedUnit(unit);
	Soldier *s = unit->getGeoscapeSoldier();
	if (s)
	{
		SurfaceSet *texture = _game->getMod()->getSurfaceSet("SMOKE.PCK");
		texture->getFrame(20 + s->getRank())->setX(0);
		texture->getFrame(20 + s->getRank())->setY(0);
		texture->getFrame(20 + s->getRank())->blit(_rank);

		std::string look = s->getArmor()->getSpriteInventory();
		if (s->getGender() == GENDER_MALE)
			look += "M";
		else
			look += "F";
		if (s->getLook() == LOOK_BLONDE)
			look += "0";
		if (s->getLook() == LOOK_BROWNHAIR)
			look += "1";
		if (s->getLook() == LOOK_ORIENTAL)
			look += "2";
		if (s->getLook() == LOOK_AFRICAN)
			look += "3";
		look += ".SPK";
		const std::set<std::string> &ufographContents = FileMap::getVFolderContents("UFOGRAPH");
		std::string lcaseLook = look;
		std::transform(lcaseLook.begin(), lcaseLook.end(), lcaseLook.begin(), tolower);
		if (ufographContents.find("lcaseLook") == ufographContents.end() && !_game->getMod()->getSurface(look))
		{
			look = s->getArmor()->getSpriteInventory() + ".SPK";
		}
		_game->getMod()->getSurface(look)->blit(_soldier);
	}
	else
	{
		Surface *armorSurface = _game->getMod()->getSurface(unit->getArmor()->getSpriteInventory());
		if (armorSurface)
		{
			armorSurface->blit(_soldier);
		}
	}

	updateStats();
	_refreshMouse();
}

/**
 * Updates the soldier stats (Weight, TU).
 */
void InventoryState::updateStats()
{
	BattleUnit *unit = _battleGame->getSelectedUnit();

	// Adding info menu  
	_txtRank->setText(tr("STR_RANK_").arg(tr(unit->getRankString())));
	_txtCraft->setText(tr("STR_CRAFT_").arg(unit->getGeoscapeSoldier()->getCraftString(_game->getLanguage())));
	_txtMissions->setText(tr("STR_MISSIONS").arg(unit->getGeoscapeSoldier()->getMissions()));
	_txtKills->setText(tr("STR_KILLS").arg(unit->getGeoscapeSoldier()->getKills()));
	_txtArmor->setText(tr("STR_ARMOR").arg(unit->getGeoscapeSoldier()->getArmor()));
//	_txtStatus->setText(tr("STR_STATUS").arg(unit->getGeoscapeSoldier()->getStatus())); want to add healthy, wounded, bleeding, unconcious, dead 


	// Adding text for all bars
	std::wostringstream ss;
	ss << unit->getTimeUnits();
	_numTimeUnits->setText(ss.str());
	_barTimeUnits->setMax(unit->getBaseStats()->tu);
	_barTimeUnits->setValue(unit->getTimeUnits());

	ss.str(L"");
	// aliens have their rank in their "name", soldiers don't
	if (unit->getType() == "SOLDIER")
	{
		ss << tr(unit->getRankString());
		ss << " ";
/**		_txtTus->setText(tr("STR_TIME_UNITS_SHORT").arg(unit->getTimeUnits()));

		int weight = unit->getCarriedWeight(_inv->getSelectedItem());
		_txtWeight->setText(tr("STR_WEIGHT").arg(weight).arg(unit->getBaseStats()->strength));
		if (weight > unit->getBaseStats()->strength)
		{
			_txtWeight->setSecondaryColor(_game->getMod()->getInterface("inventory")->getElement("weight")->color2);
		}
		else
		{
			_txtWeight->setSecondaryColor(_game->getMod()->getInterface("inventory")->getElement("weight")->color);
		} **/
		ss << unit->getName(_game->getLanguage(), BattlescapeGame::_debugPlay);
		_txtName->setBig();
		_txtName->setText(ss.str());

		ss.str(L"");
		ss << unit->getEnergy();
		_numEnergy->setText(ss.str());
		_barEnergy->setMax(unit->getBaseStats()->stamina);
		_barEnergy->setValue(unit->getEnergy());

		ss.str(L"");
		ss << unit->getHealth();
		_numHealth->setText(ss.str());
		_barHealth->setMax(unit->getBaseStats()->health);
		_barHealth->setValue(unit->getHealth());
		_barHealth->setValue2(unit->getStunlevel());

		ss.str(L"");
		ss << unit->getFatalWounds();
		_numFatalWounds->setText(ss.str());
		_barFatalWounds->setMax(unit->getFatalWounds());
		_barFatalWounds->setValue(unit->getFatalWounds());

		ss.str(L"");
		ss << unit->getBaseStats()->bravery;
		_numBravery->setText(ss.str());
		_barBravery->setMax(unit->getBaseStats()->bravery);
		_barBravery->setValue(unit->getBaseStats()->bravery);

		ss.str(L"");
		ss << unit->getMorale();
		_numMorale->setText(ss.str());
		_barMorale->setMax(100);
		_barMorale->setValue(unit->getMorale());

		ss.str(L"");
		ss << unit->getBaseStats()->reactions;
		_numReactions->setText(ss.str());
		_barReactions->setMax(unit->getBaseStats()->reactions);
		_barReactions->setValue(unit->getBaseStats()->reactions);

		ss.str(L"");
		ss << (int)((unit->getBaseStats()->firing * unit->getHealth()) / unit->getBaseStats()->health);
		_numFiring->setText(ss.str());
		_barFiring->setMax(unit->getBaseStats()->firing);
		_barFiring->setValue((unit->getBaseStats()->firing * unit->getHealth()) / unit->getBaseStats()->health);

		ss.str(L"");
		ss << (int)((unit->getBaseStats()->throwing * unit->getHealth()) / unit->getBaseStats()->health);
		_numThrowing->setText(ss.str());
		_barThrowing->setMax(unit->getBaseStats()->throwing);
		_barThrowing->setValue((unit->getBaseStats()->throwing * unit->getHealth()) / unit->getBaseStats()->health);

		ss.str(L"");
		ss << (int)((unit->getBaseStats()->melee * unit->getHealth()) / unit->getBaseStats()->health);
		_numMelee->setText(ss.str());
		_barMelee->setMax(unit->getBaseStats()->melee);
		_barMelee->setValue((unit->getBaseStats()->melee * unit->getHealth()) / unit->getBaseStats()->health);

		ss.str(L"");
		ss << unit->getBaseStats()->strength;
		_numStrength->setText(ss.str());
		_barStrength->setMax(unit->getBaseStats()->strength);
		_barStrength->setValue(unit->getBaseStats()->strength);

		if (unit->getBaseStats()->psiSkill > 0 || (Options::psiStrengthEval && _game->getSavedGame()->isResearched(_game->getMod()->getPsiRequirements())))
		{
			ss.str(L"");
			ss << unit->getBaseStats()->psiStrength;
			_numPsiStrength->setText(ss.str());
			_barPsiStrength->setMax(unit->getBaseStats()->psiStrength);
			_barPsiStrength->setValue(unit->getBaseStats()->psiStrength);

			_txtPsiStrength->setVisible(true);
			_numPsiStrength->setVisible(true);
			_barPsiStrength->setVisible(true);
			_txtFAcc->setText(tr("STR_ACCURACY_SHORT").arg((int)(unit->getBaseStats()->firing * unit->getHealth()) / unit->getBaseStats()->health));

			_txtReact->setText(tr("STR_REACTIONS_SHORT").arg(unit->getBaseStats()->reactions));

			if (unit->getBaseStats()->psiSkill > 0)
			{
				_txtPSkill->setText(tr("STR_PSIONIC_SKILL_SHORT").arg(unit->getBaseStats()->psiSkill));
			}
			else
			{
				//		_txtPSkill->setText(L"");
				_txtPsiStrength->setVisible(false);
				_numPsiStrength->setVisible(false);
				_barPsiStrength->setVisible(false);
			}

			if (unit->getBaseStats()->psiSkill > 0 || (Options::psiStrengthEval && _game->getSavedGame()->isResearched(_game->getMod()->getPsiRequirements())))
			{
				ss.str(L"");
				ss << unit->getBaseStats()->psiSkill;
				_numPsiSkill->setText(ss.str());
				_barPsiSkill->setMax(unit->getBaseStats()->psiSkill);
				_barPsiSkill->setValue(unit->getBaseStats()->psiSkill);

				_txtPsiSkill->setVisible(true);
				_numPsiSkill->setVisible(true);
				_barPsiSkill->setVisible(true);
				_txtPStr->setText(tr("STR_PSIONIC_STRENGTH_SHORT").arg(unit->getBaseStats()->psiStrength));
			}
			else
			{
				//		_txtPStr->setText(L"");
				_txtPsiSkill->setVisible(false);
				_numPsiSkill->setVisible(false);
				_barPsiSkill->setVisible(false);
			}

			ss.str(L"");
			ss << unit->getArmor(SIDE_FRONT);
			_numFrontArmor->setText(ss.str());
			_barFrontArmor->setMax(unit->getArmor()->getFrontArmor());
			_barFrontArmor->setValue(unit->getArmor(SIDE_FRONT));

			ss.str(L"");
			ss << unit->getArmor(SIDE_LEFT);
			_numLeftArmor->setText(ss.str());
			_barLeftArmor->setMax(unit->getArmor()->getSideArmor());
			_barLeftArmor->setValue(unit->getArmor(SIDE_LEFT));

			ss.str(L"");
			ss << unit->getArmor(SIDE_RIGHT);
			_numRightArmor->setText(ss.str());
			_barRightArmor->setMax(unit->getArmor()->getSideArmor());
			_barRightArmor->setValue(unit->getArmor(SIDE_RIGHT));

			ss.str(L"");
			ss << unit->getArmor(SIDE_REAR);
			_numRearArmor->setText(ss.str());
			_barRearArmor->setMax(unit->getArmor()->getRearArmor());
			_barRearArmor->setValue(unit->getArmor(SIDE_REAR));

			ss.str(L"");
			ss << unit->getArmor(SIDE_UNDER);
			_numUnderArmor->setText(ss.str());
			_barUnderArmor->setMax(unit->getArmor()->getUnderArmor());
			_barUnderArmor->setValue(unit->getArmor(SIDE_UNDER));
		}
	}
}

/**
 * Saves the soldiers' equipment-layout.
 */
void InventoryState::saveEquipmentLayout()
{
	for (std::vector<BattleUnit*>::iterator i = _battleGame->getUnits()->begin(); i != _battleGame->getUnits()->end(); ++i)
	{
		// we need X-Com soldiers only
		if ((*i)->getGeoscapeSoldier() == 0) continue;

		std::vector<EquipmentLayoutItem*> *layoutItems = (*i)->getGeoscapeSoldier()->getEquipmentLayout();

		// clear the previous save
		if (!layoutItems->empty())
		{
			for (std::vector<EquipmentLayoutItem*>::iterator j = layoutItems->begin(); j != layoutItems->end(); ++j)
				delete *j;
			layoutItems->clear();
		}

		// save the soldier's items
		// note: with using getInventory() we are skipping the ammos loaded, (they're not owned) because we handle the loaded-ammos separately (inside)
		for (std::vector<BattleItem*>::iterator j = (*i)->getInventory()->begin(); j != (*i)->getInventory()->end(); ++j)
		{
			std::string ammo;
			if ((*j)->needsAmmo() && 0 != (*j)->getAmmoItem()) ammo = (*j)->getAmmoItem()->getRules()->getType();
			else ammo = "NONE";
			layoutItems->push_back(new EquipmentLayoutItem(
				(*j)->getRules()->getType(),
				(*j)->getSlot()->getId(),
				(*j)->getSlotX(),
				(*j)->getSlotY(),
				ammo,
				(*j)->getFuseTimer()
			));
		}
	}
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void InventoryState::btnOkClick(Action *)
{
	if (_inv->getSelectedItem() != 0)
		return;
	_game->popState();
	Tile *inventoryTile = _battleGame->getSelectedUnit()->getTile();
	if (!_tu)
	{
		saveEquipmentLayout();
		_battleGame->resetUnitTiles();
		if (_battleGame->getTurn() == 1)
		{
			_battleGame->randomizeItemLocations(inventoryTile);
			if (inventoryTile->getUnit())
			{
				// make sure we select the unit closest to the ramp.
				_battleGame->setSelectedUnit(inventoryTile->getUnit());
			}
		}

		// initialize xcom units for battle
		for (std::vector<BattleUnit*>::iterator j = _battleGame->getUnits()->begin(); j != _battleGame->getUnits()->end(); ++j)
		{
			if ((*j)->getOriginalFaction() != FACTION_PLAYER || (*j)->isOut())
			{
				continue;
			}

			(*j)->prepareNewTurn();
		}
	}
}

/**
 * Selects the previous soldier.
 * @param action Pointer to an action.
 */
void InventoryState::btnPrevClick(Action *)
{
	if (_inv->getSelectedItem() != 0)
	{
		return;
	}

	if (_parent)
	{
		_parent->selectPreviousPlayerUnit(false, false, true);
	}
	else
	{
		_battleGame->selectPreviousPlayerUnit(false, false, true);
	}
	init();
}

/**
 * Selects the next soldier.
 * @param action Pointer to an action.
 */
void InventoryState::btnNextClick(Action *)
{
	if (_inv->getSelectedItem() != 0)
	{
		return;
	}
	if (_parent)
	{
		_parent->selectNextPlayerUnit(false, false, true);
	}
	else
	{
		_battleGame->selectNextPlayerUnit(false, false, true);
	}
	init();
}

/**
 * Unloads the selected weapon.
 * @param action Pointer to an action.
 */
void InventoryState::btnUnloadClick(Action *)
{
	if (_inv->unload())
	{
		_txtItem->setText(L"");
		_txtAmmo->setText(L"");
		_selAmmo->clear();
		updateStats();
		_game->getMod()->getSoundByDepth(0, Mod::ITEM_DROP)->play();
	}
}

/**
 * Shows more ground items / rearranges them.
 * @param action Pointer to an action.
 */
void InventoryState::btnGroundClick(Action *)
{
	_inv->arrangeGround();
}

/**
 * Shows the unit info screen.
 * @param action Pointer to an action.
 */
void InventoryState::btnRankClick(Action *)
{
	_game->pushState(new UnitInfoState(_battleGame->getSelectedUnit(), _parent, true, false));
}

void InventoryState::btnCreateTemplateClick(Action *)
{
	// don't accept clicks when moving items
	if (_inv->getSelectedItem() != 0)
	{
		return;
	}

	// clear current template
	_clearInventoryTemplate(_curInventoryTemplate);

	// copy inventory instead of just keeping a pointer to it.  that way
	// create/apply can be used as an undo button for a single unit and will
	// also work as expected if inventory is modified after 'create' is clicked
	std::vector<BattleItem*> *unitInv = _battleGame->getSelectedUnit()->getInventory();
	for (std::vector<BattleItem*>::iterator j = unitInv->begin(); j != unitInv->end(); ++j)
	{
		std::string ammo;
		if ((*j)->needsAmmo() && (*j)->getAmmoItem())
		{
			ammo = (*j)->getAmmoItem()->getRules()->getType();
		}
		else
		{
			ammo = "NONE";
		}

		_curInventoryTemplate.push_back(new EquipmentLayoutItem(
				(*j)->getRules()->getType(),
				(*j)->getSlot()->getId(),
				(*j)->getSlotX(),
				(*j)->getSlotY(),
				ammo,
				(*j)->getFuseTimer()));
	}

	// give audio feedback
	_game->getMod()->getSoundByDepth(_battleGame->getDepth(), Mod::ITEM_DROP)->play();
	_refreshMouse();
}

static void _clearInventory(Game *game, std::vector<BattleItem*> *unitInv, Tile *groundTile)
{
	RuleInventory *groundRuleInv = game->getMod()->getInventory("STR_GROUND");

	// clear unit's inventory (i.e. move everything to the ground)
	for (std::vector<BattleItem*>::iterator i = unitInv->begin(); i != unitInv->end(); )
	{
		(*i)->setOwner(NULL);
		groundTile->addItem(*i, groundRuleInv);
		i = unitInv->erase(i);
	}
}

void InventoryState::btnApplyTemplateClick(Action *)
{
	// don't accept clicks when moving items
	// it's ok if the template is empty -- it will just result in clearing the
	// unit's inventory
	if (_inv->getSelectedItem() != 0)
	{
		return;
	}

	BattleUnit               *unit          = _battleGame->getSelectedUnit();
	std::vector<BattleItem*> *unitInv       = unit->getInventory();
	Tile                     *groundTile    = unit->getTile();
	std::vector<BattleItem*> *groundInv     = groundTile->getInventory();
	RuleInventory            *groundRuleInv = _game->getMod()->getInventory("STR_GROUND");

	_clearInventory(_game, unitInv, groundTile);

	// attempt to replicate inventory template by grabbing corresponding items
	// from the ground.  if any item is not found on the ground, display warning
	// message, but continue attempting to fulfill the template as best we can
	bool itemMissing = false;
	std::vector<EquipmentLayoutItem*>::iterator templateIt;
	for (templateIt = _curInventoryTemplate.begin(); templateIt != _curInventoryTemplate.end(); ++templateIt)
	{
		// search for template item in ground inventory
		std::vector<BattleItem*>::iterator groundItem;
		const bool needsAmmo = !_game->getMod()->getItem((*templateIt)->getItemType())->getCompatibleAmmo()->empty();
		bool found = false;
		bool rescan = true;
		while (rescan)
		{
			rescan = false;

			const std::string targetAmmo = (*templateIt)->getAmmoItem();
			BattleItem *matchedWeapon = NULL;
			BattleItem *matchedAmmo   = NULL;
			for (groundItem = groundInv->begin(); groundItem != groundInv->end(); ++groundItem)
			{
				// if we find the appropriate ammo, remember it for later for if we find
				// the right weapon but with the wrong ammo
				const std::string groundItemName = (*groundItem)->getRules()->getType();
				if (needsAmmo && targetAmmo == groundItemName)
				{
					matchedAmmo = *groundItem;
				}

				if ((*templateIt)->getItemType() == groundItemName)
				{
					// if the loaded ammo doesn't match the template item's,
					// remember the weapon for later and continue scanning
					BattleItem *loadedAmmo = (*groundItem)->getAmmoItem();
					if ((needsAmmo && loadedAmmo && targetAmmo != loadedAmmo->getRules()->getType())
					 || (needsAmmo && !loadedAmmo))
					{
						// remember the last matched weapon for simplicity (but prefer empty weapons if any are found)
						if (!matchedWeapon || matchedWeapon->getAmmoItem())
						{
							matchedWeapon = *groundItem;
						}
						continue;
					}

					// move matched item from ground to the appropriate inv slot
					(*groundItem)->setOwner(unit);
					(*groundItem)->setSlot(_game->getMod()->getInventory((*templateIt)->getSlot()));
					(*groundItem)->setSlotX((*templateIt)->getSlotX());
					(*groundItem)->setSlotY((*templateIt)->getSlotY());
					(*groundItem)->setFuseTimer((*templateIt)->getFuseTimer());
					unitInv->push_back(*groundItem);
					groundInv->erase(groundItem);
					found = true;
					break;
				}
			}

			// if we failed to find an exact match, but found unloaded ammo and
			// the right weapon, unload the target weapon, load the right ammo, and use it
			if (!found && matchedWeapon && (!needsAmmo || matchedAmmo))
			{
				// unload the existing ammo (if any) from the weapon
				BattleItem *loadedAmmo = matchedWeapon->getAmmoItem();
				if (loadedAmmo)
				{
					groundTile->addItem(loadedAmmo, groundRuleInv);
					matchedWeapon->setAmmoItem(NULL);
				}

				// load the correct ammo into the weapon
				if (matchedAmmo)
				{
					matchedWeapon->setAmmoItem(matchedAmmo);
					groundTile->removeItem(matchedAmmo);
				}

				// rescan and pick up the newly-loaded/unloaded weapon
				rescan = true;
			}
		}

		if (!found)
		{
			itemMissing = true;
		}
	}

	if (itemMissing)
	{
		_inv->showWarning(tr("STR_NOT_ENOUGH_ITEMS_FOR_TEMPLATE"));
	}

	// refresh ui
	_inv->arrangeGround(false);
	updateStats();
	_refreshMouse();

	// give audio feedback
	_game->getMod()->getSoundByDepth(_battleGame->getDepth(), Mod::ITEM_DROP)->play();
}

void InventoryState::_refreshMouse()
{
	// send a mouse motion event to refresh any hover actions
	int x, y;
	SDL_GetMouseState(&x, &y);
	SDL_WarpMouse(x+1, y);

	// move the mouse back to avoid cursor creep
	SDL_WarpMouse(x, y);
}

void InventoryState::onClearInventory(Action *)
{
	// don't accept clicks when moving items
	if (_inv->getSelectedItem() != 0)
	{
		return;
	}

	BattleUnit               *unit       = _battleGame->getSelectedUnit();
	std::vector<BattleItem*> *unitInv    = unit->getInventory();
	Tile                     *groundTile = unit->getTile();

	_clearInventory(_game, unitInv, groundTile);

	// refresh ui
	_inv->arrangeGround(false);
	updateStats();
	_refreshMouse();

	// give audio feedback
	_game->getMod()->getSoundByDepth(_battleGame->getDepth(), Mod::ITEM_DROP)->play();
}

/**
 * Updates item info.
 * @param action Pointer to an action.
 */
void InventoryState::invClick(Action *)
{
	updateStats();
}

/**
 * Shows item info.
 * @param action Pointer to an action.
 */
void InventoryState::invMouseOver(Action *)
{
	if (_inv->getSelectedItem() != 0)
	{
		return;
	}

	BattleItem *item = _inv->getMouseOverItem();
	if (item != 0)
	{
		if (item->getUnit() && item->getUnit()->getStatus() == STATUS_UNCONSCIOUS)
		{
			_txtItem->setText(item->getUnit()->getName(_game->getLanguage()));
		}
		else
		{
			if (_game->getSavedGame()->isResearched(item->getRules()->getRequirements()))
			{
				_txtItem->setText(tr(item->getRules()->getName()));
			}
			else
			{
				_txtItem->setText(tr("STR_ALIEN_ARTIFACT"));
			}
		}
		std::wstring s;
		if (item->getAmmoItem() != 0 && item->needsAmmo())
		{
			s = tr("STR_AMMO_ROUNDS_LEFT").arg(item->getAmmoItem()->getAmmoQuantity());
			SDL_Rect r;
			r.x = 0;
			r.y = 0;
			r.w = RuleInventory::HAND_W * RuleInventory::SLOT_W;
			r.h = RuleInventory::HAND_H * RuleInventory::SLOT_H;
			_selAmmo->drawRect(&r, _game->getMod()->getInterface("inventory")->getElement("grid")->color);
			r.x++;
			r.y++;
			r.w -= 2;
			r.h -= 2;
			_selAmmo->drawRect(&r, Palette::blockOffset(0)+15);
			item->getAmmoItem()->getRules()->drawHandSprite(_game->getMod()->getSurfaceSet("BIGOBS.PCK"), _selAmmo);
			_updateTemplateButtons(false);
		}
		else
		{
			_selAmmo->clear();
			_updateTemplateButtons(!_tu);
		}
		if (item->getAmmoQuantity() != 0 && item->needsAmmo())
		{
			s = tr("STR_AMMO_ROUNDS_LEFT").arg(item->getAmmoQuantity());
		}
		else if (item->getRules()->getBattleType() == BT_MEDIKIT)
		{
			s = tr("STR_MEDI_KIT_QUANTITIES_LEFT").arg(item->getPainKillerQuantity()).arg(item->getStimulantQuantity()).arg(item->getHealQuantity());
		}
		_txtAmmo->setText(s);
	}
	else
	{
		if (_currentTooltip.empty())
		{
			_txtItem->setText(L"");
		}
		_txtAmmo->setText(L"");
		_selAmmo->clear();
		_updateTemplateButtons(!_tu);
	}
}

/**
 * Hides item info.
 * @param action Pointer to an action.
 */
void InventoryState::invMouseOut(Action *)
{
	_txtItem->setText(L"");
	_txtAmmo->setText(L"");
	_selAmmo->clear();
	_updateTemplateButtons(!_tu);
}

/**
 * Takes care of any events from the core game engine.
 * @param action Pointer to an action.
 */
void InventoryState::handle(Action *action)
{
	State::handle(action);


#ifndef __MORPHOS__
	if (action->getDetails()->type == SDL_MOUSEBUTTONDOWN)
	{
		if (action->getDetails()->button.button == SDL_BUTTON_X1)
		{
			btnNextClick(action);
		}
		else if (action->getDetails()->button.button == SDL_BUTTON_X2)
		{
			btnPrevClick(action);
		}
	}
#endif
}

/**
* Shows a tooltip for the appropriate button.
* @param action Pointer to an action.
*/
void InventoryState::txtTooltipIn(Action *action)
{
	if (_inv->getSelectedItem() == 0 && Options::battleTooltips)
	{
		_currentTooltip = action->getSender()->getTooltip();
		_txtItem->setText(tr(_currentTooltip));
	}
}

/**
* Clears the tooltip text.
* @param action Pointer to an action.
*/
void InventoryState::txtTooltipOut(Action *action)
{
	if (_inv->getSelectedItem() == 0 && Options::battleTooltips)
	{
		if (_currentTooltip == action->getSender()->getTooltip())
		{
			_currentTooltip = "";
			_txtItem->setText(L"");
		}
	}
}

void InventoryState::_updateTemplateButtons(bool isVisible)
{
	if (isVisible)
	{
		if (_curInventoryTemplate.empty())
		{
			// use "empty template" icons
			_game->getMod()->getSurface("InvCopy")->blit(_btnCreateTemplate);
			_game->getMod()->getSurface("InvPasteEmpty")->blit(_btnApplyTemplate);
			_btnApplyTemplate->setTooltip("STR_CLEAR_INVENTORY");
		}
		else
		{
			// use "active template" icons
			_game->getMod()->getSurface("InvCopyActive")->blit(_btnCreateTemplate);
			_game->getMod()->getSurface("InvPaste")->blit(_btnApplyTemplate);
			_btnApplyTemplate->setTooltip("STR_APPLY_INVENTORY_TEMPLATE");
		}
		_btnCreateTemplate->initSurfaces();
		_btnApplyTemplate->initSurfaces();
	}
	else
	{
		_btnCreateTemplate->clear();
		_btnApplyTemplate->clear();
	}
}
}
