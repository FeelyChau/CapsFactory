#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cstring>

#include "cJSON.h"
#include "CppMsgGroup.h"
#include "JavaMsgGroup.h"
#include "Common.h"
#include "clargs.h"
#include "JSMsgGroup.h"

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




static void print_prompt(const char* progname) {
    static const char* form = "CapsFactory\n\n"
                              "USAGE: %s [options]\n"
                              "options:\n"
                              "\t-i input_file_path        set the message-define file path\n"
                              "\t-l [cpp, c, js, java]      set code language";
    printf(form, progname);
}
#define FUNC_STRING(v) "\"\\\"" #v "\\\":\\\"%s\\\"\"\n"
#define FUNC_UINT32(v) "\"\\\"" #v "\\\":%u\""
#define FUNC_INT32(v) "\"\\\"" #v "\\\":%d\""
#define FUNC_UINT64(v) "\"\\\"" #v "\\\":%llu\""
#define FUNC_INT64(v) "\"\\\"" #v "\\\":%lld\""
#define FUNC_OBJ(v) "\"\\\"" #v "\\\":%s\""
#define FUNC_ARRAY(v) "\"\\\"" #v "\\\":[%s]\""


int main(int argc, char** argv) {
//    char * abc;
//    int64_t a;
//    printf("%s\n", FUNC_STRING(abc));
//    printf("%s\n", FUNC_UINT32(abc));
//    printf("%s\n", FUNC_INT32(abc));
//    printf("%s\n", FUNC_UINT64(abc));
//    printf("%s\n", FUNC_INT64(abc));
//    printf("%s\n", FUNC_OBJ(abc));
//    printf("%s\n", FUNC_ARRAY(abc));
//    printf(FUNC_STRING(abc), "abddddc");
    // parse arguments
    clargs_h h = clargs_parse(argc, argv);
    if (h == 0 || clargs_opt_has(h, "h")) {
        print_prompt(argv[0]);
        clargs_destroy(h);
        return 1;
    }
    const char* input_file = clargs_opt_get(h, "i");
    const char* l = clargs_opt_get(h, "l");
    string language;
    if (l != nullptr)
        language = l;
    string s = Common::read_file(input_file);
    cJSON *root = cJSON_Parse(s.c_str());
    BaseMsgGroup *mg = nullptr;
    if (language == "cpp")
        mg = new CppMsgGroup();
    else if (language == "java")
        mg = new JavaMsgGroup();
    else if (language == "js")
        mg = new JSMsgGroup();
    else
        print_prompt(argv[0]);
    if (mg)
    {
        mg->parse(root);
        mg->create_code_file("./");
        delete mg;
    }
    clargs_destroy(h);
    return 0;
}