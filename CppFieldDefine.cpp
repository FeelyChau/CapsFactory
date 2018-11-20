//
// Created by 周飞宇 on 2018/11/17.
//

#include <iostream>
#include <algorithm>
#include "CppFieldDefine.h"
#include "Common.h"




const string CppFieldDefine::create_get_function(const string &tab) {
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

const string CppFieldDefine::create_set_function(const string &tab) {
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

const string CppFieldDefine::create_serialize_function(const string &tab) {
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

const string CppFieldDefine::create_deserialize_function(const string &tab) {
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
                                                                        "int32_t r_rst_%s = caps->read(c_%s);\n"
                                                                        "if (r_rst_%s != CAPS_SUCCESS) return r_rst_%s;\n"
                                                                        "r_rst_%s = _%s.desrialize_for_caps_obj(c_%s);\n"
                                                                        "if (r_rst_%s != CAPS_SUCCESS) return r_rst_%s;\n";

            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_User_Define, name.c_str(), name.c_str(), name.c_str(),
                              name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str());
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

const string CppFieldDefine::create_class_member(const string &tab)
{
    if (repeated)
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char* const Template_Member_Array_User_Define = "std::vector<%s> _%s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_User_Define, user_define_type_name.c_str(), name.c_str());
        }
        else if (field_type == FieldType::STRING)
        {
            static const char* const Template_Member_Array_String = "std::vector<std::string> _%s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_String, name.c_str());
        }
        else
        {
            static const char* const Template_Member_Array_String = "std::vector<%s> _%s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_String, FieldTypeStr[static_cast<int>(field_type)], name.c_str());
        }
    }
    else
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char* const Template_Member_User_Define = "%s _%s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define, user_define_type_name.c_str(), name.c_str());
        }
        else if (field_type == FieldType::STRING)
        {
            static const char* const Template_Member_String = "std::string _%s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_String, name.c_str());
        }
        else
        {
            static const char* const Template_Member_String = "%s _%s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_String, FieldTypeStr[static_cast<int>(field_type)], name.c_str());
        }
    }
}