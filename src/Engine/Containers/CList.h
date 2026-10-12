#ifndef LEMBALL_ENGINE_CONTAINERS_CLIST_H
#define LEMBALL_ENGINE_CONTAINERS_CLIST_H

#include <stddef.h>

template <class T>
class CListElement {
public:
	CListElement(T* p_object) : m_object(p_object), m_next(NULL), m_prev(NULL) {}
	T* m_object;
	CListElement<T>* m_next;
	CListElement<T>* m_prev;
};

template <class T, class Element>
class CList {
public:
	CList() : m_head(NULL), m_tail(NULL), m_count(0) {}
	~CList()
	{
		Element* node = m_head;
		while (node != NULL) {
			Element* next = node->m_next;
			delete node;
			node = next;
		}
	}
	Element* FindElement(T* p_object)
	{
		Element* node = m_head;
		while (node != NULL && node->m_object != p_object) {
			node = node->m_next;
		}
		return node;
	}
	void Add(T* p_object)
	{
		Element* node = new Element(p_object);
		node->m_prev = m_tail;
		if (m_tail != NULL) {
			m_tail->m_next = node;
		}
		m_tail = node;
		if (m_head == NULL) {
			m_head = node;
		}
		++m_count;
	}
	void Remove(T* p_object)
	{
		Element* node = FindElement(p_object);
		if (node != NULL) {
			Element* next = node->m_next;
			Element* prev = node->m_prev;
			delete node;
			if (next != NULL) {
				next->m_prev = prev;
			}
			else {
				m_tail = prev;
			}
			if (prev != NULL) {
				prev->m_next = next;
			}
			else {
				m_head = next;
			}
			--m_count;
		}
	}
	Element* m_head;
	Element* m_tail;
	unsigned int m_count;
};

#endif
