#pragma once
#pragma once

#include "includes.h"

template<typename T>
struct Node
{
	T value;
	Node<T>* Next;
	Node<T>* Back;

	Node(void)
	{
		Next = nullptr;
		Back = nullptr;
	}
	~Node(void)
	{
		Next = nullptr;
		Back = nullptr;
		value = NULL;
	}
};

namespace PoolingStack
{
	struct FunctionContainer
	{
		typedef std::function<void(void)> func;
		func f;
		FunctionContainer(std::function<void(void)> function) :f(std::move(function)) {}
		~FunctionContainer()
		{
			if (f)
			{
				f();
			}
		}
	};

	template<typename T>
	class LockFreePool
	{
	public:
		LockFreePool(unsigned int PreservingSize = 10000) : _pushes(0), _pops(0)
		{
			if (PreservingSize < 1)
			{
				PreservingSize = 10000;
			}
			while (_pushes < PreservingSize)
			{
				Node<T>* NewNode = new Node<T>();
				if (_pushes == 0)
				{
					NewNode->Next = nullptr;
				}
				else
				{
					NewNode->Next = _pHead;
				}
				_pHead = NewNode;
				++_pushes;
			}
		}
		~LockFreePool(void)
		{
			while (true)
			{
				if (!_pHead->Next)
				{
					delete _pHead;
					break;
				}
				else
				{
					Node<T>* preHead = _pHead;
					_pHead = preHead->Next;
					delete preHead;
				}
			}
		}
		void push(Node<T>* NewNode)
		{
			Node<T>* TempHead = _pHead;
			NewNode->Next = TempHead;
			_pHead = NewNode;
			++_pushes;
		}
		Node<T>* pop(void)
		{
			Node<T>* tempHead = _pHead;
			Node<T>* ReturningNode;
			if (!tempHead->Next)
			{
				ReturningNode = new Node<T>();
			}
			else
			{
				ReturningNode = tempHead->Next;
				tempHead->Next = ReturningNode->Next;
			}
			ReturningNode->Next = nullptr;
			ReturningNode->Back = nullptr;
			ReturningNode->value = NULL;
			++_pops;
			return ReturningNode;
		}
		int size(void) { return _pushes - _pops; }
	private:
		Node<T>* _pHead;
		unsigned int _pushes;
		unsigned int _pops;
	};

	template<typename T>
	class PoolingStack
	{
	public:
		PoolingStack(int PreservingSize = 10000) :_pushes(0), _pops(0), _poplock(false), _pushlock(false)
		{
			if (PreservingSize < 1)
			{
				PreservingSize = 10000;
			}
			_UsedPool = new LockFreePool<T>(PreservingSize);
			_pHead = _UsedPool->pop();
		}
		~PoolingStack(void)
		{
			if (_pHead)
			{
				while (true)
				{
					if (!_pHead->Next)
					{
						delete _pHead;
						break;
					}
					else
					{
						Node<T>* preHead = _pHead;
						_pHead = preHead->Next;
						delete preHead;
					}
				}
			}
			delete _UsedPool;
		}
		void push(T val)
		{
			if (FunctionContainer releaser = _getpushlock(); releaser.f)
			{
				Node <T>* newNode;
				newNode = _UsedPool->pop();
				newNode->value = val;
				newNode->Next = _pHead;
				_pHead = newNode;
				++_pushes;
			}
		}
		T pop(void)
		{
			if (FunctionContainer releaser = _getpoplock(); releaser.f)
			{
				Node <T>* ReturningNode;
				if (!_pHead->Next)
				{
					return NULL;
				}
				else
				{
					ReturningNode = _pHead;
					_pHead = _pHead->Next;
					T val = ReturningNode->value;
					ReturningNode->Next = nullptr;
					ReturningNode->Back = nullptr;
					ReturningNode->value = NULL;
					_UsedPool->push(ReturningNode);
					++_pops;
					return val;
				}
			}
			return NULL;
		}

		int Count(void) { return _pushes - _pops; }

	private:
		Node<T>* _pHead;
		LockFreePool<T>* _UsedPool;
		unsigned int _pushes;
		unsigned int _pops;

		std::atomic<bool> _poplock;
		std::atomic<bool> _pushlock;
		std::function<void(void)> _getpushlock(void)
		{
			bool expected = false;
			bool desired = true;
			for (;;)
			{
				if (_pushlock.compare_exchange_weak(expected, desired))
				{
					return [this] {releasepush(); };
				}
				else
				{
					expected = false;
				}
			}
		}
		std::function<void(void)> _getpoplock(void)
		{
			bool expected = false;
			bool desired = true;
			for (;;)
			{
				if (_poplock.compare_exchange_weak(expected, desired))
				{
					return [this] {releasepop(); };
				}
				else
				{
					expected = false;
				}
			}
		}
		void releasepush(void) { _pushlock = false; }
		void releasepop(void) { _poplock = false; }
	};
}

