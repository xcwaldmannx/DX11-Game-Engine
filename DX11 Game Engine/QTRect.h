#pragma once

#include "Point2f.h"

struct QTRect {
	Point2f position;
	Point2f size;

	QTRect(const Point2f& position = { 0.0f, 0.0f }, const Point2f& size = { 1.0f, 1.0f }) : position(position), size(size) {
	}

	constexpr bool contains(const Point2f& point) const {
		return (position.x >= point.x && position.x <= point.x + size.x) &&
			(position.y >= point.y && position.y <= point.y + size.y);
	}

	constexpr bool contains(const QTRect& rect) const {
		return (rect.position.x >= position.x) && (rect.position.x + rect.size.x < position.x + size.x) &&
			(rect.position.y >= position.y) && (rect.position.y + rect.size.y < position.y + size.y);
	}

	constexpr bool overlaps(const QTRect& rect) const {
		return (position.x < rect.position.x + rect.size.x) && (position.x + size.x >= rect.position.x) &&
			(position.y < rect.position.y + rect.size.y) && (position.y + size.y >= rect.position.y);
	}
};