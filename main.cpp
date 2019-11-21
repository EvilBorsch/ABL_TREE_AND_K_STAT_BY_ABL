#include <iostream>


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

    void req_print(node *nod) {
        if (nod == nullptr)
            return;
        req_print(nod->left);
        std::cout << nod->key << " ";
        req_print(nod->right);
    }

    void rotateright(node *&p) {

        node *q = p->left;
        int A_num = num_child(q->left);
        int B_num = num_child(q->right);
        int C_num = num_child(p->right);
        int p_num_childs = num_child(p);
        int q_num_childs = num_child(q);


        p->left = q->right;
        q->right = p;
        fixheight(p);
        fixheight(q);

        p = q;
        p->num_of_childs = p_num_childs;
        if (p->right != nullptr) p->right->num_of_childs = q_num_childs - 1;
        if (p->left != nullptr) p->left->num_of_childs = A_num;
        if (p->right->right != nullptr) p->right->right->num_of_childs = C_num;
        if (p->right->left != nullptr) p->right->left->num_of_childs = B_num;

    }


    void rotateleft(node *&q) {

        node *p = q->right;
        int A_num = num_child(q->left);
        int B_num = num_child(p->left);
        int C_num = num_child(p->right);
        int p_num_childs = num_child(p);
        int q_num_childs = num_child(q);


        q->right = p->left;
        p->left = q;
        fixheight(q);
        fixheight(p);
        q = p;

        q->num_of_childs = q_num_childs;
        if (q->left != nullptr) q->left->num_of_childs = p_num_childs - 1;
        if (q->right != nullptr) q->right->num_of_childs = C_num;
        if (q->left->left != nullptr) q->left->left->num_of_childs = A_num;
        if (q->left->right != nullptr) q->left->right->num_of_childs = B_num;


    }


    node *removemin(node *p, node *&temp_data) // удаление узла с минимальным ключом из дерева p
    {
        temp_data = p;
        while (temp_data->left != nullptr) {
            temp_data = temp_data->left;
        }
        if (p->left == 0)
            return p->right;
        p->left = removemin(p->left, temp_data);

        p->num_of_childs--;
        balance(p);
        return p;
    }

    node *remove(node *p, int k) // удаление ключа k из дерева p
    {

        if (k < p->key) {
            p->num_of_childs--;
            p->left = remove(p->left, k);
        } else if (k > p->key) {
            p->num_of_childs--;
            p->right = remove(p->right, k);
        } else //  k == p->key
        {
            node *l = p->left;
            node *r = p->right;
            int num_childs = p->num_of_childs;
            node *min;
            node *test;
            //delete p;
            if (l != nullptr && r != nullptr && r->height < l->height) {
                test = removemin(r, min);
                min->right = test;
                min->left = l;
                min->num_of_childs = num_childs - 1;
                balance(min);
                return min;
            } else {

                if (!r) return l;
                if (!l) return r;
                test = removemin(l, min);
                test->left = min;
                test->right = r;
                test->num_of_childs = num_childs - 1;
                balance(test);
                return test;
            }

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

    void print() {
        req_print(data);
    }

    void del(const T &k) {
        data = remove(data, k);
    }

    T findkstat(int key) {
        node *cur = data;
        int kstat;
        int lol=cur->num_of_childs;
        if (cur->left == nullptr) kstat = 0;
        else kstat = cur->left->num_of_childs + 1;
        int delta;
        while (key != cur->key) {

            if (key > cur->key) {
                if (cur->right->left == nullptr) {
                    kstat++;
                } else kstat += cur->right->left->num_of_childs + 2;
                cur = cur->right;
            }

            if (key < cur->key) {
                cur = cur->left;
                if (cur == nullptr) kstat--;
                else {
                    if (cur->right == nullptr) {
                        kstat--;
                    }
                    else kstat -= (cur->num_of_childs - cur->right->num_of_childs);
                }

            }
            if (cur == nullptr) {
                return kstat;
            }


        }
        return kstat;
    }


};


class Comparator {
public:
    bool operator()(int a, int b) {
        return a > b;
    }

};


void test() {
    //int j=5;
    for (int j = 2; j < 100; j++) {
        Comparator cmp;
        Tree<int, Comparator> tr(cmp);
        for (int i = 0; i < j; i++) {
            tr.add(i + 1);
        }

        tr.del(1);
        for (int i = 1; i < j; i++) {
            std::cout<< tr.findkstat(i+1) << std::endl;
            //assert(tr.findkstat(i + 1) == i);
        }
        std::cout<< "///////////tr: ///////" << j << std::endl;
    }


}

int main() {


    test();
    /*
    Comparator cmp;
    Tree<int, Comparator> tr(cmp);
    int operation;
    int dig;
    int N;
    std::cin >> N;
    for (int i = 0; i < N; i++) {
        std::cin >> operation;
        std::cin >> dig;
        if (operation == 1) {
            tr.add(dig);
            cout << tr.findkstat(dig);
        } else if (operation == 2) {
            tr.del(dig);
            cout << tr.findkstat(dig);
        } else break;

    }
     */
}


