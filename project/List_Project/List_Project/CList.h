#ifndef __CLIST_H__
#define __CLIST_H__

template<typename T>

class CList
{
public: 
    struct Node 
    {
        T _data;
        Node *_Prev;
        Node *_Next;
    };

    class iterator 
    {
    private:
        Node *_node;
    public:
        iterator(Node *node = nullptr)
        {
            // 
        }

        iterator operator ++(int)
        {

        }
        iterator& operator ++()
        {
            return *this;
        }
        iterator operator --(int)
        {

        }
        iterator& operator --()
        {
            return *this;
        }

        T& operator *()
        {

        }
        bool operator ==(const iterator& other)
        {
        }
        bool operator !=(const iterator& other)
        {
        }
    };

public:
    CList();
    ~CList();



};


#endif
