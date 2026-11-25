#include "../include/headers.h"

class Optimizer{
    private:
    vector<string> optimizedInstruction;

    public:
    string constantFolding(string instruction, int equalIndex, int operatorIndex);

    void optimize(vector<string> instructions);
};