#include "StaticQuadTree.h"

template<typename T>
StaticQuadTree<T>::StaticQuadTree() {

}

template<typename T>
StaticQuadTree<T>::StaticQuadTree(const QTRect& area, const size_t depth) : m_area(area), m_depth(depth) {
	resize(area);
}

template<typename T>
StaticQuadTree<T>::~StaticQuadTree() {

}

template<typename T>
void StaticQuadTree<T>::resize(const QTRect& area) {
	clear();
	m_area = area;

	Point2f childSize = { m_area.size.x / 2.0f, m_area.size.y / 2.0f };
	m_child = {
		QTRect(m_area.position, childSize),
		QTRect({ m_area.position.x + childSize.x, m_area.position.y }, childSize),
		QTRect({ m_area.position.x, m_area.position.y + childSize.y }, childSize),
		QTRect(m_area.position + childSize, childSize),
	};
}

template<typename T>
void StaticQuadTree<T>::clear() {
	m_items.clear();
	for (int i = 0; i < 4; i++) {
		if (m_pChild[i]) {
			m_pChild[i]->clear();
		}
		m_pChild[i].reset();
	}
}

template<typename T>
size_t StaticQuadTree<T>::size() const {
	size_t count = m_items.size();
	for (int i = 0; i < 4; i++) {
		if (m_pChild[i]) {
			count += m_pChild[i]->size();
		}
	}
	return count;
}

template<typename T>
const QTRect& StaticQuadTree<T>::area() {
	return m_area;
}

template<typename T>
void StaticQuadTree<T>::insert(const T& item, const QTRect& area) {
	for (int i = 0; i < 4; i++) {
		if (m_child[i].contains(area)) {
			if (m_depth + 1 < MAX_DEPTH) {
				if (!m_pChild[i]) {
					m_pChild[i] = std::make_shared<StaticQuadTree<T>>(m_child[i], m_depth + 1);
				}

				m_pChild[i]->insert(item, area);
				return;
			}
		}
	}

	m_items.push_back({ area, item });
}

template<typename T>
std::list<T> StaticQuadTree<T>::search(const QTRect& area) const {
	std::list<T> items;
	search(area, items);
	return items;
}

template<typename T>
void StaticQuadTree<T>::search(const QTRect& area, std::list<T>& listItems) const {
	for (const auto& i : m_items) {
		if (area.overlaps(i.first)) {
			listItems.push_back(i.second);
		}
	}

	for (int i = 0; i < 4; i++) {
		if (m_pChild[i]) {
			if (area.contains(m_child[i])) {
				m_pChild[i]->items(listItems);
			} else if (m_child[i].overlaps(area)) {
				m_pChild[i]->search(area, listItems);
			}
		}
	}
}

template<typename T>
void StaticQuadTree<T>::items(std::list<T>& listItems) const {
	for (const auto& i : m_items) {
		listItems.push_back(i.second);
	}

	for (int i = 0; i < 4; i++) {
		if (m_pChild[i]) {
			m_pChild[i]->items(listItems);
		}
	}
}
