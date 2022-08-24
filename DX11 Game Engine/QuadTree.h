#pragma once

#include <array>
#include <list>
#include <vector>
#include <memory>

#include "Vec2f.h"
#include "Point2f.h"

struct Rect {
	Vec2f position;
	Vec2f size;

	Rect() : position{ 0, 0 }, size{ 1, 1 } {
	}

	bool contains(Point2f point) {
		return (position.x >= point.x && position.x <= point.x + size.x) &&
			(position.y >= point.y && position.y <= point.y + size.y);
	}

	bool contains(Rect rect) {
		return (position.x >= rect.position.x && position.x + size.x <= rect.position.x + rect.size.x) &&
			(position.y >= rect.position.y && position.y + size.y <= rect.position.y + rect.size.y);
	}

	bool overlaps(Rect rect) {
		return ((position.x >= rect.position.x && position.x <= rect.position.x + rect.size.x) &&
			(position.y >= rect.position.y && position.y <= rect.position.y + rect.size.y)) ||
			((position.x + size.x <= rect.position.x + rect.size.x) &&
				(position.y + size.y <= rect.position.y + rect.size.y));
	}
};

template<typename T>
class QuadTree {
public:
	QuadTree(const Rect& size = { {0, 0}, {1, 1} }, const size_t depth = 0);
	~QuadTree();

	void resize(const Rect& size);
	void clear();

	size_t size() const;
	const Rect& area();

	void insert(const T& item, const Rect& size);
	std::list<T> search(const Rect& area) const;
	std::list<T> search(const Rect& area, std::list<T>& listItems) const;
	void items(std::list<T>& listItems) const;

private:
	// Maximum tree depth
	size_t depth = 0;
	const size_t MAX_DEPTH = 4;

	// Area of the tree
	Rect quadArea;

	// 4 children of tree
	std::array<Rect, 4> child{};

	// 4 potential children of tree
	std::array<std::shared_ptr<QuadTree<T>>, 4> potentialChild{};

	// Items contained by tree
	std::vector<std::pair<Rect, T>> listItems;
};