#pragma once
#include <assert.h>
#include <initializer_list>
#include <utility>

namespace LinkedList2024 {
	template<class DataType>
	class LinkedList;

	template<class DataType>
	class LLIterator;

	// class for storing nodes of the linked list
	template<class DataType>
	class LLNode
	{
		friend class LinkedList<DataType>;
		friend class LLIterator<DataType>;

		LLNode *_prev, *_next;
		DataType _d;

		LLNode(DataType&& value)
			: _d(std::move(value)) {
			_prev = nullptr;
			_next = nullptr;
		}

		LLNode(const DataType& value)
			: _d(value) {
			_prev = nullptr;
			_next = nullptr;
		}
	};

	// iterator class
	template<class DataType>
	class LLIterator
	{
		friend class LinkedList<DataType>;

		LLNode<DataType>* _ptr;
		bool _reverse;

		LLIterator(LLNode<DataType>* newPtr, bool reverseDirection=false) {
			_ptr = newPtr;
			_reverse = reverseDirection;
		}


	public:
		LLIterator<DataType>& operator++() {
			if (_reverse) {
				_ptr = _ptr->_prev;
			}
			else {
				_ptr = _ptr->_next;
			}
			return *this;
		}

		LLIterator<DataType>& operator--() {
			if (_reverse) {
				_ptr = _ptr->_next;
			}
			else {
				_ptr = _ptr->_prev;
			}
			return *this;
		}

		bool operator!=(const LLIterator &other) const {
			return _ptr != other._ptr;
		}

		DataType& operator*() const {
			return _ptr->_d;
		}
	};

	// the class for the linked list container, the meat of the code
	template<class DataType>
	class LinkedList
	{
	public:
		using Iterator = LLIterator<DataType>;
		using Node = LLNode<DataType>;

		~LinkedList() {
			clear();
		}

		LinkedList() {
			_head = nullptr;
			_tail = nullptr;
		}

		// move constructor
		LinkedList(LinkedList<DataType>&& other) {
			_head = other._head;
			_tail = other._tail;
			other._head = nullptr;
			other._tail = nullptr;
		}

		LinkedList(const std::initializer_list<DataType>& init) {
			_head = nullptr;
			_tail = nullptr;

			operator=(init);
		}

		auto& operator=(const std::initializer_list<DataType>& init) {
			clear();
			for (auto& v : init) {
				push_back(v);
			}
			return *this;
		}

		// insert by move
		auto insert(Iterator it, DataType&& data) {
			auto* n = new Node(std::move(data));
			return _insert(it, n);
		}

		// insert copy by reference
		auto insert(Iterator it, const DataType& data) {
			auto* n = new Node(data);
			return _insert(it, n);
		}

		auto push_front(DataType&& data) {
			return insert(rbegin(), std::move(data));
		}

		auto push_front(const DataType& data) {
			return insert(rbegin(), data);
		}

		auto push_back(DataType&& data) {
			return insert(end(), std::move(data));
		}

		auto push_back(const DataType& data) {
			return insert(end(), data);
		}

		// clear everything in the list
		void clear() {
			while(_head != nullptr) {
				auto p = _head;
				_head = p->_next;
				delete p;
			}
			_tail = nullptr;
		}

		// erase 1 item by iterator
		auto erase(Iterator it) {
			Node* n = it._ptr;
			assert(n != nullptr);
			++it;
			auto prev = n->_prev;
			auto next = n->_next;

			if (_head == n) {
				_head = next;
			}
			if (_tail == n) {
				_tail = prev;
			}
			if (prev != nullptr) {
				prev->_next = next;
			}
			if (next != nullptr) {
				next->_prev = prev;
			}

			delete n;
			return it;
		}

		// return iterator of first item matching value
		Iterator find_first(const DataType& val) const {
			for (Iterator it = begin(); it != end(); ++it) {
				if (*it == val) {
					return it;
				}
			}
			return end();
		}

		auto pop_front() {
			return erase(begin());
		}

		auto pop_back() {
			return erase(rend());
		}

		Iterator begin() const {
			return Iterator(_head);
		}

		Iterator end() const {
			return Iterator(nullptr);
		}

		// reverse iterator starting from beginning
		Iterator rbegin() const {
			return Iterator(nullptr, true);
		}

		// reverse iterator starting from the beginning
		Iterator rend() const {
			return Iterator(_tail, true);
		}

    private:
		Node* _head, * _tail;

        auto _insert(const Iterator& it, Node* n) {
            assert(n != nullptr);

            auto ptr = it._ptr;

            if (it._reverse && ptr == nullptr) {
                n->_prev = nullptr;
                n->_next = _head;
            }
            else if (it._reverse && ptr != nullptr) {
                n->_prev = ptr->_prev;
                n->_next = ptr;
            }
            else if (!it._reverse && ptr != nullptr) {
                n->_prev = ptr;
                n->_next = ptr->_next;
            }
            else {
                // ptr is null, this is for push_back
                n->_prev = _tail;
                n->_next = nullptr;
            }

            if (n->_prev == nullptr) {
                _head = n;
                _tail = n;
            }
            else if (n->_prev == _tail) {
                _tail = n;
            }
            if (n->_prev != nullptr) {
                n->_prev->_next = n;
            }
            if (n->_next != nullptr) {
                n->_next->_prev = n;
            }
            return Iterator(n);
        }
	};

};
