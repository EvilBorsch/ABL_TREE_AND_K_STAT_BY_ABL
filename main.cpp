#include <iostream>
#include <sstream>
#include <cassert>
#include <queue>

using namespace std;

template<class T, class Cmp>
class Tree {
private:

    struct node {
        T key;
        node *left = nullptr;
        node *right = nullptr;
        int height = 1;
        int num_of_childs = 0;

        explicit node(T data) {
            key = data;
        }


    };

    int height(node *p) {
        return p ? p->height : 0;
    }

    int num_child(node *p) {
        return p ? p->num_of_childs : 0;
    }

    void fixheight(node *&p) {

        int hl = height(p->left);
        int hr = height(p->right);
        p->height = (hl > hr ? hl : hr) + 1;
    }

    int bfactor(node *p) {
        return height(p->right) - height(p->left);
    }

    Cmp cmp;


    void balance(node *&p) {


        fixheight(p);
        if (bfactor(p) == 2) {
            if (bfactor(p->right) < 0)
                rotateright(p->right);
            rotateleft(p);
        }
        if (bfactor(p) == -2) {
            if (bfactor(p->left) > 0)
                rotateleft(p->left);
            rotateright(p);
        }

    }

    void req_add(node *&nod, const T &key) {
        if (nod == nullptr) {
            nod = new node(key);
            return;
        }
        if (cmp(key, nod->key)) {
            nod->num_of_childs += 1;
            req_add(nod->right, key);
        } else {
            nod->num_of_childs += 1;
            req_add(nod->left, key);
        }
        balance(nod);
    }


    void rotateright(node *&p) {

        node *q = p->left;
        int A_num = num_child(q->left);
        int B_num = num_child(q->right);
        int C_num = num_child(p->right);
        int p_num_childs = num_child(p);


        p->left = q->right;
        q->right = p;
        fixheight(p);
        fixheight(q);

        p = q;
        p->num_of_childs = p_num_childs;
        if (p->left != nullptr) p->left->num_of_childs = A_num;
        if (p->right->right != nullptr) p->right->right->num_of_childs = C_num;
        if (p->right->left != nullptr) p->right->left->num_of_childs = B_num;

        if (p->right != nullptr) {
            p->right->num_of_childs = 0;
            if (p->right->left != nullptr) p->right->num_of_childs += p->right->left->num_of_childs + 1;
            if (p->right->right != nullptr) p->right->num_of_childs += p->right->right->num_of_childs + 1;
        }


    }


    void rotateleft(node *&q) {

        node *p = q->right;
        int A_num = num_child(q->left);
        int B_num = num_child(p->left);
        int C_num = num_child(p->right);
        int q_num_childs = num_child(q);


        q->right = p->left;
        p->left = q;
        fixheight(q);
        fixheight(p);
        q = p;

        if (q->right != nullptr) q->right->num_of_childs = C_num;
        if (q->left->left != nullptr) q->left->left->num_of_childs = A_num;
        if (q->left->right != nullptr) q->left->right->num_of_childs = B_num;
        q->num_of_childs = q_num_childs;
        if (q->left != nullptr) {
            q->left->num_of_childs = 0;
            if (q->left->left != nullptr) q->left->num_of_childs += q->left->left->num_of_childs + 1;
            if (q->left->right != nullptr) q->left->num_of_childs += q->left->right->num_of_childs + 1;


        }


    }


    node *removemin(node *p, node *&temp_data) {
        temp_data = p;
        while (temp_data->left != nullptr) {
            temp_data = temp_data->left;
        }
        if (p->left == nullptr)
            return p->right;
        p->left = removemin(p->left, temp_data);
        p->num_of_childs--;
        balance(p);
        return p;
    }

    node *removemax(node *p, node *&temp_data) {
        temp_data = p;
        while (temp_data->right != nullptr) {
            temp_data = temp_data->right;
        }
        if (p->right == nullptr)
            return p->left;
        p->right = removemax(p->right, temp_data);

        p->num_of_childs--;
        balance(p);
        return p;
    }

    node *remove(node *p, const int &k) {
        int sizeLeft;
        if (p->left == nullptr && p->right == nullptr) return nullptr;
        if (p->left == nullptr) {
            sizeLeft = 0;
        } else {
            sizeLeft = p->left->num_of_childs + 1;
            if (sizeLeft == 0) sizeLeft = 1;
        }

        if (sizeLeft == k) {
            node *l = p->left;
            node *r = p->right;
            int num_childs = p->num_of_childs;
            node *min;
            node *temp;
            if (l != nullptr && r != nullptr && r->height <= l->height) {
                temp = removemin(r, min);
                min->right = temp;
                min->left = l;
                min->num_of_childs = num_childs - 1;
                balance(min);
                return min;
            } else {
                if (!r) return l;
                if (!l) return r;
                temp = removemax(l, min);
                min->left = temp;
                min->right = r;
                min->num_of_childs = num_childs - 1;
                balance(min);
                return min;
            }

        }
        if (sizeLeft > k) {
            p->num_of_childs--;
            p->left = remove(p->left, k);
        } else {
            p->num_of_childs--;
            p->right = remove(p->right, k - sizeLeft - 1);
        }

        balance(p);
        return p;
    }


    node *data = nullptr;

public:


    explicit Tree(Cmp m_cmp) {
        cmp = m_cmp;
    }

    void add(const T &key) {
        req_add(data, key);
    }


    void del(const T &k) {
        data = remove(data, k);
    }

