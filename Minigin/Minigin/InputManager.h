#pragma once
#include <XInput.h>
#include "Singleton.h"
#include "Command.h"

enum class ControllerButton
	{
		ButtonUp,
		ButtonDown,
		ButtonLeft,
		ButtonRight,
		ButtonA
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;
		std::shared_ptr<Command> HandleInput();
	private:
		XINPUT_STATE m_CurrentState{};

		// Commands
		std::shared_ptr<WalkLeftCommand> m_pButtonLeft = std::make_shared<WalkLeftCommand>();
		std::shared_ptr<WalkRightCommand> m_pButtonRight = std::make_shared<WalkRightCommand>();
		std::shared_ptr<WalkUpCommand> m_pButtonUp = std::make_shared<WalkUpCommand>();
		std::shared_ptr<WalkDownCommand> m_pButtonDown = std::make_shared<WalkDownCommand>();
		
		std::shared_ptr<ShootCommand> m_pButtonA = std::make_shared<ShootCommand>();
	};

