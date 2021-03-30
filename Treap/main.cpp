#include <iostream>
#include <random>
#include <ctime>


using namespace std;

mt19937 gen(time(NULL));

const int NOT_FOUND = 2e9;

struct Node {
    int key;
    int prior;
    Node* lc = nullptr;
    Node* rc = nullptr;

    Node(int _key) {
        key = _key;
        prior = gen();
    }
};

using Node_ptr = Node*;

Node_ptr merge(Node_ptr l, Node_ptr r) {
    if (l == nullptr) return r;
    if (r == nullptr) return l;

    if (l->prior > r->prior) {
        l->rc = merge(l->rc, r);
        return l;
    } else {
        r->lc = merge(l, r->lc);
        return r;
    }
}

pair<Node_ptr, Node_ptr> split(Node_ptr root, int x) {
    if (root == nullptr) return {nullptr, nullptr};

    pair<Node_ptr, Node_ptr> p;

    if (root->key > x) {
        p = split(root->lc, x);
        root->lc = p.second;
        return {p.first, root};
    } else {
        p = split(root->rc, x);
        root->rc = p.first;
        return {root, p.second};
    }
}

int r_low_b(Node_ptr node, int x) {
    if (node == nullptr) return NOT_FOUND;

    if (node->key <= x) {
        return r_low_b(node->rc, x);
    } else {
        int buf = r_low_b(node->lc, x);
        return buf == NOT_FOUND ? node->key : buf;
    }
}

int l_low_b(Node_ptr node, int x) {
    if (node == nullptr) return NOT_FOUND;

    if (node->key >= x) {
        return l_low_b(node->lc, x);
    } else {
        int buf = l_low_b(node->rc, x);
        return buf == NOT_FOUND ? node->key : buf;
    }
}

inline bool exists(Node_ptr &treap, int x) {
    Node_ptr node = treap;

    while (node != nullptr && node->key != x) {
        if (node->key > x) {
            node = node->lc;
        } else {
            node = node->rc;
        }
    }

    return node != nullptr;
}

inline void add(Node_ptr &treap, int x) {
    if (treap == nullptr) {
        treap = new Node(x);
        return;
    }

    if (exists(treap, x)) return;

    pair<Node_ptr, Node_ptr> p = split(treap, x);
    Node_ptr q = new Node(x);
    p.first = merge(p.first, q);
    treap = merge(p.first, p.second);
}

inline void del(Node_ptr &treap, int x) {
    if (treap == nullptr || !exists(treap, x)) return;

     Node_ptr node = treap;
     Node_ptr prev = nullptr;

     while (node->key != x) {
        prev = node;
        if (node->key > x) {
            node = node->lc;
        } else {
            node = node->rc;
        }
     }

     if (prev == nullptr) {
        treap = merge(treap->lc, treap->rc);
     } else if (prev->lc == node) {
        prev->lc = merge(node->lc, node->rc);
     } else {
        prev->rc = merge(node->lc, node->rc);
     }
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);


    string req;
    int x;

    Node_ptr treap = nullptr;

    while (cin >> req) {
        cin >> x;

        if (req == "insert") {
            add(treap, x);
        } else if (req == "delete") {
            del(treap, x);
        } else if (req == "exists") {
            if (exists(treap, x)) {
                cout << "true\n";
            } else {
                cout << "false\n";
            }
        } else if (req == "next") {
            x = r_low_b(treap, x);
            if (x == NOT_FOUND) {
                cout << "none\n";
            } else {
                cout << x << '\n';
            }
        } else {  // req == "prev"
            x = l_low_b(treap, x);
            if (x == NOT_FOUND) {
                cout << "none\n";
            } else {
                cout << x << '\n';
            }
        }
    }


    return 0;
}