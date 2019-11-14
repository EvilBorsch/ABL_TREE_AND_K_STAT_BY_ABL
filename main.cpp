#include <iostream>
template <class T, class Cmp>
class Tree{
private:

    struct node{
        T key;
        node* left= nullptr;
        node* right= nullptr;
        int height=1;
        int num_of_childs=0;
        node(T data){
            key=data;

        }

    };

    int height(node* p)
    {
        return p?p->height:0;
    }

    int num_child(node* p){
        return p?p->num_of_childs:0;
    }

    void fixheight(node*& p)
    {
        int hl = height(p->left);
        int hr = height(p->right);
        p->height = (hl>hr?hl:hr)+1;
    }

    int bfactor(node* p)
    {
        return height(p->right)-height(p->left);
    }

    Cmp cmp;



    void balance(node*& p){

        fixheight(p);

        if( bfactor(p)==-2 )
        {
            //if( bfactor(p->left) > 0  )
            //p->left = rotateleft(p->left);
            rotateright(p);
        } //

    }

    void req_add(node*& nod,const T& key){
        if (nod== nullptr){
            nod=new node(key);
            return;
        }
        if (cmp(key,nod->key)){
            nod->num_of_childs+=1;
            req_add(nod->right,key);
        }
        else {
            nod->num_of_childs+=1;
            req_add(nod->left, key);
        }
        balance(nod);
    }

    void req_print(node* nod){
        if (nod== nullptr)
            return;
        req_print(nod->left);
        std::cout<< nod->key;
        req_print(nod->right);
    }




public:
    void rotateright(node*& p) // правый поворот вокруг p
    {

        node* q = p->left;
        int A_num=num_child(q->left);
        int B_num=num_child(q->right);
        int C_num=num_child(p->right);
        int p_num_childs=num_child(p);
        int q_num_childs=num_child(q);


        p->left = q->right;
        q->right = p;
        fixheight(p);
        fixheight(q);

        p=q;
        p->num_of_childs=p_num_childs;
        p->right->num_of_childs=B_num+C_num+2;
        p->left->num_of_childs=A_num;

        if (p->right->right!= nullptr)    p->right->right->num_of_childs=C_num;
        if (p->right->left!= nullptr)     p->right->left->num_of_childs=B_num;


    }

    node* data= nullptr;

    Tree(Cmp m_cmp){
        cmp=m_cmp;
    }

    void add(const T& key){
        req_add(data,key);
    }

    void print(){
        req_print(data);
    }






};


class Comparator{
public:
    bool operator () (int a, int b){
        return a>b;
    }

};

int main(){
    Comparator cmp;
    Tree<int,Comparator> tr(cmp);
    tr.add(7);
    tr.add(4);
    tr.add(8);
    tr.add(3);
    tr.add(5);
    tr.add(6);



    tr.print();



}