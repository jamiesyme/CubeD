#pragma once

class Player;
class SystemEvent;

class UserPlayerController {
public:
	UserPlayerController(Player& player);

	void onEvent(const SystemEvent& systemEvent);

	void tick(float deltaTime);

	Player& player;
	float lookSpeedX;
	float lookSpeedY;
	float moveSpeedX;
	float moveSpeedY;
	float moveSpeedZ;

private:
	int previousMouseX;
	int previousMouseY;
};
