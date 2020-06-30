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
	bool isDied() const;
	void grow();

private:
	bool died;

	typedef std::array<std::array<bool, SCREEN_HEIGHT/SNAKE_CELL_HEIGHT>, SCREEN_WIDTH/SNAKE_CELL_WIDTH> Board_type;
	std::unique_ptr<Board_type> board;	// a large array for the whole board,
										// it will be used in checking whether the snake has hit itself or not

	enum class direction {
		UP, DOWN, LEFT, RIGHT
	};

	struct Cell {
		sf::RectangleShape body;
		direction bodyDirection;
		Cell(sf::RectangleShape body, direction bodyDirection) : body(body), bodyDirection(bodyDirection) {}
	};

	std::deque<Cell> snake_dq; // TODO: allocating statically may cause out of memory problems

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void addCellAtFront(sf::Vector2f bodyPosition, const direction bodyDirection);
	void wrapIfNecessary(sf::Vector2f& headPosition);
	bool getBoardElement(const sf::Vector2f& renderPosition) const;
	void setBoardElement(const sf::Vector2f& renderPosition, const bool value);
};

#endif // SNAKE_H
