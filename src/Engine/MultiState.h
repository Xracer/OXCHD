/*
* Copyright 2010-2014 OpenXcom Developers.
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
#ifndef OPENXCOM_MULTISTATE_H
#define OPENXCOM_MULTISTATE_H

#include <vector>
#include "../Engine/State.h"

namespace OpenXcom
{
class Window;
class GeoscapeState;
class BasescapeState;
class BattlescapeState;
class BaseInfoState;
class MonthlyCostsState;
class StoresState;
class TransfersState;
class InventoryState;
class Base;
class Surface;
class MiniBaseView;
class TextButton;
class TextEdit;
class Text;
class Bar;
class TextList;

/**
* A game state that can contain multiple sub-states,
* to allow for concurrent running of multiple game states.
* Just serves to conveniently forward calls to all running states.
*/
class MultiState : public State
{
protected:
	std::vector<State*> _states;
public:
	/// Creates a new multi-state.
	MultiState()
	{
	}
	/// Cleans up the multi-state.
	~MultiState()
	{
		for (std::vector<State*>::iterator i = _states.begin(); i != _states.end(); ++i)
			delete (*i);
	}
	/// Adds a sub-state to this multi-state.
	void add(State *state)
	{
		_states.push_back(state);
	}
	/// Initializes sub-states.
	void init()
	{
		for (std::vector<State*>::iterator i = _states.begin(); i != _states.end(); ++i)
			(*i)->init();
	}
	/// Handles any sub-state events.
	void handle(Action *action)
	{
		for (std::vector<State*>::iterator i = _states.begin(); i != _states.end(); ++i)
			(*i)->handle(action);
	}
	/// Runs sub-state functionality every cycle.
	void think()
	{
		for (std::vector<State*>::iterator i = _states.begin(); i != _states.end(); ++i)
			(*i)->think();
	}
	/// Blits the sub-states to the screen.
	void blit()
	{
		for (std::vector<State*>::iterator i = _states.begin(); i != _states.end(); ++i)
			(*i)->blit();
	}
};

}

#endif