#include "Server.h"

void main()
{
	Server server(1111);

	// Wait for both players
	for (int itr = 0; itr < 2; itr++)
	{
		server.ListenForNewConnection();
	}

	sf::RenderWindow window(sf::VideoMode(800, 600), "window");

	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Clock clock;

	clock.restart();

	server.playerPos[0].x = 100;
	server.playerPos[0].y = 100;

	server.playerPos[1].x = 200;
	server.playerPos[1].y = 200;

	server.playerPos[2].x = 300;
	server.playerPos[2].y = 300;

	sf::CircleShape host(50);
	host.setFillColor(sf::Color::White);
	host.setOutlineColor(sf::Color::Red);
	host.setOutlineThickness(8);
	host.setPosition(100, 100);

	sf::CircleShape playerOneShape(50);
	playerOneShape.setFillColor(sf::Color::White);
	playerOneShape.setOutlineColor(sf::Color::Green);
	playerOneShape.setOutlineThickness(8);
	playerOneShape.setPosition(200, 200);

	sf::CircleShape playerTwoShape(50);
	playerTwoShape.setFillColor(sf::Color::White);
	playerTwoShape.setOutlineColor(sf::Color::Green);
	playerTwoShape.setOutlineThickness(8);
	playerTwoShape.setPosition(300, 300);

	server.currentChaser = (rand() % 3);
	server.sendChaser();

	if (server.currentChaser == 0)
	{
		host.setFillColor(sf::Color::Red);
		playerOneShape.setFillColor(sf::Color::White);
		playerTwoShape.setFillColor(sf::Color::White);
	}
	if (server.currentChaser == 1)
	{
		host.setFillColor(sf::Color::White);
		playerOneShape.setFillColor(sf::Color::Red);
		playerTwoShape.setFillColor(sf::Color::White);
	}
	if (server.currentChaser == 2)
	{
		host.setFillColor(sf::Color::White);
		playerOneShape.setFillColor(sf::Color::White);
		playerTwoShape.setFillColor(sf::Color::Red);
	}

	server.sendNewPos();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		timeSinceLastUpdate += clock.restart();

		server.timer += 1;

		if (timeSinceLastUpdate > timePerFrame)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && server.playerHit != 0)
			{
				server.playerPos[0].y -= server.speed;
				server.sendNewPos();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && server.playerHit != 0)
			{
				server.playerPos[0].y += server.speed;
				server.sendNewPos();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && server.playerHit != 0)
			{
				server.playerPos[0].x -= server.speed;
				server.sendNewPos();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && server.playerHit != 0)
			{
				server.playerPos[0].x += server.speed;
				server.sendNewPos();
			}

			// Every 60 frames, update all seconds by 1
			if (server.timer > 60)
			{
				server.hostAliveTime++;
				server.playerOneAliveTime++;
				server.playerTwoAliveTime++;
				server.timer = 0;
			}

			if (server.hitDelay > 0)//the cooldown between players getting caught
			{
				server.hitDelay--;
			}
			if (server.hitDelay == 1)
			{
				server.playerHit = -1;
				server.timeAlice.setString("");
			}

			window.clear();

			window.draw(host);
			window.draw(playerOneShape);
			window.draw(playerTwoShape);
			window.draw(server.timeAlice);

			window.display();

			timeSinceLastUpdate = sf::Time::Zero;
		}
	}

	system("pause");
}