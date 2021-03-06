#include <iostream>
#include <string>

using namespace std;

class HasPtr {

public:
    HasPtr(const string &s = string()) :
            ps(new string(s)), i(0), use(new size_t(1)) {}

    HasPtr(const HasPtr &p) :
            ps(p.ps), i(p.i), use(p.use) { ++*use; }

    HasPtr &operator=(const HasPtr &rhs);

    HasPtr &operator=(const string &s);

    string &operator*();

    bool unique();

    string *get();

    ~HasPtr();

private:
    string *ps;
    int i;
    size_t *use;
};

HasPtr &HasPtr::operator=(const HasPtr &rhs) {
    ++*rhs.use;
    if (--*use == 0) {
        delete ps;
        delete use;
    }
    ps = rhs.ps;
    use = rhs.use;
    i = rhs.i;
    return *this;
}

HasPtr & HasPtr::operator=(const string &s) {
    auto newp = new string(s);
    if (--*use == 0) {
        delete ps;
        delete use;
    }
    ps = newp;
    use = new size_t(1);
    i = 0;
    return *this;
}

string &HasPtr::operator*() {
    return *ps;
}

bool HasPtr::unique() {
    return *use == 1;
}

string *HasPtr::get() {
    return ps;
}

HasPtr::~HasPtr() {
    if (--*use == 0) {
        delete ps;
        delete use;
    }
}

int main() {
    HasPtr h("hello");
    cout << "h unique: " << h.unique() << endl;
    HasPtr h2 = h;
    cout << "h unique: " << h.unique() << endl;
    h = "world";

    auto sp = h2.get();

    cout << "h: " << *h << " unique: " << h.unique() << endl;
    cout << "h2: " << *sp << " unique: " <<h2.unique() << endl;

    cout << "h: " << *h << " unique: " << h.unique() << endl;
    cout << "h2: " << *h2 << " unique: " <<h2.unique() << endl;

    return 0;
}
