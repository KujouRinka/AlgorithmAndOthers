#include <iostream>
#include <vector>
#include <string>
#include <memory>

template<typename>
class Blob;
template<typename>
class BlobPtr;

template<typename T>
bool operator==(const Blob<T> &lhs, const Blob<T> &rhs);

template<typename T>
class Blob {
    friend class BlobPtr<T>;
    friend bool operator==<T>(const Blob<T> &lhs, const Blob<T> &rhs);
public:
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    Blob();
    Blob(std::initializer_list<T> il);
    template<typename It>
    Blob(It b, It e);

    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    void push_back(const T &t) { data->push_back(t); }
    void push_back(T &&t) { data->push_back(std::move(t)); }
    void pop_back();
    T &back();
    T &operator[](size_type i);

private:
    std::shared_ptr<std::vector<T>> data;
    // if invalid data[i], then throw msg.
    void check(size_type i, const std::string &msg) const;
};

template<typename T>
class BlobPtr {
    BlobPtr() : wptr(nullptr), curr(0) {}
    BlobPtr(Blob<T> &a, std::size_t sz = 0) :
            wptr(a), curr(sz) {}

    T &operator*() const {
        auto p = check(curr, "dereference past end");
        return (*p)[curr];
    }
    BlobPtr &operator++();
    BlobPtr operator++(int);
    BlobPtr &operator--();
    BlobPtr operator--(int);

private:
    std::shared_ptr<std::vector<T>>
    check(std::size_t i, const std::string &msg) const;

    std::weak_ptr<std::vector<T>> wptr;
    std::size_t curr;
};

template<typename T>
Blob<T>::Blob() : data(std::make_shared<std::vector<T>>()) {}

template<typename T>
Blob<T>::Blob(std::initializer_list<T> il) :
        data(std::make_shared<std::vector<T>>(il)) {}

template<typename T>
template<typename It>
Blob<T>::Blob(It b, It e) :
        data(std::make_shared<std::vector<T>>(b, e)) {}

template<typename T>
void Blob<T>::pop_back() {
    check(0, "pop back on empty Blob");
    data->pop_back();
}

template<typename T>
T &Blob<T>::back() {
    check(0, "back on empty Blob");
    return data->back();
}

template<typename T>
T &Blob<T>::operator[](size_type i) {
    return (*data)[i];
}

template<typename T>
void Blob<T>::check(size_type i, const std::string &msg) const {
    if (i >= data->size())
        throw std::out_of_range(msg);
}

template<typename T>
BlobPtr<T> &BlobPtr<T>::operator++() {
    check(++curr, "pointer move past end");
    return *this;
}

template<typename T>
BlobPtr<T> BlobPtr<T>::operator++(int) {
    auto ret = *this;
    ++*this;
    return ret;
}

template<typename T>
BlobPtr<T> &BlobPtr<T>::operator--() {
    check(--curr, "pointer move before end");
    return *this;

}

template<typename T>
BlobPtr<T> BlobPtr<T>::operator--(int) {
    auto ret = *this;
    --*this;
    return ret;
}

template<typename T>
std::shared_ptr<std::vector<T>>
BlobPtr<T>::check(std::size_t i, const std::string &msg) const {
    if (wptr.expired())
        throw std::runtime_error("unbound BlobPtr");
    if (i >= wptr->size() || i < 0)
        throw std::out_of_range(msg);
}


template<typename T>
bool operator==(const Blob<T> &lhs, const Blob<T> &rhs) {
    return *lhs.data == *rhs.data;
}

int main() {
    Blob<std::string> sBlob = {"Hello", "world", "!"};
    Blob<int> emptyBlob;
    emptyBlob.back();
    sBlob.push_back("!!");
    std::cout << sBlob.back();
    std::cout << sBlob.size();
    return 0;
}
