#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "settings.h"

class Snake: public sf::Drawable {
public:
	Snake();
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	const sf::RectangleShape & getHeadBounds() const;
	void setNewFoodPosition(sf::RectangleShape & food) const;
	void grow();

private:
	enum direction {
		UP, DOWN, LEFT, RIGHT
	};
	struct Cell {
		sf::RectangleShape body;
		direction bodyDirection;
		Cell(sf::RectangleShape body, direction bodyDirection) : body(body), bodyDirection(bodyDirection) {}

		Cell& operator= (const Cell &other) = default;
		Cell(const Cell &other) = default;
	};
	std::deque<Cell> snake_dq; // TODO: allocating statically may cause out of memory problems

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	void addCellAtFront(sf::Vector2f && bodyPosition, const direction bodyDirection);
	void wrapIfNecessary(sf::Vector2f & headPosition);
};

#endif // SNAKE_H
