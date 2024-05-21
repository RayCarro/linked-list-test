#pragma once
#include <assert.h>
#include <initializer_list>
#include <utility>

namespace LinkedList2024 {
	template<class DataType>
	class LinkedList;
	template<class DataType>
	class Iterator;

	template<class DataType>
	class Node
	{
		friend class LinkedList<DataType>;
		friend class Iterator<DataType>;

		Node *_prev, *_next;
		DataType _d;

		Node(DataType&& value)
			: _d(std::move(value)) {
			_prev = nullptr;
			_next = nullptr;
		}

		Node(const DataType& value)
			: _d(value) {
			_prev = nullptr;
			_next = nullptr;
		}
	};

	template<class DataType>
	class Iterator
	{
		friend class LinkedList<DataType>;

		Node<DataType>* _ptr;
		bool _reverse;

		Iterator(Node<DataType>* newPtr, bool reverseDirection=false) {
			_ptr = newPtr;
			_reverse = reverseDirection;
		}


	public:
		Iterator<DataType>& operator++() {
			if (_reverse) {
				_ptr = _ptr->_prev;
			}
			else {
				_ptr = _ptr->_next;
			}
			return *this;
		}

		Iterator<DataType>& operator--() {
			if (_reverse) {
				_ptr = _ptr->_next;
			}
			else {
				_ptr = _ptr->_prev;
			}
			return *this;
		}

		bool operator!=(const Iterator &other) const {
			return _ptr != other._ptr;
		}

		DataType& operator*() const {
			return _ptr->_d;
		}
	};

	template<class DataType>
	class LinkedList
	{
	public:
		~LinkedList() {
			clear();
		}

		LinkedList() {
			_first = nullptr;
			_last = nullptr;
		}

		LinkedList(LinkedList<DataType>&& other) {
			_first = other._first;
			_last = other._last;
			other._first = nullptr;
			other._last = nullptr;
		}

		LinkedList(const std::initializer_list<DataType>& init) {
			_first = nullptr;
			_last = nullptr;

			operator=(init);
		}

		auto& operator=(const std::initializer_list<DataType>& init) {
			clear();
			for (auto& v : init) {
				push_back(v);
			}
			return *this;
		}

		auto insert(Iterator<DataType> it, DataType&& data) {
			auto* n = new Node<DataType>(std::move(data));
			return _insert(it, n);
		}

		auto insert(Iterator<DataType> it, const DataType& data) {
			auto* n = new Node<DataType>(data);
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

		void clear() {
			while(_first != nullptr) {
				auto p = _first;
				_first = p->_next;
				delete p;
			}
			_last = nullptr;
		}

		auto erase(Iterator<DataType> it) {
			Node<DataType>* n = it._ptr;
			assert(n != nullptr);
			++it;
			auto prev = n->_prev;
			auto next = n->_next;

			if (_first == n) {
				_first = next;
			}
			if (_last == n) {
				_last = prev;
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

		Iterator<DataType> find_first(const DataType& val) const {
			for (Iterator<DataType> it = begin(); it != end(); ++it) {
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

		Iterator<DataType> begin() const {
			return Iterator<DataType>(_first);
		}

		Iterator<DataType> end() const {
			return Iterator<DataType>(nullptr);
		}

		Iterator<DataType> rbegin() const {
			return Iterator<DataType>(nullptr, true);
		}

		Iterator<DataType> rend() const {
			return Iterator<DataType>(_last, true);
		}

    private:
        Node<DataType>* _first, * _last;

        auto _insert(const Iterator<DataType>& it, Node<DataType>* n) {
            assert(n != nullptr);

            auto ptr = it._ptr;

            if (it._reverse && ptr == nullptr) {
                n->_prev = nullptr;
                n->_next = _first;
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
                n->_prev = _last;
                n->_next = nullptr;
            }

            if (n->_prev == nullptr) {
                _first = n;
                _last = n;
            }
            else if (n->_prev == _last) {
                _last = n;
            }
            if (n->_prev != nullptr) {
                n->_prev->_next = n;
            }
            if (n->_next != nullptr) {
                n->_next->_prev = n;
            }
            return Iterator<DataType>(n);
        }
	};

};
