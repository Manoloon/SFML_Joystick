#include <iostream>
#include <SFML/Graphics.hpp>

const unsigned  int JOYSTICK_POSITION = 1;
const float DEAD_ZONE = 15.f;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800,600,32),"SFML Joystick",sf::Style::Default);
    sf::Event event;

    sf::RectangleShape square;
    square.setFillColor(sf::Color(51, 232, 137,255));
    square.setPosition(window.getSize().x/2,window.getSize().y/2);
    square.setOrigin(square.getSize().x/2,square.getSize().y/2);
    square.setOutlineColor(sf::Color(3,63,81,255));
    square.setOutlineThickness(2.f);
    square.setSize(sf::Vector2f(50.f,50.f));

    // Get joystick info
    sf::Joystick::Identification joyId = sf::Joystick::getIdentification(JOYSTICK_POSITION);
    std::cout << "\nVendor ID: " << joyId.vendorId << "\nProductID: " << joyId.productId << std::endl;
    sf::String controller ("Joystick use : " + joyId.name);
    window.setTitle(controller);

    //query joystick for settings if its plugged in
    if(sf::Joystick::isConnected(JOYSTICK_POSITION))
    {
        unsigned int buttonCount = sf::Joystick::getButtonCount(JOYSTICK_POSITION);
        bool hasZAxis = sf::Joystick::hasAxis(JOYSTICK_POSITION,sf::Joystick::Z);
        std::cout << "Button count : " << buttonCount << std::endl;
        std::cout << "Has Z Axis?: " << hasZAxis << std::endl;
    }

    bool move = false;
    int turbo = 1;

    // for movement
    sf::Vector2f speed = sf::Vector2f(0.f,0.f);

    // time
    sf::Clock tickClock;
    sf::Time timeSinceLastTick = sf::Time::Zero;
    // FPS = 60
    sf::Time timePerFrame = sf::seconds(1.f/60.f);
    sf::Time tickDuration = sf::Time::Zero;

    bool bGameRunning = true;
    while (bGameRunning)
    {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Escape: {
                        window.close();
                        return 0;
                    }
                        break;
                    default:
                        break;
                }
            }
            if (event.type == sf::Event::JoystickMoved) {
                move = true;
                std::cout << "X Axis: " << speed.x << std::endl;
                std::cout << "Y Axis: " << speed.y << std::endl;
            } else {
                move = false;
            }
            // A button on XBOX 360
            if (sf::Joystick::isButtonPressed(JOYSTICK_POSITION, 0)) {
                turbo = 2;
            }
            if (!sf::Joystick::isButtonPressed(JOYSTICK_POSITION, 0)) {
                turbo = 1;
            }
            // B Button on XBOX 360
            if (sf::Joystick::isButtonPressed(JOYSTICK_POSITION, 1)) {
                window.close();
                return 0;
            }
        }
        // check state of joystick
        speed = sf::Vector2f(sf::Joystick::getAxisPosition(JOYSTICK_POSITION,sf::Joystick::X),sf::Joystick::getAxisPosition(JOYSTICK_POSITION,sf::Joystick::Y));
        timeSinceLastTick += tickClock.restart();
        while (timeSinceLastTick > timePerFrame)
        {
           //update the position of the square according to input from joystick
           //CHECK DEAD ZONES - OTHERWISE INPUT WILL RESULT IN JITTERY MOVEMENTS WHEN NO INPUT IS PROVIDED
           //INPUT RANGES FROM -100 TO 100
           //A 15% DEAD ZONE SEEMS TO WORK FOR ME - GIVE THAT A SHOT
           // DEAD_ZONE = 15
           if(speed.x >DEAD_ZONE || speed.x < -DEAD_ZONE ||speed.y > DEAD_ZONE ||speed.y < -DEAD_ZONE)
           {
              square.move(turbo*speed.x*timePerFrame.asSeconds(),turbo*speed.y*timePerFrame.asSeconds());
              square.setRotation(turbo*speed.x*timePerFrame.asSeconds());
           }
            timeSinceLastTick -=timePerFrame;
           std::cout << "Time since : " << std::to_string(timeSinceLastTick.asMilliseconds()) <<
           std::endl;
        }
        window.clear(sf::Color(128,32,19));
        window.draw(square);
        window.display();
    }
    return 0;
}
