#pragma once
#include <iostream>

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

using namespace std;

namespace ElectionsDay
{
	template <class T>
	class Node
	{

	public:

		Node() : n_data(nullptr), n_next(nullptr) {}
		Node(T& dataNode) : n_data(new T(dataNode)), n_next(nullptr) {}

		~Node()
		{
			if (n_data!=nullptr)
			{
				delete n_data;
			}
			if (n_next != nullptr)
			{
				delete n_next;
			}
		}

		T* getData() const { return this->n_data; }
		Node<T>* getNextNode() const { return this->n_next; }
		void setNext(Node* next) { n_next = next; }
		void setData(T* data) { n_data = data; }

		void save(ofstream& outFile) const
		{
			try
			{
				this->getData()->save(outFile);
			}
			catch (SaveException& ex)
			{
				throw;
			}
		}

	private:
		T* n_data;
		Node<T>* n_next;
	};

	template <class T>
	class List
	{
	private:
		Node<T>* head;
		Node<T>* tail;
		int size;

	public:
		class Iterator
		{
		private:
			Node<T>* ptr;

		public:
			using iterator_category = std::forward_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

		public:
			Iterator() = default;
			Iterator(Node<T>* ptrNode) :ptr(ptrNode) {}
			Iterator(const Iterator& ptrNode) : ptr(ptrNode.ptr) {}
			const Iterator& operator=(const Iterator& other) { ptr = other.ptr; return *this; }
			Iterator& operator++() { ptr = ptr->getNextNode(); return *this; }
			Iterator operator++(int) { Iterator tmpItr(*this); ptr = ptr->getNextNode(); return tmpItr; }
			Iterator operator+(int i) { Iterator tmpItr(*this); return ++tmpItr; }
			bool operator==(const Iterator& other) const { return (ptr == other.ptr); }
			bool operator!=(const Iterator& other) const { return !(*this == other); }
			T& operator*() { return *(ptr->getData()); }
			T* operator->() { return ptr->getData(); }

			friend class const_Iterator;
		};

		class const_Iterator
		{
		private:
			Node<T>* ptr;
		public:
			const_Iterator() = default;
			const_Iterator(Node<T>* ptrNode) :ptr(ptrNode) {}
			const_Iterator(const Iterator& ptrNode) :ptr(ptrNode.ptr) {}
			const const_Iterator& operator=(const const_Iterator& other) { ptr = other.ptr; return *this; }
		};

	public:
		List() : head(nullptr), tail(nullptr), size(0) {}
		List(List& other) :head(other.head), tail(other.tail), size(other.size) {}
		~List() {};
		int getSize() const { return size; };
		void setSize() { size++; }
		Iterator begin() { return Iterator(head); }
		Iterator end() { return Iterator(nullptr); }
		Iterator& insert(const Iterator& pos, const T& val) 
		{
			Node newNode(val);
			for (Iterator itr = this->begin(); itr != pos || itr != this->end(); ++itr)
			{
				if (itr + 1 == pos)
				{
					newNode->next = pos;
					itr->next = newNode;
				}
			}
			Iterator* res = new Iterator(&newNode);
			return &res;
		}
		const Iterator& erase(const Iterator& itr)
		{
			Iterator i = this->begin();
			for (i; i != this->end(); ++i)
			{
				if (i + 1 == itr)
				{
					i->next = itr->next;
					delete itr;
				}
			}
			++i;
			Iterator* pItr = &i;
			return &pItr;
		}
		const Iterator& erase(const Iterator& itr1, const Iterator& itr2)
		{
			Iterator i = this->begin();
			for (i; i != this->end(); ++i)
			{
				if(i + 1 == itr1)
				{
					i->next = itr2->next;
					for (Iterator j = itr1; (j != itr2->next && j != this->end());)
					{
						Iterator tempItr = j;
						++j;
						delete tempItr;
					}
				}
			}
			++i;
			Iterator* pItr = &i;
			return &pItr;
		}
		const_Iterator begin() const { return const_Iterator(head); }
		const_Iterator end() const { return const_Iterator(nullptr); }
		const const_Iterator cbegin() const { return const_Iterator(head); }
		const const_Iterator cend() const { return const_Iterator(nullptr); }

		bool isEmptyLst() const
		{
			if (this->size == 0)
				return true;
			else
				return false;
		}

		void Insert(T& data)
		{
			Node<T>* newNode = new Node<T>(data);
			newNode->setData(&data);

			if (isEmptyLst())
				head = tail = newNode;
			else
			{
				tail->setNext(newNode);
				tail = newNode;
			}
			size++; //increase the size of the list.
		}

		T* findNode(int number) const
		{
			Node<T>* curr = head;
			while (curr)
			{
				if (curr->getData()->getID() == number)
					return curr->getData();

				curr = curr->getNextNode();
			}
			return nullptr; // the node is not at the list.
		}
		void PrintListData(const char* str) const
		{
			if (isEmptyLst())
			{
				cout << "There are no " << str << "'s at the state." << endl;
			}
			else
			{
				cout << str << "'s details:" << endl;
				Node<T>* curr = this->head;
				int i = 1;
				while (curr)
				{
					cout << i++ << ". "<< *(curr->getData()) << endl;
					curr = curr->getNextNode();
				}

			}
		}
		void save(ofstream& outFile) const  // This function saves the district list to a given file.
		{
			outFile.write(rcastcc(&size), sizeof(int));
			if (!outFile.good())
				throw SaveException();

			Node<T>* curr = head;
			while (curr)
			{
				try
				{
					curr->save(outFile);
					curr = curr->getNextNode();
				}
				catch (SaveException& ex)
				{
					throw;
				}
			}
		}

		//**************************Load Fanction****************************************//

		List(ifstream& inFile) 
		{
			int sizeOfList;
			inFile.read(rcastc(&sizeOfList), sizeof(int));  
			for (int i = 0; i < sizeOfList; i++)
			{
				T* currNode = new T(inFile); 
				this->Insert(*currNode);
			}
		}

		template <class L1>
		List(ifstream& inFile, List<L1>* lst1) 
		{
			int sizeOfList;

			inFile.read(rcastc(&sizeOfList), sizeof(int)); 
			for (int i = 0; i < sizeOfList; i++)
			{
				T* currNode = new T(inFile, lst1); 
				this->Insert(*currNode); 
			}
		}

		template <class L1, class L2>
		List(ifstream& inFile, List<L1>* lst1, List<L2>* lst2) 
		{
			int sizeOfList;
			inFile.read(rcastc(&sizeOfList), sizeof(int));
			for (int i = 0; i < sizeOfList; i++)
			{
				T* currNode = new T(inFile, lst1, lst2); 
				this->Insert(*currNode);
			}
		}
	};
//******************************************************************//
};


