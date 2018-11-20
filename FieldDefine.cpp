//
// Created by 周飞宇 on 2018/11/17.
//

#include <iostream>
#include <algorithm>
#include "FieldDefine.h"
#include "Common.h"


bool FieldDefine::Parse(cJSON *root) {
    if (root->type != cJSON_Object)
        return false;
    cJSON * name = cJSON_GetObjectItem(root, "Name");
    if (name && name->type == cJSON_String)
        this->name = name->valuestring;
    else
        return false;

    cJSON * ftype = cJSON_GetObjectItem(root, "Type");
    if (ftype && ftype->type == cJSON_String)
    {
        string field_type_name = ftype->valuestring;
        std::transform(field_type_name.begin(), field_type_name.end(), field_type_name.begin(), ::tolower);
        if (field_type_name == "string")
            field_type = FieldType::STRING;
        else if (field_type_name == "int32")
            field_type = FieldType::INT32;
//        else if (field_type_name == "uint32")
//            field_type = FieldType::UINT32;
//        else if (field_type_name == "uint64")
//            field_type = FieldType::UINT64;
        else if (field_type_name == "int64")
            field_type = FieldType::INT64;
        else if (field_type_name == "float")
            field_type = FieldType::FLOAT;
        else if (field_type_name == "double")
            field_type = FieldType::DOUBLE;
        else
        {
            user_define_type_name = ftype->valuestring;
            field_type = FieldType::USERDEFINE;
        }
    }
    else
        return false;



    cJSON * optional = cJSON_GetObjectItem(root, "Optional");
    if (!optional)
    {
        cout<<"[Optional] of field ["<<this->name<<"] is invalid(defalue 'false')"<<endl;
        this->optional = false;
    }
    else if (optional->type == cJSON_False)
        this->optional = false;
    else if (optional->type == cJSON_True)
        this->optional = true;

    cJSON * repeated = cJSON_GetObjectItem(root, "Repeated");
    if (!repeated)
    {
        cout<<"[repeated] of field ["<<this->name<<"] is invalid(defalue 'false')"<<endl;
        this->repeated = false;
    }
    else if (repeated->type == cJSON_False)
        this->repeated = false;
    else if (repeated->type == cJSON_True)
        this->repeated = true;

    return true;
}

void FieldDefine::setName(const string &name) {
    FieldDefine::name = name;
}

void FieldDefine::setField_type(FieldType field_type) {
    FieldDefine::field_type = field_type;
}

void FieldDefine::setOptional(bool optional) {
    FieldDefine::optional = optional;
}

void FieldDefine::setRepeated(bool repeated) {
    FieldDefine::repeated = repeated;
}

const string &FieldDefine::getName() const {
    return name;
}

FieldType FieldDefine::getField_type() const {
    return field_type;
}

bool FieldDefine::isOptional() const {
    return optional;
}

bool FieldDefine::isRepeated() const {
    return repeated;
}

const string FieldDefine::createGetFunction(const string &tab) {
    if (repeated) {
        if (field_type == FieldType::USERDEFINE) {
            static const char *const Template_TypeName_Array_User_Define = "const std::vector<%s>& get_%s() const { return _%s; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_User_Define, user_define_type_name.c_str(),
                              name.c_str(), name.c_str());
        } else if (field_type == FieldType::STRING) {
            static const char *const Template_TypeName_Array_String = "const std::vector<std::string>& get_%s() const { return _%s; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_String, name.c_str(), name.c_str());
        } else {
            static const char *const Template_TypeName_Array_Inner = "const std::vector<%s>& get_%s() const { return _%s; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_Inner, FieldTypeStr[static_cast<int>(field_type)],
                              name.c_str(), name.c_str());
        }
    } else {
        if (field_type == FieldType::USERDEFINE) {
            static const char *const Template_TypeName_User_Define = "const %s & get_%s() const { return _%s; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_User_Define, user_define_type_name.c_str(), name.c_str(),
                              name.c_str());
        } else if (field_type == FieldType::STRING) {
            static const char *const Template_TypeName_String = "const std::string& get_%s() const { return _%s; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_String, name.c_str(), name.c_str());
        } else {
            static const char *const Template_TypeName_Inner = "%s get_%s() const { return _%s; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Inner, FieldTypeStr[static_cast<int>(field_type)],
                              name.c_str(), name.c_str());
        }
    }
}

