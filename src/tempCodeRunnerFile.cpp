r(int i = 0; i < tokens.size(); i++){
        cout << "Type: " << static_cast<int>(tokens[i].type) << endl;
        cout << "Value: " << tokens[i].value << endl;
        cout << "Line: " << tokens[i].line << endl;
    }