namespace LockFreeQueue
{
	struct FunctionContainer
	{
		typedef std::function<void(void)> func;
		func f;
		FunctionContainer(std::function<void(void)> function) :f(std::move(function)) {}
		~FunctionContainer()
		{
			if (f)
			{
				f();
			}
		}
	};

	template<typename T>
	class LockFreePool
	{
	public:
		LockFreePool(unsigned int PreservingSize = 10000) : _pushes(0), _pops(0)
		{
			if (PreservingSize < 1)
			{
				PreservingSize = 10000;
			}
			while (_pushes < PreservingSize)
			{
				Node<T>* NewNode = new Node<T>();
				if (_pushes == 0)
				{
					NewNode->Next = nullptr;
				}
				else
				{
					NewNode->Next = _pHead;
				}
				_pHead = NewNode;
				++_pushes;
			}
		}
		~LockFreePool(void)
		{
			while (true)
			{
				if (!_pHead->Next)
				{
					delete _pHead;
					break;
				}
				else
				{
					Node<T>* preHead = _pHead;
					_pHead = preHead->Next;
					delete preHead;
				}
			}
		}
		void push(Node<T>* NewNode)
		{
			Node<T>* TempHead = _pHead;
			NewNode->Next = TempHead;
			_pHead = NewNode;
			++_pushes;
		}
		Node<T>* pop(void)
		{
			Node<T>* tempHead = _pHead;
			Node<T>* ReturningNode;
			if (!tempHead->Next)
			{
				ReturningNode = new Node<T>();
			}
			else
			{
				ReturningNode = tempHead->Next;
				tempHead->Next = ReturningNode->Next;
			}
			ReturningNode->Next = nullptr;
			ReturningNode->Back = nullptr;
			ReturningNode->value = NULL;
			++_pops;
			return ReturningNode;
		}
		int size(void) { return _pushes - _pops; }
	private:
		Node<T>* _pHead;
		unsigned int _pushes;
		unsigned int _pops;
	};

	template<typename T>
	class MIMOQueue
	{
	public:
		MIMOQueue<T>* NextQueue;
		std::atomic<int> DequeingTh;
		std::atomic<int> EnqueingTh;

		MIMOQueue(int PoolSize = 20000) : DequeingTh(0), EnqueingTh(0), _Enqueues(0), _Dequeues(0), _Enqueuelock(false), _Dequeuelock(false)
		{
			if (PoolSize < 2)
			{
				PoolSize = 20000;
			}
			_NodePool = new LockFreePool<T>(PoolSize);
			_pHead = _NodePool->pop();
			_pTail = _pHead;
			_pHeadTemp = _NodePool->pop();
		}
		~MIMOQueue(void)
		{
			NextQueue = nullptr;
			delete _NodePool;
			for (;;)
			{
				if (!_pTail->Back)
				{
					break;
				}
				else
				{
					Node<T>* nd;
					nd = _pTail;
					_pTail = nd->Back;
					nd->value = NULL;
					_pTail->Next = nullptr;
					delete nd;
				}
			}
			delete _pHead;
			delete _pTail;
		}

		void Enqueue(T val)
		{
			if (FunctionContainer releaser = _GetEnqueuelock(); releaser.f)
			{
				Node<T>* newNode;
				newNode = _NodePool->pop();
				newNode->value = val;
				Node<T>* OldHead = _pHead;
				newNode->Next = OldHead;
				_pHead = newNode;
				OldHead->Back = newNode;
				_pHeadTemp->value = newNode->value;
				_pHeadTemp->Next = newNode->Next;
				_pHeadTemp->Back = newNode->Back;
				++_Enqueues;
			}
		}
		T Dequeue(void)
		{
			T val;
			if (FunctionContainer releaser = _GetDequeuelock(); releaser.f)
			{
				Node<T>* preTail = _pTail;
				if (!preTail->Back)
				{
					return NULL;
				}
				val = preTail->Back->value;
				_pTail = _pTail->Back;
				_pTail->Next = nullptr;
				_pTail->value = NULL;
				preTail->Back = nullptr;
				_NodePool->push(preTail);
				++_Dequeues;
				return val;
			}
			else
			{
				return NULL;
			}
		}
		T Peak(void)
		{
			if (FunctionContainer releaser = _GetDequeuelock(); releaser.f)
			{
				if (!_pTail->Back)
				{
					return NULL;
				}
				else
				{
					return _pTail->Back->value;
				}
			}
		}
		int Count(void) { _Enqueues - _Dequeues; }

