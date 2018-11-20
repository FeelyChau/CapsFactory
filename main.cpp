#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cstring>

#include "cJSON.h"
#include "MsgGroup.h"
#include "Common.h"
#include "clargs.h"
using namespace std;
/*
 * {
	"NameSpace": "ns",
	"Msg": [{
		"MsgName": "testMsg",
	  	"Members":[
		  {"Name":"id", "Type":"uint32","optional":false, "repeated": false},
		  {"Name":"version", "Type":"string","optional":false, "repeated": false},
		  {"Name":"task_id", "Type":"string","optional":false, "repeated": false},
		  {"Name":"shell_id", "Type":"string","optional":false, "repeated": false},
		  {"Name":"deadline", "Type":"datetime","optional":false, "repeated": false}
		]
	}]

}
 */


//从文件读入到string里
string readFileIntoString(const char * filename)
{
    ifstream ifile(filename);
    ostringstream buf;
    char ch;
    while(buf&&ifile.get(ch))
        buf.put(ch);
    return buf.str();
}


bool writeFile(const char * filename, const string& content)
{
    ofstream ofile(filename);
    ofile.write(content.c_str(), content.size());
    ofile.close();
    return true;
}

static void print_prompt(const char* progname) {
    static const char* form = "CapsFactory\n\n"
                              "USAGE: %s [options]\n"
                              "options:\n"
                              "\t-i input_file_path        set the message-define file path\n"
                              "\t-o output_file_path       set the code file path\n"
                              "\t-l [cpp, c , nodejs]      set code language(support cpp only for now)";
    printf(form, progname);
}


int main(int argc, char** argv) {
    // parse arguments
    clargs_h h = clargs_parse(argc, argv);
    if (h == 0 || clargs_opt_has(h, "help")) {
        print_prompt(argv[0]);
        clargs_destroy(h);
        return 1;
    }
    const char* input_file = clargs_opt_get(h, "i");
    const char* output_file = clargs_opt_get(h, "o");
    const char* l = clargs_opt_get(h, "l");
    string language;
    if (l != nullptr)
        language = l;
    string s = readFileIntoString(input_file);
    cJSON *root = cJSON_Parse(s.c_str());
    MsgGroup mg;
    mg.Parse(root);
    if (language == "cpp")
    {
        string source = mg.CreateCpp();
        writeFile(output_file, source);
    }
    else
        print_prompt(argv[0]);
    clargs_destroy(h);
    return 0;
}