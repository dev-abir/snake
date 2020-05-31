#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "settings.h"
#include "Snake.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), TITLE);
	window.setFramerateLimit(FPS);

	Snake snake;
	sf::RectangleShape food(sf::Vector2f(FOOD_WIDTH, FOOD_HEIGHT)); // I may create food class later, but for now.....
	food.setFillColor(sf::Color::Red);
	snake.setNewFoodPosition(food);

	long score = 0;

	/////////////////////MAIN LOOP/////////////////////
	sf::Event event;
	while (window.isOpen()) {

		/////////////////////EVENT HANDLING/////////////////////
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					window.close();
					break;
				default:
					break;
				}
			}
		}

		/////////////////////REAL TIME EVENT HANDLING(FOR SMOOTHER MOVEMENT)/////////////////////
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) snake.moveUp();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) snake.moveDown();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) snake.moveLeft();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) snake.moveRight();

		if (snake.getHeadBounds().getGlobalBounds().intersects(food.getGlobalBounds())) {
			score += 1;
			std::cout << "score: " << score << "\n";
			snake.setNewFoodPosition(food);
			snake.grow();
		}

		window.clear();
		window.draw(food);
		window.draw(snake);
		window.display();
	}

	return 0;
}
