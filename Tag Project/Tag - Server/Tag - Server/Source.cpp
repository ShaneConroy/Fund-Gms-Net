#include "Server.h"

void main()
{
	Server server(1111);

	std::cout << "Waiting for players" << "\n";
	// Wait for both players
	for (int itr = 0; itr < 2; itr++)
	{
		server.ListenForNewConnection();
	}

	sf::RenderWindow window(sf::VideoMode(800, 600), "window");

	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero; 
	sf::Clock clock;

	sf::Text header;

	header.setFont(server.font);
	header.setCharacterSize(25);
	header.setString("HOST");
	header.setPosition(50, 50);

	clock.restart();

	server.playerPos[0].x = 100;
	server.playerPos[0].y = 100;

	server.playerPos[1].x = 200;
	server.playerPos[1].y = 200;

	server.playerPos[2].x = 300;
	server.playerPos[2].y = 300;

	server.host.setRadius(50);
	server.host.setFillColor(sf::Color::White);
	server.host.setPosition(100, 100);

	server.playerOneShape.setRadius(50);
	server.playerOneShape.setFillColor(sf::Color::White);
	server.playerOneShape.setPosition(200, 200);

	server.playerTwoShape.setRadius(50);
	server.playerTwoShape.setFillColor(sf::Color::White);
	server.playerTwoShape.setPosition(300, 300);

	server.currentChaser = (rand() % 3);
	server.sendChaser();

	if (server.currentChaser == 0)
	{
		server.host.setFillColor(sf::Color::Red);
		server.playerOneShape.setFillColor(sf::Color::Green);
		server.playerTwoShape.setFillColor(sf::Color::Green);
	}
	if (server.currentChaser == 1)
	{
		server.host.setFillColor(sf::Color::Green);
		server.playerOneShape.setFillColor(sf::Color::Red);
		server.playerTwoShape.setFillColor(sf::Color::Green);
	}
	if (server.currentChaser == 2)
	{
		server.host.setFillColor(sf::Color::Green);
		server.playerOneShape.setFillColor(sf::Color::Green);
		server.playerTwoShape.setFillColor(sf::Color::Red);
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				server.playerPos[0].y -= server.speed;
				server.sendNewPos();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				server.playerPos[0].y += server.speed;
				server.sendNewPos();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				server.playerPos[0].x -= server.speed;
				server.sendNewPos();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				server.playerPos[0].x += server.speed;
				server.sendNewPos();
			}
			if (server.playerPos[0].x > 800)
			{
				server.playerPos[0].x = -100;
			}
			else if (server.playerPos[0].x < -100)
			{
				server.playerPos[0].x = 800;
			}
			else if (server.playerPos[0].y > 600)
			{
				server.playerPos[0].y = -100;
			}
			else if (server.playerPos[0].y < -100)
			{
				server.playerPos[0].y = 600;
			}
			server.collisions();

			if (server.hitDelay > 0)
			{
				server.hitDelay--;
			}

			window.clear();

			window.draw(server.host);
			server.host.setPosition(server.playerPos[0]);
			window.draw(server.playerOneShape);
			server.playerOneShape.setPosition(server.playerPos[1]);
			window.draw(server.playerTwoShape);
			server.playerTwoShape.setPosition(server.playerPos[2]);
			window.draw(header);

			window.display();

			timeSinceLastUpdate = sf::Time::Zero;
		}
	}

	system("pause");
}