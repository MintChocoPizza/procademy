#pragma onc#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

template <class DATA>
struct st_BLOCK_NODE {
    DATA data;
    st_BLOCK_NODE* next;
};

template <class DATA>
class CMemoryPool
{
public:
    CMemoryPool(int iBlockNum, bool bPlacementNew = false) : m_iCapacity(iBlockNum), m_iUseCount(0), _pFreeNode(nullptr) {
        _pFreeNode = new st_BLOCK_NODE<DATA>[m_iCapacity];
        for (int i = 0; i < m_iCapacity - 1; ++i) {
            _pFreeNode[i].next = &_pFreeNode[i + 1];
        }
        _pFreeNode[m_iCapacity - 1].next = nullptr;
    }

    virtual ~CMemoryPool() {
        delete[] _pFreeNode;
    }

    DATA* Alloc(void) {
        if (_pFreeNode == nullptr) {
            return nullptr;
        }
        st_BLOCK_NODE<DATA>* node = _pFreeNode;
        _pFreeNode = _pFreeNode->next;
        ++m_iUseCount;
        return &node->data;
    }

    bool Free(DATA* pData) {
        st_BLOCK_NODE<DATA>* node = reinterpret_cast<st_BLOCK_NODE<DATA>*>(pData);
        node->next = _pFreeNode;
        _pFreeNode = node;
        --m_iUseCount;
        return true;
    }

    int GetCapacityCount(void) { return m_iCapacity; }
    int GetUseCount(void) { return m_iUseCount; }

private:
    int m_iCapacity;
    int m_iUseCount;
    st_BLOCK_NODE<DATA>* _pFreeNode;
};

#endif // !__MEMORY_POOL_H__
e