	private:
		Node<T>* _pHead;
		Node<T>* _pHeadTemp;
		Node<T>* _pTail;
		LockFreePool<T>* _NodePool;
		unsigned int _Enqueues;
		unsigned int _Dequeues;

		std::atomic<bool> _Enqueuelock;
		std::atomic<bool> _Dequeuelock;
		std::function<void(void)> _GetEnqueuelock(void)
		{
			bool expected = false;
			bool desired = true;
			for (;;)
			{
				if (_Enqueuelock.compare_exchange_weak(expected, desired))
				{
					return [this] {ReleaseEnqueue(); };
				}
				else
				{
					expected = false;
				}
			}
		}
		std::function<void(void)> _GetDequeuelock(void)
		{
			bool expected = false;
			bool desired = true;
			for (;;)
			{
				if (_Dequeuelock.compare_exchange_weak(expected, desired))
				{
					return [this] {ReleaseDequeue(); };
				}
				else
				{
					expected = false;
				}
			}
		}
		void ReleaseEnqueue(void) { _Enqueuelock = false; }
		void ReleaseDequeue(void) { _Dequeuelock = false; }
	};

	template<typename T>
	class SIMOQueue
	{
	public:
		SIMOQueue<T>* NextQueue;
		std::atomic<int> DequeingTh;
		std::atomic<int> EnqueingTh;

		SIMOQueue(int PoolSize = 20000) : DequeingTh(0), EnqueingTh(0), _Enqueues(0), _Dequeues(0), _Dequeuelock(false)
		{
			if (PoolSize < 2)
			{
				PoolSize = 20000;
			}
			_NodePool = new LockFreePool<T>(PoolSize);
			_pHead = _NodePool->pop();
			_pTail = _pHead;
			_pHeadTemp = _NodePool->pop();
		}
		~SIMOQueue(void)
		{
			NextQueue = nullptr;
			delete _NodePool;
			for (;;)
			{
				if (!_pTail->Back)
				{
					break;
				}
				else
				{
					Node<T>* nd;
					nd = _pTail;
					_pTail = nd->Back;
					nd->value = NULL;
					_pTail->Next = nullptr;
					delete nd;
				}
			}
			delete _pHead;
			delete _pTail;
		}

		void Enqueue(T val)
		{
			Node<T>* newNode;
			newNode = _NodePool->pop();
			newNode->value = val;
			Node<T>* OldHead = _pHead;
			newNode->Next = OldHead;
			_pHead = newNode;
			OldHead->Back = newNode;
			_pHeadTemp->value = newNode->value;
			_pHeadTemp->Next = newNode->Next;
			_pHeadTemp->Back = newNode->Back;
			++_Enqueues;
		}
		T Dequeue(void)
		{
			T val;
			if (FunctionContainer releaser = _GetDequeuelock())
			{
				Node<T>* preTail = _pTail;
				if (!preTail->Back)
				{
					return NULL;
				}
				val = preTail->Back->value;
				_pTail = _pTail->Back;
				_pTail->Next = nullptr;
				_pTail->value = NULL;
				preTail->Back = nullptr;
				_NodePool->push(preTail);
				++_Dequeues;
				return val;
			}
			else
			{
				return NULL;
			}
		}
		T Peak(void)
		{
			if (FunctionContainer releaser = _GetDequeuelock())
			{
				if (!_pTail->Back)
				{
					return NULL;
				}
				else
				{
					return _pTail->Back->value;
				}
			}
		}
		int Count(void) { _Enqueues - _Dequeues; }

	private:
		Node<T>* _pHead;
		Node<T>* _pHeadTemp;
		Node<T>* _pTail;
		LockFreePool<T>* _NodePool;
		unsigned int _Enqueues;
		unsigned int _Dequeues;

		std::atomic<bool> _Dequeuelock;
		std::function<void(void)> _GetDequeuelock(void)
		{
			bool expected = false;
			bool desired = true;
			for (;;)
			{
				if (_Dequeuelock.compare_exchange_weak(expected, desired))
				{
					return [this] {ReleaseDequeue(); };
				}
				else
				{
					expected = false;
				}
			}
		}
		void ReleaseDequeue(void) { _Dequeuelock = false; }
	};

	template<typename T>
	class MISOQueue
	{
	public:
		MISOQueue<T>* NextQueue;
		std::atomic<int> DequeingTh;
		std::atomic<int> EnqueingTh;

