#pragma once

#include <array>
#include <list>
#include <vector>
#include <memory>

#include "QTRect.h"

template<typename T>
class StaticQuadTree {
public:
	StaticQuadTree() {

	}

	StaticQuadTree(const QTRect& area, const size_t depth) : m_area(area), m_depth(depth) {
		resize(area);
	}

	~StaticQuadTree() {

	}


	void resize(const QTRect& area) {
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

	void clear() {
		m_items.clear();
		for (int i = 0; i < 4; i++) {
			if (m_pChild[i]) {
				m_pChild[i]->clear();
			}
			m_pChild[i].reset();
		}
	}

	size_t size() const {
		size_t count = m_items.size();
		for (int i = 0; i < 4; i++) {
			if (m_pChild[i]) {
				count += m_pChild[i]->size();
			}
		}
		return count;
	}

	const QTRect& area() {
		return m_area;
	}

	void insert(const T& item, const QTRect& area) {
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

	std::vector<T> search(const QTRect& area) const {
		std::vector<T> items;
		search(area, items);
		return items;
	}

	void search(const QTRect& area, std::vector<T>& listItems) const {
		for (const auto& i : m_items) {
			if (area.overlaps(i.first)) {
				listItems.push_back(i.second);
			}
		}

		for (int i = 0; i < 4; i++) {
			if (m_pChild[i]) {
				if (area.contains(m_child[i])) {
					m_pChild[i]->items(listItems);
				}
				else if (m_child[i].overlaps(area)) {
					m_pChild[i]->search(area, listItems);
				}
			}
		}
	}

	void items(std::vector<T>& listItems) const {
		for (const auto& i : m_items) {
			listItems.push_back(i.second);
		}

		for (int i = 0; i < 4; i++) {
			if (m_pChild[i]) {
				m_pChild[i]->items(listItems);
			}
		}
	}

protected:
	// Maximum tree depth
	const size_t MAX_DEPTH = 4;
	size_t m_depth = 0;

	// Area of the tree
	QTRect m_area;

	// 4 children of tree
	std::array<QTRect, 4> m_child{};

	// 4 potential children of tree
	std::array<std::shared_ptr<StaticQuadTree<T>>, 4> m_pChild{};

	// Items contained by this tree
	std::vector<std::pair<QTRect, T>> m_items;
};