    T findkstat(const int &key) {
        node *cur = data;
        int kstat;
        if (cur->left == nullptr) kstat = 0;
        else kstat = cur->left->num_of_childs + 1;
        int delta = 0;
        while (key != cur->key) {

            if (cmp(key, cur->key)) {
                delta = kstat + 1;
                if (cur->right->left == nullptr) {
                    kstat++;
                } else kstat += cur->right->left->num_of_childs + 2;
                cur = cur->right;
                continue;
            } else {
                cur = cur->left;
                if (cur->left == nullptr && cur->right != nullptr) kstat -= 2;
                else if (cur->left == nullptr && cur->right == nullptr) kstat -= 1;
                else {
                    kstat = cur->left->num_of_childs + delta + 1;
                }
            }


        }
        return kstat;
    }

    ~Tree() {
        std::queue<node *> q;
        q.push(data->left);
        q.push(data->right);
        while (!q.empty()) {
            node *temp = q.front();
            if (temp != nullptr) {
                if (temp->left != nullptr) q.push(temp->left);
                if (temp->right != nullptr) q.push(temp->right);
            }
            delete temp;
            q.pop();
        }

        delete data;
    }

    Tree(const Tree &) = delete;

    Tree &operator=(const Tree &) = delete;




};


class Comparator {
public:
    bool operator()(int a, int b) {
        return a < b;
    }

};


void run(std::istream &in, std::ostream &out) {
    Comparator cmp;
    Tree<int, Comparator> tr(cmp);
    int operation;
    int dig;
    int N;
    in >> N;
    for (int i = 0; i < N; i++) {
        in >> operation;
        in >> dig;
        if (operation == 1) {
            tr.add(dig);
            out << tr.findkstat(dig) << std::endl;
        } else if (operation == 2) {
            tr.del(dig);
        }
    }
}


void temp() {


    {
        stringstream ss;
        ss << "5\n"
              "1 100\n"
              "1 200\n"
              "1 50\n"
              "2 1\n"
              "1 150";
        stringstream out;
        run(ss, out);
        assert(out.str() == "0\n"
                            "0\n"
                            "2\n"
                            "1\n");
    }


    {
        stringstream ss;
        ss << "5\n"
              "1 100\n"
              "1 50\n"
              "2 0\n"
              "1 45\n"
              "1 200";
        stringstream out;
        run(ss, out);
        assert(out.str() == "0\n"
                            "1\n"
                            "1\n"
                            "0\n");
    }

    {
        stringstream ss;
        ss << "6\n"
              "1 100\n"
              "1 200\n"
              "1 300\n"
              "1 400\n"
              "1 500\n"
              "1 600\n";
        stringstream out;
        run(ss, out);
        assert(out.str() == "0\n"
                            "0\n"
                            "0\n"
                            "0\n"
                            "0\n"
                            "0\n");
    }

    {
        stringstream ss;
        ss << "6\n"
              "1 100\n"
              "1 90\n"
              "1 80\n"
              "1 70\n"
              "1 60\n"
              "1 50\n";
        stringstream out;
        run(ss, out);
        assert(out.str() == "0\n"
                            "1\n"
                            "2\n"
                            "3\n"
                            "4\n"
                            "5\n");
    }


    {
        stringstream ss;
        ss << "6\n"
              "1 100\n"
              "2 100\n"
              "1 100\n"
              "2 100\n"
              "1 100\n"
              "2 100\n";
        stringstream out;
        run(ss, out);
        assert(out.str() == "0\n"
                            "0\n"
                            "0\n"
        );
    }


    {
        stringstream ss;
        ss << "12\n"
              "1 100\n"
              "1 200\n"
              "1 300\n"
              "1 400\n"
              "1 500\n"
              "1 600\n"
              "2 0\n"
              "2 0\n"
              "2 0\n"
              "2 0\n"
              "1 150\n"
              "1 250\n";
        stringstream out;
        run(ss, out);
        assert(out.str() == "0\n"
                            "0\n"
                            "0\n"
                            "0\n"
                            "0\n"
                            "0\n"
                            "1\n"
                            "0\n"
        );
    }

    {
        stringstream ss;
        ss << "12\n"
              "1 600\n"
              "1 500\n"
              "1 400\n"
              "1 300\n"
              "1 200\n"
              "1 100\n"
              "2 0\n"
              "2 0\n"
              "2 0\n"
              "2 0\n"
              "1 150\n"
              "1 250\n";
        stringstream out;
        run(ss, out);
        assert(out.str() == "0\n"
                            "1\n"
                            "2\n"
                            "3\n"
                            "4\n"
                            "5\n"
                            "1\n"
                            "0\n"
        );
    }

    {
        stringstream ss;
        ss << "12\n"
              "1 600\n"
              "1 400\n"
              "1 500\n"
              "1 200\n"
              "1 300\n"
              "1 100\n"
              "2 0\n"
              "2 0\n"
              "2 0\n"
              "2 0\n"
              "1 150\n"
              "1 250\n";
        stringstream out;
        run(ss, out);
        assert(out.str() == "0\n"
                            "1\n"
                            "1\n"
                            "3\n"
                            "3\n"
                            "5\n"
                            "1\n"
                            "0\n"
        );
    }


    {
        stringstream ss;
        ss << "12\n"
              "1 600\n"
              "1 400\n"
              "1 500\n"
              "2 1\n"
              "2 0\n"
              "2 0\n"
              "1 400\n"
              "1 500\n"
              "1 600\n"
              "2 1\n"
              "1 800\n"
              "1 10\n";
        stringstream out;
        run(ss, out);
        assert(out.str() == "0\n"
                            "1\n"
                            "1\n"
                            "0\n"
                            "0\n"
                            "0\n"
                            "0\n"
                            "3\n"
        );
    }

}


int main() {
    //test();
    run(std::cin, std::cout);


}


