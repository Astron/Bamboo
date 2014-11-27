#include <iostream>
#include <string>
#include <vector>
#include <bamboo/module/Module.h>
#include <bamboo/module/Class.h>
#include <bamboo/module/Method.h>
#include <bamboo/dcfile/parser.h>
using namespace bamboo;
using namespace std;

string astron_keywords[] = {"clsend", "ownsend", "clrecv", "ownrecv",
                            "airecv", "broadcast", "ram", "required", "db"};


/* Helper function for printing complex types */
void print_type(Type *typ, int indent) {
    string tab(2 * indent, ' ');
    if(typ->subtype() == Subtype_Method) {
        Method *method = typ->as_method();
        for(unsigned int i = 0; i < method->num_params(); ++i) {
            Parameter *param = method->nth_param(i);
            cout << tab;
            if(!param->name().empty()) { cout << param->name() << " : "; }
            cout << param->type()->to_string() << '\n';
            print_type(param->type(), indent + 1);
        }
    } else if(typ->subtype() == Subtype_Struct) {
        Struct *cls = typ->as_struct();
        for(unsigned int i = 0; i < cls->num_fields(); ++i) {
            Field *field = cls->nth_field(i);
            cout << tab;
            if(!field->name().empty()) { cout << field->name() << " : "; }
            cout << field->type()->to_string() << '\n';
            print_type(field->type(), indent + 1);
        }
    }
}


// @NOTE(Kevin): Temporarily read the file and run the parser manually until
// the load_module method has been implemented in dcfile/parser.cpp
#include <stdio.h>
#include <bamboo/dcfile/lexer.h>

int main(void) {
    Module *mod = new Module();
    for(string keyword : astron_keywords) {
        mod->add_keyword(keyword);
    }

    // Read file into memory
    const size_t bufsize = 1024 * 4;
    char *buffer = (char *)malloc(bufsize);
    FILE *file = fopen("simple_example.dc", "r");
    size_t read = fread((void *)buffer, sizeof(char), bufsize - 1, file);
    buffer[read] = '\0';

    // Parse module from file
    auto lexer = Lexer(buffer, read + 1);
    auto parser = Parser(&lexer);
    parser.start();
    parser.parse_module(mod);

    cout << '\n'; // Newline after prompt

    /* Inspect python imports */
    cout << "Imports\n";
    for(unsigned int i = 0; i < mod->num_imports(); ++i) {
        Import *pyimport = mod->nth_import(i);
        cout << "  " << pyimport->module << '\n';
        for(string symbol: pyimport->symbols) {
            // Figure out the names of used classes
            string base = symbol.substr(0, symbol.find('/'));
            string remaining = symbol.substr(symbol.find('/'));
            vector<string> suffixes;
            while(remaining.length() > 0) {
                suffixes.push_back(remaining.substr(0, remaining.find('/')));
                if(remaining.find('/') == string::npos) { break; }
                remaining = remaining.substr(remaining.find('/') + 1);
            }
            for(string suffix: suffixes) {
                cout << "    " << base << suffix << '\n';
            }
        }
    }

    cout << '\n'; // Newline after imports

    /* Inspect module keywords */
    cout << "Keywords\n";
    for(unsigned int i = 0; i < mod->num_keywords(); ++i) {
        cout << "  " << mod->nth_keyword(i) << '\n';
    }

    cout << '\n'; // Newline after keywords

    /* Inspect module classes */
    cout << "Classes\n";
    for(unsigned int cls_num = 0; cls_num < mod->num_classes(); ++cls_num) {
        Class *cls = mod->nth_class(cls_num);
        cout << "  " << cls->name() << '\n';
        for(unsigned int field_num = 0; field_num < cls->num_fields(); ++field_num) {
            Field *field = cls->nth_field(field_num);
            cout << "    " << field->name() << " : " << field->type()->to_string() << '\n';
            print_type(field->type(), 3);
        }
    }

    cout << '\n'; // Newline before prompt

    delete mod;
    return 0;
}
