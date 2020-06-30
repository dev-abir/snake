#include "Snake.h"

Snake::Snake() : died(false) {
	board = std::make_unique<Board_type>();
	// TODO: check for insufficient memory, when creating the above array...

	/*ensure each and every element of board is false*/
	for (auto& row : *board) {
		row.fill(false);
	}
	/**/

	for (unsigned int i = 0; i < 5; ++i) {
		snake_dq.emplace_back(
			sf::RectangleShape(sf::Vector2f(SNAKE_CELL_WIDTH - SNAKE_CELL_BORDER_WIDTH, SNAKE_CELL_HEIGHT - SNAKE_CELL_BORDER_WIDTH)),
			direction::LEFT
		);
		snake_dq[i].body.setPosition(SCREEN_WIDTH / 2 + (SNAKE_CELL_WIDTH * i) + SNAKE_CELL_BORDER_WIDTH, SCREEN_HEIGHT / 2 + SNAKE_CELL_BORDER_WIDTH);
		setBoardElement(snake_dq[i].body.getPosition(), true);
		// std::cout << SCREEN_WIDTH / 2 + (SNAKE_CELL_WIDTH * i) + SNAKE_CELL_BORDER_WIDTH << "\t" << SCREEN_HEIGHT / 2 + SNAKE_CELL_BORDER_WIDTH << "\n";
		snake_dq[i].body.setFillColor(sf::Color::Green);
	}
	snake_dq[0].body.setFillColor(sf::Color::Magenta); // set the head's color different than the body color....
}

const sf::RectangleShape & Snake::getHeadBounds() const {
	return snake_dq.begin()->body;
}

void Snake::setNewFoodPosition(sf::RectangleShape & food) const {
	// This class may not be a good place to generate new food position... but for now......

	// This is not a good way to generate food position, especially
	// when the snake is very large... I may change it in the future...
	bool intersect = false;
	while (true) {
		food.setPosition((int)(rand() % SCREEN_WIDTH), (int)(rand() % SCREEN_HEIGHT));
		for (const Cell & cell : snake_dq) {
			if (cell.body.getGlobalBounds().intersects(food.getGlobalBounds())) {
				intersect = true;
				break;
			}
		}
		if (!intersect) break;
	}
}

bool Snake::isDied() const { return died; }

void Snake::grow() {
	Cell newTail = *(--snake_dq.end()); // making a copy of the last-but-one cell
										// TODO(RESEARCH): for some reason the snake_dq.pop_back();
										// in Snake::addCellAtFront(...) doesn't delete the
										// last cell till this point (the last cell will be deleted in the future)
										// so I am copying the last-but-one cell... BTW, snake_dq.end()--, will not work...
	direction tailDirection = newTail.bodyDirection;
	sf::Vector2f newTailPosition = newTail.body.getPosition();
	if (tailDirection == direction::UP) newTailPosition.y += SNAKE_CELL_HEIGHT;
	else if (tailDirection == direction::DOWN) newTailPosition.y -= SNAKE_CELL_HEIGHT;
	else if (tailDirection == direction::LEFT) newTailPosition.x += SNAKE_CELL_WIDTH;
	else if (tailDirection == direction::RIGHT) newTailPosition.x -= SNAKE_CELL_WIDTH;
	wrapIfNecessary(newTailPosition);
	newTail.body.setPosition(newTailPosition); // changing it's position

	// std::cout << newTailPosition.x << "\t" << newTailPosition.y << "\n";
	if (getBoardElement(newTailPosition)) { /*TODO: BUG... died = true; std::cout << "log0\n";*/ } // if there is a cell at the new tail, then the snake hits itself, then die...

	// even if the snake dies(or maybe not) let the player actually see that the snake hits itself.... so let's continue....
	setBoardElement(newTailPosition, true); // make the new cell alive in the board
	snake_dq.push_back(newTail); // push it to the snake_dq
}

void Snake::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (const Cell& cell : snake_dq) {
		target.draw(cell.body, states);
	}
}

void Snake::moveUp() {
	if (snake_dq.begin()->bodyDirection != direction::DOWN) {
		sf::RectangleShape oldHeadBody = snake_dq.begin()->body;
		addCellAtFront(
				sf::Vector2f(oldHeadBody.getPosition().x, oldHeadBody.getPosition().y - SNAKE_CELL_HEIGHT),
				direction::UP
		);
	}
}

