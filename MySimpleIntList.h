#include <cstddef>

// it is cyclic: ++List.end() == List.begin()
class List {
public:
    struct Node {
        Node * prev = this, * next = this;
        int * item;

        Node(): item(nullptr) {}

        explicit Node(int item): item(new int(item)) {}

        ~Node() {
            delete this->item;
        }
    };

    class iterator {
    private:
        Node * ptr;

    public:
        explicit iterator(Node * other_ptr = nullptr): ptr(other_ptr) {}

        bool inline operator == (const iterator& other) const {
            return this->ptr->item == other.ptr->item;
        }

        bool inline operator != (const iterator& other) const {
            return !(*this == other);
        }

        int inline operator * () const {
            return *(this->ptr->item);
        }

        iterator& operator++ () {
            this->ptr = this->ptr->next;
            return *this;
        }

        iterator& operator-- () {
            this->ptr = this->ptr->prev;
            return *this;
        }
    };

private:
    size_t actual_size;
    Node * after_last;

public:
    List(): actual_size(0), after_last(new Node()) {}

    ~List() {
        while (actual_size) pop_front();
        delete after_last;
    }

    [[nodiscard]] inline iterator begin() const {
        return iterator(after_last->next);
    }

    [[nodiscard]] inline iterator end() const {
        return iterator(after_last);
    }

    [[nodiscard]] inline size_t size() const {
        return actual_size;
    }

    inline void push_back(int elem) {
        ++actual_size;
        Node * to_add = new Node(elem), * real_last = after_last->prev;
        to_add->prev = real_last;
        to_add->next = after_last;
        real_last->next = to_add;
        after_last->prev = to_add;
    }

    inline void pop_back() {
        --actual_size;
        Node * real_last = after_last->prev;
        after_last->prev = real_last->prev;
        after_last->prev->next = after_last;
        delete real_last;
    }

    inline void push_front(int elem) {
        ++actual_size;
        Node * to_add = new Node(elem), * real_first = after_last->next;
        to_add->next = real_first;
        to_add->prev = after_last;
        after_last->next = to_add;
        real_first->prev = to_add;
    }

    inline void pop_front() {
        --actual_size;
        Node * real_first = after_last->next;
        after_last->next = real_first->next;
        after_last->next->prev = after_last;
        delete real_first;
    }
};
