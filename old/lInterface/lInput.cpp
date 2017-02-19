
#include "lInput.h"

void lInput::SetMX(int x)
{
	MouseState.X = x;
}

void lInput::SetMY(int y)
{
	MouseState.Y = y;
}

void lInput::SetLmb(bool lmb)
{
	MouseState.LmbPressed = lmb;
}

void lInput::SetMmb(bool mmb)
{
	MouseState.MmbPressed = mmb;
}

void lInput::SetRmb(bool rmb)
{
	MouseState.RmbPressed = rmb;
}

int lInput::GetX() const
{
	return MouseState.X;
}

int lInput::GetY() const
{
	return MouseState.Y;
}

bool lInput::GetLmb() const
{
	return MouseState.LmbPressed;
}

bool lInput::GetMmb() const
{
	return MouseState.MmbPressed;
}

bool lInput::GetRmb() const
{
	return MouseState.RmbPressed;
}

lInput::lMouseState &lInput::GetMouseState()
{
	return MouseState;
}

const lInput::lMouseState &lInput::GetMouseState() const
{
	return MouseState;
}

lInput::lKeyboardState &lInput::GetKeyboardState()
{
	return KeyboardState;
}

const lInput::lKeyboardState &lInput::GetKeyboardState() const
{
	return KeyboardState;
}

void lInput::SetEscape(bool Val)
{
    KeyboardState.Escape = Val;
}

void lInput::SetEnter(bool Val)
{
    KeyboardState.Enter = Val;
}

void lInput::SetLShift(bool Val)
{
    KeyboardState.LShift = Val;
}

void lInput::SetRShift(bool Val)
{
    KeyboardState.RShift = Val;
}

void lInput::SetSpace(bool Val)
{
    KeyboardState.Space = Val;
}

void lInput::SetUp(bool Val)
{
    KeyboardState.Up = Val;
}

void lInput::SetDown(bool Val)
{
    KeyboardState.Down = Val;
}

void lInput::SetLeft(bool Val)
{
    KeyboardState.Left = Val;
}

void lInput::SetRight(bool Val)
{
    KeyboardState.Right = Val;
}

bool lInput::GetEscape() const
{
    return KeyboardState.Escape;
}

bool lInput::GetEnter() const
{
    return KeyboardState.Enter;
}

bool lInput::GetLShift() const
{
    return KeyboardState.LShift;
}

bool lInput::GetRShift() const
{
    return KeyboardState.RShift;
}

bool lInput::GetSpace() const
{
    return KeyboardState.Space;
}

bool lInput::GetUp() const
{
    return KeyboardState.Up;
}

bool lInput::GetDown() const
{
    return KeyboardState.Down;
}

bool lInput::GetLeft() const
{
    return KeyboardState.Left;
}

bool lInput::GetRight() const
{
    return KeyboardState.Right;
}