void Snake::moveDown() {
	if (snake_dq.begin()->bodyDirection != direction::UP) {
		sf::RectangleShape oldHeadBody = snake_dq.begin()->body;
		addCellAtFront(
				sf::Vector2f(oldHeadBody.getPosition().x, oldHeadBody.getPosition().y + SNAKE_CELL_HEIGHT),
				direction::DOWN
		);
	}
}

void Snake::moveLeft() {
	if (snake_dq.begin()->bodyDirection != direction::RIGHT) {
		sf::RectangleShape oldHeadBody = snake_dq.begin()->body;
		addCellAtFront(
				sf::Vector2f(oldHeadBody.getPosition().x - SNAKE_CELL_WIDTH, oldHeadBody.getPosition().y),
				direction::LEFT
		);
	}
}

void Snake::moveRight() {
	if (snake_dq.begin()->bodyDirection != direction::LEFT) {
		sf::RectangleShape oldHeadBody = snake_dq.begin()->body;
		addCellAtFront(
				sf::Vector2f(oldHeadBody.getPosition().x + SNAKE_CELL_WIDTH, oldHeadBody.getPosition().y),
				direction::RIGHT
		);
	}
}

void Snake::addCellAtFront(sf::Vector2f bodyPosition, const direction bodyDirection) {
	snake_dq.begin()->body.setFillColor(sf::Color::Green); // set previous head color to Green
	Cell newHead = *snake_dq.begin(); // copy the previous head
	wrapIfNecessary(bodyPosition);
	newHead.body.setPosition(bodyPosition); // change it's position,
	newHead.bodyDirection = bodyDirection; // direction
	newHead.body.setFillColor(sf::Color::Magenta); // it's color(head's color is different from body color)

	if (getBoardElement(bodyPosition)) { died = true; /*std::cout << "log1\n";*/ } // if there is a cell at the new head, then the snake hits itself, then die...

	// even if the snake dies(or maybe not) let the player actually see that the snake hits itself.... so let's continue....
	setBoardElement(bodyPosition, true); // make the new cell alive in the board
	snake_dq.push_front(newHead); // push it to the snake_dq

	Cell lastCell = *snake_dq.end();
	setBoardElement(lastCell.body.getPosition(), false); // make the last cell dead in the board
	// std::cout << lastCell.body.getPosition().x << "\t" << lastCell.body.getPosition().y << "\n";
	snake_dq.pop_back(); // delete the last cell
}

void Snake::wrapIfNecessary(sf::Vector2f& headPosition) {
	if (headPosition.x >= SCREEN_WIDTH) headPosition.x = 0 + SNAKE_CELL_BORDER_WIDTH;
	else if (headPosition.x <= 0) headPosition.x = SCREEN_WIDTH - SNAKE_CELL_WIDTH + SNAKE_CELL_BORDER_WIDTH;

	if (headPosition.y >= SCREEN_HEIGHT) headPosition.y = 0 + SNAKE_CELL_BORDER_WIDTH;
	else if (headPosition.y <= 0) headPosition.y = SCREEN_HEIGHT - SNAKE_CELL_HEIGHT + SNAKE_CELL_BORDER_WIDTH;
}

bool Snake::getBoardElement(const sf::Vector2f& renderPosition) const {
	// WARNING: No bounds check(for performance), I can confirm that there is very less chance of hapenning that for this codebase....
	return (*board)[(renderPosition.x - SNAKE_CELL_BORDER_WIDTH) / SNAKE_CELL_WIDTH][(renderPosition.y - SNAKE_CELL_BORDER_WIDTH) / SNAKE_CELL_HEIGHT];
}

void Snake::setBoardElement(const sf::Vector2f& renderPosition, const bool value) {
	// WARNING: No bounds check(for performance), I can confirm that there is very less chance of hapenning that for this codebase....
	(*board)[(renderPosition.x - SNAKE_CELL_BORDER_WIDTH) / SNAKE_CELL_WIDTH][(renderPosition.y - SNAKE_CELL_BORDER_WIDTH) / SNAKE_CELL_HEIGHT] = value;
}
