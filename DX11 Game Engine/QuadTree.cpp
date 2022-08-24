#include "QuadTree.h"

template<typename T>
QuadTree<T>::QuadTree(const Rect& size, const size_t depth) : quadArea(size), depth(depth) {

}

template<typename T>
QuadTree<T>::~QuadTree() {

}

template<typename T>
void QuadTree<T>::resize(const Rect& size) {
	clear();
	quadArea = size;

	Vec2f childSize = { quadArea.size / 2.0f, quadArea.size / 2.0f };
	child = {
		Rect(quadArea.position, childSize),
		Rect({ quadArea.position.x + childSize.x, quadArea.position.y }, childSize),
		Rect({ quadArea.position.x, quadArea.position.y + childSize.y }, childSize),
		Rect(quadArea.position + childSize, childSize),
	};
}

template<typename T>
void QuadTree<T>::clear() {
	items.clear();
	for (int i = 0; i < 4; i++) {
		if (potentialChild[i]) {
			potentialChild[i]->clear();
		}
		potentialChild[i].reset();
	}
}

template<typename T>
size_t QuadTree<T>::size() const {
	size_t count = listItems.size();
	for (int i = 0; i < 4; i++) {
		if (potentialChild[i]) {
			count += potentialChild[i]->size();
		}
	}
	return count;
}

template<typename T>
const Rect& QuadTree<T>::area() {
	return quadArea;
}

template<typename T>
void QuadTree<T>::insert(const T& item, const Rect& size) {
	for (int i = 0; i < 4; i++) {
		if (child[i].contains(size)) {
			if (depth + 1 < MAX_DEPTH) {
				if (!potentialChild[i]) {
					potentialChild[i] = std::make_shared<QuadTree<T>>(child[i], depth + 1);
				}

				potentialChild[i]->insert(item, size);
				return;
			}
		}
	}

	listItems.push_back({size, item});
}

template<typename T>
std::list<T> QuadTree<T>::search(const Rect& area) const {
	std::list<T> items;
	search(area, items);
	return items;
}

template<typename T>
std::list<T> QuadTree<T>::search(const Rect& area, std::list<T>& listItems) const {
	for (const auto& i : listItems) {
		if (area.overlaps(i.first)) {
			listItems.push_back(i.second);
		}
	}

	for (int i = 0; i < 4; i++) {
		if (potentialChild[i]) {
			if (area.contains(child[i])) {
				potentialChild[i]->items(listItems);
			} else if (child[i].overlaps(area)) {
				potentialChild[i]->search(area, listItems);
			}
		}
	}
}

template<typename T>
void QuadTree<T>::items(std::list<T>& listItems) const {
	for (const auto& i : listItems) {
		listItems.push_back(i.second);
	}

	for (int i = 0; i < 4; i++) {
		if (potentialChild[i]) {
			potentialChild[i]->items(listItems);
		}
	}
}
