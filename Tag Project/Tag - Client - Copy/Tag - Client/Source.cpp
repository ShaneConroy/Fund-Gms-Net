#include "Client.h"
#include <conio.h>

int main()
{
    Client client("127.0.0.1", 1111);

    std::cout << "Loading host server..." << "\n";

    if (!client.Connect())
    {
        std::cout << "Failed to connect to server" << std::endl;
        system("pause");
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "Client window");

    sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Clock clock;

    sf::Text clientHeader;
    clientHeader.setFont(client.font);
    clientHeader.setCharacterSize(25);
    clientHeader.setString("Client");
    clientHeader.setPosition(50, 50);

    clock.restart();

    client.playerPos[0].x = 100;
    client.playerPos[0].y = 100;

    client.playerPos[1].x = 200;
    client.playerPos[1].y = 200;

    client.playerPos[2].x = 300;
    client.playerPos[2].y = 300;

    sf::CircleShape host(50);
    host.setFillColor(sf::Color::Red);
    host.setPosition(100, 100);

    sf::CircleShape playerOne(50);
    playerOne.setFillColor(sf::Color::Green);
    playerOne.setPosition(200, 200);

    sf::CircleShape playerTwo(50);
    playerTwo.setFillColor(sf::Color::Green);
    playerTwo.setPosition(300, 300);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        timeSinceLastUpdate += clock.restart();

        if (timeSinceLastUpdate > timePerFrame)
        {
            //if (_kbhit()) // Check if a key has been pressed
            //{
            //    char key = _getch();
            //    if (key == 'i' || key == 'j' || key == 'k' || key == 'l')
            //    {
            //        std::string userinput(1, key);
            //        if (!client.SendString(userinput))
            //        {
            //            break;
            //        }
            //        std::cout << "Sent: " << key << std::endl; // Debugging
            //    }
            //}

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                std::string userinput = "w";
                client.SendString(userinput);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                std::string keyPressed = "s";
                client.SendString(keyPressed);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                std::string keyPressed = "a";
                client.SendString(keyPressed);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                std::string keyPressed = "d";
                client.SendString(keyPressed);
            }

            if (client.timeAliveDisplay > 0)
            {
                client.timeAliveDisplay--;
            }
            if (client.timeAliveDisplay == 1)
            {
                client.timeAlive.setString("");
            }

            window.clear();

            window.draw(client.host);
            window.draw(client.playerOneShape);
            window.draw(client.playerTwoShape);

            window.draw(clientHeader);
            window.draw(client.timeAlive);

            window.display();

            timeSinceLastUpdate = sf::Time::Zero;
        }
    }

    system("pause");
    return 0;
}