		MISOQueue(int PoolSize = 20000) : DequeingTh(0), EnqueingTh(0), _Enqueues(0), _Dequeues(0), _Enqueuelock(false)
		{
			if (PoolSize < 2)
			{
				PoolSize = 20000;
			}
			_NodePool = new LockFreePool<T>(PoolSize);
			_pHead = _NodePool->pop();
			_pTail = _pHead;
			_pHeadTemp = _NodePool->pop();
		}
		~MISOQueue(void)
		{
			NextQueue = nullptr;
			delete _NodePool;
			for (;;)
			{
				if (!_pTail->Back)
				{
					break;
				}
				else
				{
					Node<T>* nd;
					nd = _pTail;
					_pTail = nd->Back;
					nd->value = NULL;
					_pTail->Next = nullptr;
					delete nd;
				}
			}
			delete _pHead;
			delete _pTail;
		}

		void Enqueue(T val)
		{
			if (FunctionContainer releaser = _GetEnqueuelock())
			{
				Node<T>* newNode;
				newNode = _NodePool->pop();
				newNode->value = val;
				Node<T>* OldHead = _pHead;
				newNode->Next = OldHead;
				_pHead = newNode;
				OldHead->Back = newNode;
				_pHeadTemp->value = newNode->value;
				_pHeadTemp->Next = newNode->Next;
				_pHeadTemp->Back = newNode->Back;
				++_Enqueues;
			}
		}
		T Dequeue(void)
		{
			T val;
			Node<T>* preTail = _pTail;
			if (!preTail->Back)
			{
				return NULL;
			}
			val = preTail->Back->value;
			_pTail = _pTail->Back;
			_pTail->Next = nullptr;
			_pTail->value = NULL;
			preTail->Back = nullptr;
			_NodePool->push(preTail);
			++_Dequeues;
			return val;
		}
		T Peak(void)
		{
			if (!_pTail->Back)
			{
				return NULL;
			}
			else
			{
				return _pTail->Back->value;
			}
		}

		int Count(void) { _Enqueues - _Dequeues; }

	private:
		Node<T>* _pHead;
		Node<T>* _pHeadTemp;
		Node<T>* _pTail;
		LockFreePool<T>* _NodePool;
		unsigned int _Enqueues;
		unsigned int _Dequeues;

		std::atomic<bool> _Enqueuelock;
		std::function<void(void)> _GetEnqueuelock(void)
		{
			bool expected = false;
			bool desired = true;
			for (;;)
			{
				if (_Enqueuelock.compare_exchange_weak(expected, desired))
				{
					return [this] {ReleaseEnqueue(); };
				}
				else
				{
					expected = false;
				}
			}
		}
		void ReleaseEnqueue(void) { _Enqueuelock = false; }
	};

	template<typename T>
	class SISOQueue
	{
	public:
		SISOQueue<T>* NextQueue;

		SISOQueue(int PoolSize = 20000) : _Enqueues(0), _Dequeues(0)
		{
			if (PoolSize < 2)
			{
				PoolSize = 20000;
			}
			_NodePool = new LockFreePool<T>(PoolSize);
			_pHead = _NodePool->pop();
			_pTail = _pHead;
			_pHeadTemp = _NodePool->pop();
		}
		~SISOQueue(void)
		{
			NextQueue = nullptr;
			delete _NodePool;
			for (;;)
			{
				if (!_pTail->Back)
				{
					break;
				}
				else
				{
					Node<T>* nd;
					nd = _pTail;
					_pTail = nd->Back;
					nd->value = NULL;
					_pTail->Next = nullptr;
					delete nd;
				}
			}
			delete _pHead;
			delete _pTail;
		}

		void Enqueue(T val)
		{
			Node<T>* newNode;
			newNode = _NodePool->pop();
			newNode->value = val;
			Node<T>* OldHead = _pHead;
			newNode->Next = OldHead;
			_pHead = newNode;
			OldHead->Back = newNode;
			_pHeadTemp->value = newNode->value;
			_pHeadTemp->Next = newNode->Next;
			_pHeadTemp->Back = newNode->Back;
			++_Enqueues;
		}
		T Dequeue(void)
		{
			T val;
			Node<T>* preTail = _pTail;
			if (!preTail->Back)
			{
				return NULL;
			}
			val = preTail->Back->value;
			_pTail = _pTail->Back;
			_pTail->Next = nullptr;
			_pTail->value = NULL;
			preTail->Back = nullptr;
			_NodePool->push(preTail);
			++_Dequeues;
			return val;
		}
		T Peak(void)
		{
			if (!_pTail->Back)
			{
				return NULL;
			}
			else
			{
				return _pTail->Back->value;
			}
		}

		int Count(void) { return _Enqueues - _Dequeues; }

	private:
		Node<T>* _pHead;
		Node<T>* _pHeadTemp;
		Node<T>* _pTail;
		LockFreePool<T>* _NodePool;
		unsigned int _Enqueues;
		unsigned int _Dequeues;
	};
}
