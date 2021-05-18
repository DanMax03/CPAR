#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

//#include <cassert>


using namespace std;


using lli = long long int;
using ld = long double;


struct Node {
    Node* par = nullptr;
    char path = '0';

    unordered_map<char, Node*> next;

    Node* suf_link = nullptr;
    Node* term_link = nullptr;

    vector<int> indexes;
    bool is_terminal = false;
};

inline void add_word(int &t, Node* node, string &s) {
    for (char &c : s) {
        if (node->next[c] == nullptr) {
            node->next[c] = new Node;
            node->next[c]->par = node;
            node->next[c]->path = c;
        }

        node = node->next[c];
    }

    node->indexes.push_back(t++);
    node->is_terminal = true;
}


inline Node* jump(Node* node, char c) {
    while (node->par != nullptr && node->next.find(c) == node->next.end()) {
        node = node->suf_link;
    }

    if (node->next.find(c) == node->next.end()) {
        return node;
    } else {
        return node->next[c];
    }
}

inline void build_links(Node* root) {
    root->suf_link = root;

    queue<Node*> bfs;

    for (char i = 'a'; i <= 'z'; ++i) {
        if (root->next.find(i) == root->next.end()) continue;
        bfs.push(root->next[i]);
    }

    Node* cur;
    Node* buf;

    while (!bfs.empty()) {
        cur = bfs.front();
        bfs.pop();

        cur->suf_link = jump(cur->par->suf_link, cur->path);
        if (cur->suf_link == cur) {
            cur->suf_link = root;
        }

        buf = cur;

        while (buf != root && !buf->is_terminal) {
            buf = buf->suf_link;
        }

        if (buf->is_terminal) {
            cur->term_link = buf;
        }

        for (char i = 'a'; i <= 'z'; ++i) {
            if (cur->next.find(i) == cur->next.end()) continue;
            bfs.push(cur->next[i]);
        }
    }
}

inline void mark_all_terminal_suffixes(Node* node, int &cur_pos, vector<vector<int>> &a) {
    node = node->term_link;
    while (node != nullptr) {
        for (int &i : node->indexes) {
            a[i].push_back(cur_pos + 1);
        }
        node = node->suf_link->term_link;
    }
}


//inline Node* read_str(Node* node, string &s) {
//    int i = 0;
//    while (i < s.size() && node->next.find(s[i]) != node->next.end()) {
//        node = node->next[s[i]];
//        ++i;
//    }
//
//    if (i != s.size()) return nullptr;
//    return node;
//}
//
//inline void check_links(Node* root) {
//    queue<Node*> bfs;
//    bfs.push(root);
//    Node* cur;
//    Node* buf;
//    string s;
//
//    while (!bfs.empty()) {
//        cur = bfs.front();
//        bfs.pop();
//
//
//        s = "";
//        buf = cur;
//        while (buf != root) {
//            s += buf->path;
//            buf = buf->par;
//        }
//
//        if (!s.empty()) {
//            s.pop_back();
//            reverse(s.begin(), s.end());
//        }
//
//        buf = read_str(root, s);
//        while (buf == nullptr) {
//            s = s.substr(1);
//            buf = read_str(root, s);
//        }
//
//        assert(cur->suf_link == buf);
//
//
//        for (char i = 'a'; i <= 'z'; ++i) {
//            if (cur->next.find(i) == cur->next.end()) continue;
//            bfs.push(cur->next[i]);
//        }
//    }
//}


int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);


    int n;
    string t;

    cin >> t >> n;

    string s;
    Node trie;
    int timer = 0;
    vector<int> lengths(n);

    for (int i = 0; i < n; ++i) {
        cin >> s;
        lengths[i] = s.size();
        add_word(timer, &trie, s);
    }

    build_links(&trie);


//  check_links(&trie);


    Node* node = &trie;
    int m = t.size();
    vector<vector<int>> ans(n);

    for (int i = 0; i < m; ++i) {
        node = jump(node, t[i]);

        mark_all_terminal_suffixes(node, i, ans);
    }


    for (int i = 0; i < n; ++i) {
        cout << ans[i].size() << ' ';
        for (int &e : ans[i]) {
            cout << e - lengths[i] + 1 << ' ';
        }
        cout << '\n';
    }


    return 0;
}
