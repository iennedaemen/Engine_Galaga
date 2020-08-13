#pragma once

enum State
{
	Idle,
	walking,
	Shooting,
	Dying
};

enum Input
{
	Left,
	Right,
	Up,
	Down,
	Space
};

enum class ControllerButton
{
	ButtonUp,
	ButtonDown,
	ButtonLeft,
	ButtonRight,
	ButtonA
};