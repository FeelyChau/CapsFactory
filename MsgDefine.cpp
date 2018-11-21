//
// Created by 周飞宇 on 2018/11/17.
//

#include "MsgDefine.h"
#include "Common.h"

bool MsgDefine::Parse(cJSON *root) {
    if (root->type != cJSON_Object)
        return false;
    cJSON * name = cJSON_GetObjectItem(root, "MsgName");
    if (name && name->type == cJSON_String)
        this->msg_name = name->valuestring;
    else
        return false;

    cJSON * members = cJSON_GetObjectItem(root, "Fields");
    if (!members)
        return false;
    else if (members->type == cJSON_Array)
    {
        for(int i = 0; i < cJSON_GetArraySize(members); ++i)
        {
            cJSON *field = cJSON_GetArrayItem(members, i);
            fields.emplace_back();
            if (!fields.back().Parse(field))
                return false;
        }
    }
    return true;
}

string MsgDefine::CreateCpp(string tab) {
    static const char* const Template_Msg_Class = "class %s {\n"
                                                  "private:\n"
                                                  "%s"
                                                  "public:\n"
                                                  "%s"
                                                  "};\n\n";

    //members
    string members;
    for(auto &field : fields)
        members += field.createClassMember(TAB_VALUE);
    string getter;
    //field getter
    for(auto &field : fields)
        getter += field.createGetFunction(TAB_VALUE);
    //field setter
    string setter;
    for(auto &field : fields)
        setter += field.createSetFunction(TAB_VALUE);
    //serialize
    string serialize = createSerialize(TAB_VALUE);
    //deserialize
    string deserialize = createDeserialize(TAB_VALUE);
    //serialize for caps obj
    string serialize_for_caps_obj = createSerializeForCapsObj(TAB_VALUE);
    //deserialize for caps obj
    string deserialize_for_caps_obj = createDeserializeForCapsObj(TAB_VALUE);
    RETURN_CODEFORMAT(tab.c_str(), Template_Msg_Class, msg_name.c_str(), members.c_str(), (getter + setter + serialize + deserialize + serialize_for_caps_obj + deserialize_for_caps_obj).c_str());
}

string MsgDefine::createSerialize(const string &tab) {
    static const char* const Template_Serialize = "int32_t serialize(void* buf, uint32_t bufsize) const {\n"
                                                  TB"std::shared_ptr<Caps> caps = Caps::new_instance();\n"
                                                  TB"caps->write(static_cast<int32_t>(MessageType::TYPE_%s));\n"
                                                  "%s"
                                                  TB"return caps->serialize(buf, bufsize);\n"
                                                  "}\n\n"
                                                  "int32_t serialize(std::shared_ptr<Caps> &caps) const {\n"
                                                  TB"if (!caps)\n"
                                                  TB TB"caps = Caps::new_instance();\n"
                                                  TB"caps->write(static_cast<int32_t>(MessageType::TYPE_%s));\n"
                                                  "%s"
                                                  TB"return CAPS_SUCCESS;\n"
                                                  "}\n\n";
    string field_serialize;
    for(auto &field : fields)
        field_serialize += field.createSerializeFunction(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_Serialize, msg_name.c_str(), field_serialize.c_str(), msg_name.c_str(), field_serialize.c_str());
}

string MsgDefine::createDeserialize(const string &tab) {
    static const char* const Template_Deserialize = "int32_t deserialize(void* buf, uint32_t bufsize) {\n"
                                                    TB"std::shared_ptr<Caps> caps;\n"
                                                    TB"int32_t p_rst = Caps::parse(buf, bufsize, caps);\n"
                                                    TB"if(p_rst != CAPS_SUCCESS) return p_rst;\n"
                                                    "%s"
                                                    TB"return CAPS_SUCCESS;\n"
                                                    "}\n\n"
                                                    "int32_t deserialize(std::shared_ptr<Caps> &caps) {\n"
                                                    "%s"
                                                    TB"return CAPS_SUCCESS;\n"
                                                    "}\n\n";
    string field_deserialize;
    for(auto &field : fields)
        field_deserialize += field.createDeserializeFunction(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize, field_deserialize.c_str(), field_deserialize.c_str());
}


string MsgDefine::createSerializeForCapsObj(const string &tab)
{
    static const char * const Template_SerializeForCapsObj = "int32_t serialize_for_caps_obj(std::shared_ptr<Caps> &caps) const {\n"
                                                             TB"caps = Caps::new_instance();\n"
                                                             "%s"
                                                             TB"return CAPS_SUCCESS;\n"
                                                             "}\n\n";

    string field_function_str;
    for(auto &field : fields)
        field_function_str += field.createSerializeFunction("  ");
    RETURN_CODEFORMAT(tab.c_str(), Template_SerializeForCapsObj, field_function_str.c_str());
}

string MsgDefine::createDeserializeForCapsObj(const string &tab)
{
    static const char * const Template_DeserializeForCapsObj = "int32_t deserialize_for_caps_obj(std::shared_ptr<Caps> caps) {\n"
                                                               "%s"
                                                               TB"return CAPS_SUCCESS;\n"
                                                               "}\n\n";
    string field_deserialize;
    for(auto &field : fields)
        field_deserialize += field.createDeserializeFunction(TB);
    RETURN_CODEFORMAT(tab.c_str(), Template_DeserializeForCapsObj, field_deserialize.c_str());
}

const string &MsgDefine::getMsg_name() const {
    return msg_name;
}

const vector<FieldDefine> &MsgDefine::getFields() const {
    return fields;
}
