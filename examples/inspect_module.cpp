#include <iostream>
#include <string>
#include <vector>
#include <bamboo/module/Module.h>
#include <bamboo/module/Class.h>
#include <bamboo/module/Method.h>
#include <bamboo/dcfile/parse.h>
using namespace bamboo;
using namespace std;

vector<string> astron_keywords {"clsend", "ownsend", "clrecv", "ownrecv",
                                "airecv", "broadcast", "ram", "required", "db"};


/* Helper function for printing complex types */
void print_type(Type *typ, int indent) {
    string tab(2 * indent, ' ');
    if(typ->subtype() == kTypeMethod) {
        Method *method = typ->as_method();
        for(int i = 0; i < method->num_parameters(); ++i) {
            Parameter *param = method->get_parameter(i);
            cout << tab;
            if(!param->name().empty()) { cout << param->name() << " : "; }
            cout << param->type()->to_string() << '\n';
            print_type(param->type(), indent + 1);
        }
    } else if(typ->subtype() == kTypeStruct) {
        Struct *cls = typ->as_struct();
        for(int i = 0; i < cls->num_fields(); ++i) {
            Field *field = cls->get_field(i);
            cout << tab;
            if(!field->name().empty()) { cout << field->name() << " : "; }
            cout << field->type()->to_string() << '\n';
            print_type(field->type(), indent + 1);
        }
    }
}

int main(void) {
    Module *mod = new Module();
    for(string keyword : astron_keywords) {
        mod->add_keyword(keyword);
    }
    parse_dcfile(mod, "simple_example.dc");

    cout << '\n'; // Newline after prompt

    /* Inspect python imports */
    cout << "Imports\n";
    for(int i = 0; i < mod->num_imports(); ++i) {
        Import *pyimport = mod->get_import(i);
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
    for(int i = 0; i < mod->num_keywords(); ++i) {
        cout << "  " << mod->get_keyword(i) << '\n';
    }

    cout << '\n'; // Newline after keywords

    /* Inspect module classes */
    cout << "Classes\n";
    for(int cls_num = 0; cls_num < mod->num_classes(); ++cls_num) {
        Class *cls = mod->get_class(cls_num);
        cout << "  " << cls->name() << '\n';
        for(int field_num = 0; field_num < cls->num_fields(); ++field_num) {
            Field *field = cls->get_field(field_num);
            cout << "    " << field->name() << " : " << field->type()->to_string() << '\n';
            print_type(field->type(), 3);
        }
    }

    cout << '\n'; // Newline before prompt

    delete mod;
    return 0;
}
