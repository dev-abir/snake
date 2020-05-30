#include "Snake.h"

Snake::Snake() {
	for (unsigned int i = 0; i < 5; ++i) {
		snake_dq.emplace_back(
			sf::RectangleShape(sf::Vector2f(SNAKE_CELL_WIDTH - SNAKE_CELL_BORDER_WIDTH, SNAKE_CELL_HEIGHT - SNAKE_CELL_BORDER_WIDTH)),
			direction::LEFT
		);
		snake_dq[i].body.setPosition(SCREEN_WIDTH / 2 + (SNAKE_CELL_WIDTH * i) + SNAKE_CELL_BORDER_WIDTH, SCREEN_HEIGHT / 2 + SNAKE_CELL_BORDER_WIDTH);
		snake_dq[i].body.setFillColor(sf::Color::Green);
	}
}

const sf::RectangleShape & Snake::getHeadBounds() const {
	return snake_dq.begin()->body;
}

void Snake::setNewFoodPosition(sf::RectangleShape & food) const {
	// This is not a good way to generate food position, especially
	// when snake is very large... I may change it in the future...
	bool notIntersect = true;
	while (true) {
		food.setPosition((int)(rand() % SCREEN_WIDTH), (int)(rand() % SCREEN_HEIGHT));
		for (auto & cell : snake_dq) {
			if (cell.body.getGlobalBounds().intersects(food.getGlobalBounds())) {
				notIntersect = false;
				break;
			}
		}
		if (notIntersect) break;
	}
}

void Snake::grow() {
	auto newTail = *snake_dq.end();
	auto tailDirection = newTail.bodyDirection;
	auto newTailPosition = newTail.body.getPosition();
	if (tailDirection == direction::UP) newTailPosition.y += SNAKE_CELL_HEIGHT;
	else if (tailDirection == direction::DOWN) newTailPosition.y -= SNAKE_CELL_HEIGHT;
	else if (tailDirection == direction::LEFT) newTailPosition.x += SNAKE_CELL_WIDTH;
	else if (tailDirection == direction::RIGHT) newTailPosition.x -= SNAKE_CELL_WIDTH;
	wrapIfNecessary(newTailPosition);
	newTail.body.setPosition(newTailPosition);
	snake_dq.push_back(newTail); // emplace_front not needed...
}

void Snake::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (auto & cell : snake_dq) {
		target.draw(cell.body, states);
	}
}

void Snake::moveUp() {
	if (snake_dq.begin()->bodyDirection != direction::DOWN) {
		auto oldHeadBody = snake_dq.begin()->body;
		addCellAtFront(
				sf::Vector2f(oldHeadBody.getPosition().x, oldHeadBody.getPosition().y - SNAKE_CELL_HEIGHT),
				direction::UP
		);
	}
}

void Snake::moveDown() {
	if (snake_dq.begin()->bodyDirection != direction::UP) {
		auto oldHeadBody = snake_dq.begin()->body;
		addCellAtFront(
				sf::Vector2f(oldHeadBody.getPosition().x, oldHeadBody.getPosition().y + SNAKE_CELL_HEIGHT),
				direction::DOWN
		);
	}
}

void Snake::moveLeft() {
	if (snake_dq.begin()->bodyDirection != direction::RIGHT) {
		auto oldHeadBody = snake_dq.begin()->body;
		addCellAtFront(
				sf::Vector2f(oldHeadBody.getPosition().x - SNAKE_CELL_WIDTH, oldHeadBody.getPosition().y),
				direction::LEFT
		);
	}
}

void Snake::moveRight() {
	if (snake_dq.begin()->bodyDirection != direction::LEFT) {
		auto oldHeadBody = snake_dq.begin()->body;
		addCellAtFront(
				sf::Vector2f(oldHeadBody.getPosition().x + SNAKE_CELL_WIDTH, oldHeadBody.getPosition().y),
				direction::RIGHT
		);
	}
}

void Snake::addCellAtFront(sf::Vector2f && bodyPosition, const direction bodyDirection) {
	auto newHead = *snake_dq.begin();
	wrapIfNecessary(bodyPosition);
	newHead.body.setPosition(bodyPosition);
	newHead.bodyDirection = bodyDirection;
	snake_dq.push_front(newHead); // emplace_front not needed...
	snake_dq.pop_back();
}

void Snake::wrapIfNecessary(sf::Vector2f & headPosition) {
	if (headPosition.x > SCREEN_WIDTH) headPosition.x = 0;
	else if (headPosition.x < 0) headPosition.x = SCREEN_WIDTH - SNAKE_CELL_WIDTH;

	if (headPosition.y > SCREEN_HEIGHT) headPosition.y = 0;
	else if (headPosition.y < 0) headPosition.y = SCREEN_HEIGHT - SNAKE_CELL_HEIGHT;
}
