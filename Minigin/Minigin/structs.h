#pragma once

enum State
{
	Idle,
	Attacking,
	Dead
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

enum class EnemyType
{
	Zako,
	Goei,
	Boss
};

enum class Event
{
	ZakoHit
};