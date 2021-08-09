#pragma once
#include <XInput.h>
#include "Singleton.h"
#include "Command.h"
#include "Controller.h"
#include "structs.h"

class Player;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		bool IsPressed(ControllerButton button, int playerNr) const;
		std::shared_ptr<Command> HandleInput(int playerNr, Player& player);
	private:
		XINPUT_STATE m_CurrentState1{};
		XINPUT_STATE m_CurrentState2{};

		// Commands
		std::shared_ptr<MoveLeftCommand> m_pButtonLeft = std::make_shared<MoveLeftCommand>();
		std::shared_ptr<MoveRightCommand> m_pButtonRight = std::make_shared<MoveRightCommand>();
		
		std::shared_ptr<ShootCommand> m_pButtonA = std::make_shared<ShootCommand>();

		// Controllers
		std::shared_ptr<Controller> m_Contoller1 = std::make_shared<Controller>(1);
		std::shared_ptr<Controller> m_Contoller2 = std::make_shared<Controller>(2);
	};

