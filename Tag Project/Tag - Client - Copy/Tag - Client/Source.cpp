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

    client.host.setRadius(50);
    client.host.setFillColor(sf::Color::Red);
    client.host.setPosition(100, 100);

    client.playerOneShape.setRadius(50);
    client.playerOneShape.setFillColor(sf::Color::Green);
    client.playerOneShape.setPosition(200, 200);

    client.playerTwoShape.setRadius(50);
    client.playerTwoShape.setFillColor(sf::Color::Green);
    client.playerTwoShape.setPosition(300, 300);

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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                std::string userinput = "W";
                client.SendString(userinput);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                std::string userinput = "S";
                client.SendString(userinput);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                std::string userinput = "A";
                client.SendString(userinput);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                std::string userinput = "D";
                client.SendString(userinput);
            }
            window.clear();

            window.draw(client.host);
            //host.setPosition(client.playerPos[0]);
            window.draw(client.playerOneShape);
            //client.playerOneShape.setPosition(client.playerPos[1]);
            window.draw(client.playerTwoShape);
            //client.playerTwoShape.setPosition(client.playerPos[2]);

            window.draw(clientHeader);

            window.display();

            timeSinceLastUpdate = sf::Time::Zero;
        }
    }

    system("pause");
    return 0;
}
