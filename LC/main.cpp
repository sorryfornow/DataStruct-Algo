#include <iostream>
#include <vector>
#include <stack>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        if (!preorder.size() || !inorder.size()){
            return nullptr;
        }   // empty return

        typedef struct TreeNode* TNode;
        stack<TNode> treeStk;
        TNode root = new TreeNode(preorder[0]);
        treeStk.push(root);
        int indexOfInO = 0;  // cumulated by 1 during inorder list
        // initialization done here

        // traverse the preorder list
        for (int i = 1; i < preorder.size(); ++i) {
            int itemOfPre = preorder[i];
            TNode curNode = treeStk.top();  // the top item in stack

            if (curNode->val != inorder[indexOfInO]) {  // left node
                // malloc newNode as left of curNode
                curNode->left = new TreeNode(itemOfPre);
                treeStk.push(curNode->left);    // push newNode into stack
            } else{ // right node
                while (!treeStk.empty() && treeStk.top()->val == inorder[indexOfInO]) {
                    curNode = treeStk.top();    // catch the last popped node in stack
                    treeStk.pop();
                    indexOfInO++;
                }
                curNode->right = new TreeNode(itemOfPre);
                treeStk.push(curNode->right);
            }
        };
        return root;
        // Code buildTree end up here
    }
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    vector<int> preorder;
    vector<int> inorder;
    preorder = {3, 9, 8, 5, 4, 10, 20, 15, 7};
    inorder = {4, 5, 8, 10, 9, 3, 15, 20, 7};
    return 0;
}