const string FieldDefine::createSetFunction(const string &tab) {
    if (repeated)
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char *const Template_TypeName_Array_User_Define_Set = "void set_%s(const std::vector<%s> &v) { _%s = v; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_User_Define_Set, name.c_str(), user_define_type_name.c_str(), name.c_str());
        }

        else if (field_type == FieldType::STRING)
        {
            static const char *const Template_TypeName_Array_String_Set = "void set_%s(const std::vector<std::string> &v) { _%s = v; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_String_Set, name.c_str(), name.c_str());
        }
        else
        {
            static const char *const Template_TypeName_Array_Inner_Set = "void set_%s(const std::vector<%s> &v) { _%s = v; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Array_Inner_Set, name.c_str(), FieldTypeStr[static_cast<int>(field_type)], name.c_str());
        }
    }
    else
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char *const Template_TypeName_User_Define_Set = "void set_%s(const %s &v) { _%s = v; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_User_Define_Set, name.c_str(), user_define_type_name.c_str(), name.c_str());
        }

        else if (field_type == FieldType::STRING)
        {
            static const char *const Template_TypeName_String_Set = "void set_%s(const std::string &v) { _%s = v; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_String_Set, name.c_str(), name.c_str());
        }
        else
        {
            static const char *const Template_TypeName_Inner_Set = "void set_%s(%s v) { _%s = v; }\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_TypeName_Inner_Set, name.c_str(), FieldTypeStr[static_cast<int>(field_type)], name.c_str());
        }
    }
}

const string FieldDefine::createSerializeFunction(const string &tab) {
    if (repeated) {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_Array_User_Define = "caps->write((int32_t)_%s.size());\n"
                                                                            "for(auto &v : _%s) {\n"
                                                                            TB"std::shared_ptr<Caps> c;\n"
                                                                            TB"int32_t s_rst = v.serialize_for_caps_obj(c);\n"
                                                                            TB"if (s_rst != CAPS_SUCCESS) return s_rst;\n"
                                                                            TB"int32_t w_rst = caps->write(c);\n"
                                                                            TB"if (w_rst != CAPS_SUCCESS) return w_rst;\n"
                                                                            "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_User_Define, name.c_str(), name.c_str());
        } else if (field_type == FieldType::STRING) {
            static const char* const Template_Serialize_Array_String = "caps->write((int32_t)_%s.size());\n"
                                                                       "for(auto &v : _%s) {\n"
                                                                       TB"int32_t w_rst = caps->write(v.c_str());\n"
                                                                       TB"if (w_rst != CAPS_SUCCESS) return w_rst;\n"
                                                                       "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_String, name.c_str(), name.c_str());
        } else {
            static const char* const Template_Serialize_Array_Inner = "caps->write((int32_t)_%s.size());\n"
                                                                      "for(auto &v : _%s) {\n"
                                                                      TB"int32_t w_rst = caps->write((%s)v);\n"
                                                                      TB"if (w_rst != CAPS_SUCCESS) return w_rst;\n"
                                                                      "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_Inner, name.c_str(), name.c_str(), FieldTypeStr[static_cast<int>(field_type)]);
        }
    } else {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_User_Define = "std::shared_ptr<Caps> c_%s;\n"
                                                                      "int32_t s_rst_%s = _%s.serialize_for_caps_obj(c_%s);"
                                                                      "if (s_rst_%s != CAPS_SUCCESS) return s_rst_%s;\n"
                                                                      "int32_t w_rst = caps->write(c_%s);\n"
                                                                      "if (w_rst != CAPS_SUCCESS) return w_rst;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_User_Define, name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str());
        } else if (field_type == FieldType::STRING) {
            static const char* const Template_Serialize_String = "int32_t w_rst_%s = caps->write(_%s.c_str());\n"
                                                                 "if (w_rst_%s != CAPS_SUCCESS) return w_rst_%s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_String, name.c_str(), name.c_str(), name.c_str(), name.c_str());
        } else {
            static const char* const Template_Serialize_Inner = "int32_t w_rst_%s = caps->write((%s)_%s);\n"
                                                                "if (w_rst_%s != CAPS_SUCCESS) return w_rst_%s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner, name.c_str(), FieldTypeStr[static_cast<int>(field_type)], name.c_str(), name.c_str(), name.c_str());
        }
    }
}

