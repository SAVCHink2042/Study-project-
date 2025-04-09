/*         УСЛОВИЕ:    Задача: Дерево выражения. Требуется реализовать набор классов для представления дерева
выражения. В таком дереве операции служат промежуточными узлами, а числа находятся в
листьях дерева.  Аргументами для операций служат значения в дочерних узлах.


 			Реализовать считывание формулы без скобок из строки и создание соответствующей систе-
мы классов.

			
 Реализовать программу, которая считывает арифметическое выражение без скобок,
  выводит результат вычисления выражения на экран.

*/


#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <bits/stdc++.h>
  
int prec(char c) {
    if (c == '#') {
        return 3;
    } else if (c == '/' || c == '*')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else
        return -1;
}
 
 std::string infixToPostfix(const std::string& s) {           // преобразование в польскую запись
    std::stack<char> st;
    std::string result;
 		for (char c : s) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            result += c;
        } else {
            while (!st.empty() && prec(c) <= prec(st.top())) {
                result += st.top();
                st.pop();
            }
            st.push(c);
        }
    }
 		while (!st.empty()) {
        result += st.top();
        st.pop();
    }
 		return result;
}
 
class Node {
 public:
    virtual double Evaluate() const = 0;
    virtual ~Node() {}
};
 
class Value : public Node {
 private:
    double value_;
 public:
    explicit Value(double value) : value_(value) {
   }
 
    double Evaluate() const override {
        return value_;
  }
};
 
class BinaryOperation : public Node {
 protected:
    Node* left_, * right_;
 public:
    BinaryOperation(Node* left, Node* right) : left_(left), right_(right) {
   }
};
 
class UnaryOperation : public Node {
 protected:
    Node* argument_;
 public:
    UnaryOperation(Node* argument) : argument_(argument) {
   }
};
 
class Addition : public BinaryOperation {
 public:
    Addition(Node* left, Node* right) : BinaryOperation(left, right) {
      }
 
    double Evaluate() const override {
        double tmp = left_->Evaluate() + right_->Evaluate();
        return tmp;
  }
};
 
class Subtraction : public BinaryOperation {
 public:
    Subtraction(Node* left, Node* right) : BinaryOperation(left, right) {
   }
 
    double Evaluate() const override {
        return right_->Evaluate() - left_->Evaluate();
  }
};
 
class Multiplication : public BinaryOperation {
 public:
    Multiplication(Node* left, Node* right) : BinaryOperation(left, right) {
    }
 
    double Evaluate() const override {
        return left_->Evaluate() * right_->Evaluate();
  }
};
 
class Division : public BinaryOperation {
 public:
    Division(Node* left, Node* right) : BinaryOperation(left, right) {
    }
 
    double Evaluate() const override {
        return right_->Evaluate() / left_->Evaluate();
    }
};
 
class Negation : public UnaryOperation {
 public:
    Negation(Node* argument) : UnaryOperation(argument) {
    }
 
    double Evaluate() const override {
        return -argument_->Evaluate();
    }
};
 
Node* constructTree(Node* tree,   const std::string& postfix, const std::vector<double>& values) {                                  //выстраивание дерева по польской записи иходного выражения
    std::stack<Node*> st;
    Node* temp_tree1, *temp_tree2;
    unsigned long int i , j = 0;
    for ( i = 0; i < postfix.length(); i++) {
      if (!(postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/' || postfix[i] == '#')) {
         tree = new Value(values[j]);
         j++;
        st.push(tree);
     } else {
         if (postfix[i] == '#') {
             temp_tree1 = st.top();
             st.pop();
              tree = new Negation(temp_tree1);
             st.push(tree);
             continue;
            }
          temp_tree1 = st.top();
         st.pop();
          temp_tree2 = st.top();
          st.pop();
 
          switch (postfix[i]) {
             case '+':tree = new Addition(temp_tree1, temp_tree2);
                break;
             case '-':tree = new Subtraction(temp_tree1, temp_tree2);
                 break;
             case '*':tree = new Multiplication(temp_tree1, temp_tree2);
                 break;
             case '/':tree = new Division(temp_tree1, temp_tree2);
                break;
             default:break;
 
          }
            st.push(tree);
     }
    }
    return tree;
}
 
Node* FromString(const char* formula) {  
		unsigned long int   i;   
    std::string exp = formula;
    exp.erase(remove_if(exp.begin(), exp.end(), isspace), exp.end()); 
    std::stringstream new_exp;
    std::string value;
    std::vector<double> values;
    char c;
    for ( i = 0; i < exp.length(); ++i) {
       c = exp[i];
       if (c == '-' || c == '+' || c == '*' || c == '/') {
          if (!value.empty()) {
             new_exp << 'v' << c;
             try {
                 values.push_back(std::stod(value));
              } catch (const std::invalid_argument& exception) {
                throw std::invalid_argument("неправилный тип");
              }
              value = "";
        } else {
             if (c != '-') {
               throw std::invalid_argument("не унарный знак " + c);
             }
                new_exp << '#';
            }
 
        } else {
            value += c;
        }
        if (i == exp.length() - 1) {
            if (!value.empty()) {
                new_exp << 'v';
                values.push_back(std::stod(value));
            }
        }
    }
    if (values.empty()) {
        throw std::invalid_argument("нет значений");
    }
    std::string postfix = infixToPostfix(new_exp.str());
    return constructTree(nullptr, postfix, values);
}
 
int main() {
    std::string input;
    std::cin >> input;
    Node* result = FromString(input.c_str());
    std::cout << result->Evaluate() << std::endl;
 		return 0;
}
 
