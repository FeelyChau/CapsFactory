//
// Created by 周飞宇 on 2018/11/21.
//

#include "JSMsgDefine.h"
#include "JSFieldDefine.h"
#include "Common.h"

JSMsgDefine::~JSMsgDefine() {

}

string JSMsgDefine::create_code_string(const string &tab) {
    static const char* const Template_Msg_Class = "class %s {\n"
                                                  TB"constructor (){\n"
                                                  "%s\n"
                                                  TB"}\n\n"
                                                  "%s"
                                                  "}\n\n";
    //members
    string members;
    for(auto &field : fields)
        members += field->create_class_member(TBTB);
    string getter;
    //field getter
    for(auto &field : fields)
        getter += field->create_get_function(TAB_VALUE);
    //field setter
    string setter;
    for(auto &field : fields)
        setter += field->create_set_function(TAB_VALUE);
    //serialize
    string serialize = create_serialize(TAB_VALUE);
    //deserialize
    string deserialize = create_deserialize(TAB_VALUE);
    //serialize for caps obj
    string serialize_for_caps_obj = create_serialize_for_caps_obj(TAB_VALUE);
    //deserialize for caps obj
    string deserialize_for_caps_obj = create_deserialize_for_caps_obj(TAB_VALUE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Msg_Class, msg_name.c_str(), members.c_str(), (getter + setter + serialize + deserialize + serialize_for_caps_obj + deserialize_for_caps_obj).c_str());
}

string JSMsgDefine::create_serialize(const string &tab) {
    static const char* const Template_Serialize = "serialize() {\n"
                                                  TB"var caps = new Caps();\n"
                                                  TB"caps.writeInt32(MessageType.TYPE_%s)\n"
                                                  "%s\n"
                                                  TB"return caps;\n"
                                                  "}\n\n";
    string field_function_str;
    for(auto &field : fields)
        field_function_str += field->create_serialize_function(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_Serialize, msg_name_upper.c_str(), field_function_str.c_str());
}

string JSMsgDefine::create_deserialize(const string &tab) {
    static const char* const Template_Deserialize =
            "deserialize(caps) {\n"
            TB"var t = caps.readInt32()\n"
            TB"if (t != MessageType.TYPE_%s)\n"
            TBTB"return false\n"
            "%s\n"
            TB"return true;\n"
            "}\n\n";
    string field_function_str;
    for(auto &field : fields)
        field_function_str += field->create_deserialize_function(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize, msg_name_upper.c_str(), field_function_str.c_str());
}

string JSMsgDefine::create_serialize_for_caps_obj(const string &tab) {
    static const char* const Template_Serialize = "serializeForCapsObj() {\n"
                                                  TB"var caps = new Caps();\n"
                                                  "%s\n"
                                                  TB"return caps;\n"
                                                  "}\n\n";
    string field_function_str;
    for(auto &field : fields)
        field_function_str += field->create_serialize_function(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_Serialize, field_function_str.c_str());
}

string JSMsgDefine::create_deserialize_for_caps_obj(const string &tab) {
    static const char* const Template_Deserialize =
            "deserializeForCapsObj(caps) {\n"
            "%s\n"
            TB"return true;\n"
            "}\n\n";
    string field_function_str;
    for(auto &field : fields)
        field_function_str += field->create_deserialize_function(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize, field_function_str.c_str());
}

shared_ptr<BaseFieldDefine> JSMsgDefine::new_field_define() {
    return std::make_shared<JSFieldDefine>();
}