const string FieldDefine::createDeserializeFunction(const string &tab) {
    if (repeated) {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Deserialize_Array_User_Define = "int32_t array_size_%s = 0;\n"
                                                                              "int32_t r_rst_%s = caps->read(array_size_%s);\n"
                                                                              "if (r_rst_%s != CAPS_SUCCESS) return r_rst_%s;\n"
                                                                              "_%s.clear();\n"
                                                                              "for(int32_t i = 0; i < array_size_%s;++i) {\n"
                                                                              TB"std::shared_ptr<Caps> c;\n"
                                                                              TB"if (caps->read(c) == CAPS_SUCCESS && c) {\n"
                                                                              TB TB"_%s.emplace_back();\n"
                                                                              TB TB"int32_t d_rst = _%s.back().deserialize_for_caps_obj(c);\n"
                                                                              TB TB"if (d_rst != CAPS_SUCCESS) return d_rst;\n"
                                                                              TB"}\n"
                                                                              "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_User_Define, name.c_str(), name.c_str(),
                    name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str());
        } else if (field_type == FieldType::STRING) {
            static const char* const Template_Deserialize_Array_String = "int32_t array_size_%s = 0;\n"
                                                                         "int32_t r_rst_%s = caps->read(array_size_%s);\n"
                                                                         "if (r_rst_%s != CAPS_SUCCESS) return r_rst_%s;\n"
                                                                         "_%s.clear();\n"
                                                                         "for(int32_t i = 0; i < array_size_%s;++i) {\n"
                                                                         TB"_%s.emplace_back();\n"
                                                                         TB"int32_t r_rst = caps->read_string(_%s.back());\n"
                                                                         TB"if (r_rst != CAPS_SUCCESS) return r_rst;\n"
                                                                         "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_String, name.c_str(), name.c_str(),
                              name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str());
        } else {
            static const char* const Template_Deserialize_Array_Inner = "int32_t array_size_%s = 0;\n"
                                                                        "int32_t r_rst_%s = caps->read(array_size_%s);\n"
                                                                        "if (r_rst_%s != CAPS_SUCCESS) return r_rst_%s;\n"
                                                                        "_%s.clear();\n"
                                                                        "for(int32_t i = 0; i < array_size_%s;++i) {\n"
                                                                        TB"_%s.emplace_back();\n"
                                                                        TB"int32_t r_rst = caps->read(_%s.back());\n"
                                                                        TB"if (r_rst != CAPS_SUCCESS) return r_rst;\n"
                                                                        "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_Inner, name.c_str(), name.c_str(),
                              name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str());
        }
    } else {
        if (field_type == FieldType::USERDEFINE) {
            static const char *const Template_Deserialize_User_Define = "std::shared_ptr<Caps> c_%s;\n"
                                                                              "int32_t r_rst_%s = caps->read(c);\n"
                                                                              "if (r_rst_%s != CAPS_SUCCESS) return r_rst_%s;\n"
                                                                              "r_rst_%s = _%s.desrialize_for_obj(c);\n"
                                                                              "if (r_rst_%s != CAPS_SUCCESS) return r_rst_%s;\n";

            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_User_Define, name.c_str(), name.c_str(),
                              name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str());
        }
        else if (field_type == FieldType::STRING) {
            static const char *const Template_Deserialize_String =
                                                                  "int32_t r_rst_%s = caps->read_string(_%s);\n"
                                                                  "if (r_rst_%s != CAPS_SUCCESS) return r_rst_%s;\n";

            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_String, name.c_str(), name.c_str(),
                              name.c_str(), name.c_str());
        } else {
            static const char *const Template_Deserialize_Inner = "int32_t r_rst_%s = caps->read(_%s);\n"
                                                                   "if (r_rst_%s != CAPS_SUCCESS) return r_rst_%s;\n";

            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Inner, name.c_str(), name.c_str(),
                              name.c_str(), name.c_str());
        }
    }
}

const string FieldDefine::createClassMember(const string &tab)
{
    if (repeated)
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char* const Template_Member_Array_User_Define = "std::vector<%s> _%s\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_User_Define, user_define_type_name.c_str(), name.c_str());
        }
        else if (field_type == FieldType::STRING)
        {
            static const char* const Template_Member_Array_String = "std::vector<std::string> _%s\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_String, name.c_str());
        }
        else
        {
            static const char* const Template_Member_Array_String = "std::vector<%s> _%s\n";
            RETURN_CODEFORMAT(tab.c_str(), FieldTypeStr[static_cast<int>(field_type)], name.c_str());
        }
    }
    else
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char* const Template_Member_User_Define = "%s _%s\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define, user_define_type_name.c_str(), name.c_str());
        }
        else if (field_type == FieldType::STRING)
        {
            static const char* const Template_Member_String = "std::string _%s\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_String, name.c_str());
        }
        else
        {
            static const char* const Template_Member_String = "%s _%s\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_String, FieldTypeStr[static_cast<int>(field_type)], name.c_str());
        }
    }
}