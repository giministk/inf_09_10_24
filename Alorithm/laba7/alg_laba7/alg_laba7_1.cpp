#include <iostream>
#include <locale>

using namespace std;

class TreeNode {
public:
    int val;            // Значение узла
    TreeNode* left;     // Указатель на левый дочерний узел
    TreeNode* right;    // Указатель на правый дочерний узел

    // Конструктор класса
    // Принимает значение узла и указатели на левый и правый узлы (по умолчанию nullptr, что аналог None)
    TreeNode(int val = 0, TreeNode* left = nullptr, TreeNode* right = nullptr)
        : val(val), left(left), right(right) {} // Инициализация членов класса
};

int findMin(TreeNode* root)
{
    // корень с максимальным значением по дефолту 
    if (root == NULL)
        return INT_MIN;

    //рассматриваем 3 величины: конень, максимальное в левом поддереве, максимальное в правом поддереве
    int res = root->val;
    int left_res = findMin(root->left);
    int right_res = findMin(root->right);
    if (left_res > res)
        res = left_res;
    if (right_res > res)
        res = right_res;
    return res;
}

int main() {
    setlocale(LC_ALL, "Russian");

    //создаём дерево
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(3);
    root->right = new TreeNode(5);
    root->left->right = new TreeNode(10);
    root->left->right->right = new TreeNode(3);
    root->left->left = new TreeNode(8);
    root->left->left->left = new TreeNode(14);
    root->left->left->right = new TreeNode(15);
    root->right->left = new TreeNode(2);
    root->right->right = new TreeNode(6);
    root->right->right->left = new TreeNode(0);
    root->right->right->right = new TreeNode(1);


    std::cout << "Самая тусклая лампочка со значением " << findMin(root) << endl; //должно вывести 0
    return 0;
};