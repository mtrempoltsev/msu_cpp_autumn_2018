#include <iostream>

enum operation {
    add,
    sub,
    mul,
    divide,
    none,
};

struct expression_tree {
    operation op;
    int64_t number;
    expression_tree *left;
    expression_tree *right;
};

void exit_with_error() {  // function to shorten emergency exits
    std::cout << "error" << std::endl;
    exit(1);
}

void skip_spaces(const char *ar, int &i) {
    while (ar[i] == ' ') {
        i++;
    }
}

// read single number from ar, and process sequences of "-",
// and set i = -1 if reached end of ar
int64_t read_number(const char *ar, int &i) {
    int sign = 1;
    std::string number;
    number = "";
    skip_spaces(ar, i);
    while (ar[i] == '-') { // process "-"
        sign *= -1;
        i++;
    }
    skip_spaces(ar, i);
    while ((ar[i] >= '0') && (ar[i] <= '9')) { //read number itself
        number += ar[i];
        i++;
    }
    if (!number.compare("")) { // break program if empty number
        exit_with_error();
    }
    skip_spaces(ar, i);
    if (ar[i] == '\0') { // set i = -1
        i = -1;
    }
    return (std::stoi(number) * sign); // return number with correct sign
}

operation read_operation(const char *ar, int &i) {
    skip_spaces(ar, i);

    switch (ar[i]) { // read opeartion
        case '+':
            i++;
            return add;
        case '-':
            i++;
            return sub;
        case '*':
            i++;
            return mul;
        case '/':
            i++;
            return divide;
        default: // break program if invalid operation
            exit_with_error();
    }
}


void add_number_to_tree(expression_tree **tree, int64_t number) {  // add number to bottom of the rightmost branch
    if ((*tree) == NULL) {
        (*tree) = new expression_tree;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        (*tree)->number = number;
        (*tree)->op = none;
    } else {
        add_number_to_tree(&((*tree)->right), number);
    }
}

void add_operation_to_tree(expression_tree **tree, operation operation) {
    switch (operation) {
        case add:
        case sub: {  // add and sub must be on top of expression tree
            expression_tree *buf = new expression_tree;
            buf->left = *tree;
            buf->right = NULL;
            buf->op = operation;
            buf->number = 0;
            *tree = buf;
            break;
        }
        case mul:
        case divide: {
            switch ((*tree)->op) {
                case add:
                case sub: {  // add and sub must be higher in tree then mul and div
                    add_operation_to_tree(&((*tree)->right), operation);
                    break;
                }
                case mul:
                case divide:
                case none: {
                    expression_tree *buf = new expression_tree;
                    buf->left = *tree;
                    buf->right = NULL;
                    buf->op = operation;
                    buf->number = 0;
                    *tree = buf;
                    break;
                }
            }
        }
    }
}

expression_tree *parse_expression(const char *ar) {
    int i = 0;
    int64_t num;
    operation op;
    expression_tree *tree = NULL;
    while (i != -1) {
        num = read_number(ar, i);
        add_number_to_tree(&tree, num);
        if (i != -1) {
            op = read_operation(ar, i);
            add_operation_to_tree(&tree, op);
        }
    }
    return tree;
}

int64_t count_expression(expression_tree *tree) {
    switch (tree->op) {
        case none:
            return tree->number;
        case add:
            return count_expression(tree->left) + count_expression(tree->right);
        case sub:
            return count_expression(tree->left) - count_expression(tree->right);
        case mul:
            return count_expression(tree->left) * count_expression(tree->right);
        case divide: {
            int64_t right = count_expression(tree->right);
            if (right != 0) {
                return count_expression(tree->left) / right;
            } else {
                exit_with_error();
            }
        }
    }
}

void del_expression_tree(expression_tree **tree) {
    if ((*tree)->left != NULL) {
        del_expression_tree(&((*tree)->left));
    }
    if ((*tree)->right != NULL) {
        del_expression_tree(&((*tree)->right));
    }
    delete((*tree));
}

int main(int argc, const char *argv[]) {
    expression_tree *tree = NULL;
    int64_t result;
    std::string str;
    if (argc != 2) {
        exit_with_error();
    }
    tree = parse_expression(argv[1]);
    result = count_expression(tree);
    std::cout << result;
    del_expression_tree(&tree);
    return 0;
}