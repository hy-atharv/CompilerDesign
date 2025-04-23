#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Structure to represent a three-address code instruction
struct Instruction {
    string code; // e.g., "goto Lx" or "if t1 goto Lx"
    int target;  // Target label index (-1 if unresolved)
};

// Backpatching class to manage instruction list and backpatching
class Backpatch {
private:
    vector<Instruction> code; // List of generated instructions
    vector<int> makelist(int index) { return {index}; } // Create a list with a single instruction index
    vector<int> merge(const vector<int>& l1, const vector<int>& l2); // Merge two lists
    void backpatch(const vector<int>& list, int target); // Patch jump targets

public:
    int nextInstr = 0; // Next instruction index
    void emit(const string& instruction, int target = -1); // Emit a new instruction
    void printCode(); // Print all generated code
    vector<int> handleIf(const string& condition, vector<int> trueList, vector<int> falseList, int label);
};

// Merge two lists of instruction indices
vector<int> Backpatch::merge(const vector<int>& l1, const vector<int>& l2) {
    vector<int> result = l1;
    result.insert(result.end(), l2.begin(), l2.end());
    return result;
}

// Backpatch: Update all instructions in 'list' with the target label
void Backpatch::backpatch(const vector<int>& list, int target) {
    for (int index : list) {
        if (index < code.size()) {
            code[index].target = target;
            if (code[index].code.find("goto") != string::npos && code[index].code.find("if") == string::npos) {
                code[index].code = "goto L" + to_string(target);
            } else {
                code[index].code.replace(code[index].code.find("L?"), 2, "L" + to_string(target));
            }
        }
    }
}

// Emit a new instruction
void Backpatch::emit(const string& instruction, int target) {
    code.push_back({instruction, target});
    nextInstr++;
}

// Print the generated code
void Backpatch::printCode() {
    for (int i = 0; i < code.size(); i++) {
        cout << i << ": " << code[i].code;
        if (code[i].target != -1) {
            cout << " (target: L" << code[i].target << ")";
        }
        cout << endl;
    }
}

// Handle if-then-else construct
vector<int> Backpatch::handleIf(const string& condition, vector<int> trueList, vector<int> falseList, int label) {
    // Emit conditional jump for the condition
    emit("if " + condition + " goto L?", -1);
    vector<int> condList = makelist(nextInstr - 1);

    // Backpatch true and false lists
    backpatch(trueList, label);
    backpatch(falseList, label + 1);

    // Return the list of conditional jumps to be patched later
    return condList;
}

int main() {
    Backpatch bp;
    string condition;

    // Get condition from user
    cout << "Enter the condition for the if statement (e.g., 'x > y'): ";
    getline(cin, condition);

    // Get starting label from user (optional, default to 100)
    int label;
    cout << "Enter the starting label number (e.g., 100): ";
    cin >> label;

    // Simulate true and false branch jumps
    vector<int> trueList = bp.makelist(bp.nextInstr);
    bp.emit("goto L?", -1); // Placeholder for true branch jump
    vector<int> falseList = bp.makelist(bp.nextInstr);
    bp.emit("goto L?", -1); // Placeholder for false branch jump

    // Handle the if construct
    vector<int> condList = bp.handleIf(condition, trueList, falseList, label);

    // Simulate some code for true and false branches
    bp.emit("L" + to_string(label) + ": t1 = x + y"); // True branch code
    bp.emit("L" + to_string(label + 1) + ": t2 = x - y"); // False branch code

    // Backpatch conditional jumps
    bp.backpatch(condList, label);

    // Print the generated code
    cout << "\nGenerated Three-Address Code:\n";
    bp.printCode();

    return 0;
}