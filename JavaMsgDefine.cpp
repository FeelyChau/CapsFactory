//
// Created by 周飞宇 on 2018/11/20.
//

#include "JavaMsgDefine.h"
#include "JavaFieldDefine.h"
#include "Common.h"

JavaMsgDefine::~JavaMsgDefine() {

}

string JavaMsgDefine::create_code_string(const string &tab) {
    static const char* const Template_Msg_Class = "class %s {\n"
                                                  "%s"
                                                  "%s"
                                                  "};\n\n";

    //members
    string members;
    for(auto &field : fields)
        members += field->create_class_member(TB);
    string getter;
    //field getter
    for(auto &field : fields)
        getter += field->create_get_function(TB);
    //field setter
    string setter;
    for(auto &field : fields)
        setter += field->create_set_function(TB);
    //serialize
    string serialize = create_serialize(TB);
    //deserialize
    string deserialize = create_deserialize(TB);
    //serialize for caps obj
    string serialize_for_caps_obj = create_serialize_for_caps_obj(TB);
    //deserialize for caps obj
    string deserialize_for_caps_obj = create_deserialize_for_caps_obj(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_Msg_Class, msg_name.c_str(), members.c_str(), (getter + setter + serialize + deserialize + serialize_for_caps_obj + deserialize_for_caps_obj).c_str());
}

string JavaMsgDefine::create_serialize(const string &tab) {
    static const char* const Template_Serialize = "\n/*\n * serialize this object as buffer\n */\nInteger serialize(Integer[] buf) {\n"
                                                  TB"Caps caps = Caps::new_instance();\n"
                                                  TB"caps.write(MessageType.TYPE_%s.value);\n"
                                                  "%s"
                                                  TB"return caps.serialize(buf);\n"
                                                  "}\n\n";
    string field_serialize;
    for(auto &field : fields)
        field_serialize += field->create_serialize_function(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_Serialize, Common::camel_case(msg_name.c_str()).c_str(), field_serialize.c_str());
}

string JavaMsgDefine::create_deserialize(const string &tab) {
    static const char* const Template_Deserialize = "\n/*\n * deserialize this object from buffer\n */\nbool deserialize(Integer[] buf) {\n"
                                                    TB"Caps caps;\n"
                                                    TB"caps = Caps::parse(buf);\n"
                                                    TB"if(caps == null) return false;\n"
                                                    "%s"
                                                    TB"return true;\n"
                                                    "}\n\n";
    string field_deserialize;
    for(auto &field : fields)
        field_deserialize += field->create_deserialize_function(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize, field_deserialize.c_str());
}


string JavaMsgDefine::create_serialize_for_caps_obj(const string &tab)
{
    static const char * const Template_SerializeForCapsObj = "\n/*\n * serialize this object as caps (without message type)\n */\nCaps serializeForCapsObj() {\n"
                                                             TB"caps = Caps::new_instance();\n"
                                                             "%s"
                                                             TB"return caps;\n"
                                                             "}\n\n";

    string field_function_str;
    for(auto &field : fields)
        field_function_str += field->create_serialize_function(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_SerializeForCapsObj, field_function_str.c_str());
}

string JavaMsgDefine::create_deserialize_for_caps_obj(const string &tab)
{
    static const char * const Template_DeserializeForCapsObj = "Integer deserializeForCapsObj(ref Caps caps) {\n"
                                                               "%s"
                                                               TB"return CAPS_SUCCESS;\n"
                                                               "}\n\n";
    string field_deserialize;
    for(auto &field : fields)
        field_deserialize += field->create_deserialize_function(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_DeserializeForCapsObj, field_deserialize.c_str());
}

shared_ptr<BaseFieldDefine> JavaMsgDefine::new_field_define() {
    return make_shared<JavaFieldDefine>();
}
