//
// Created by 周飞宇 on 2018/11/20.
//

#include "JavaFieldDefine.h"
#include "Common.h"

JavaFieldDefine::~JavaFieldDefine() {

}

const string JavaFieldDefine::create_get_function(const string &tab) {
    string getter_comment = comment.length() == 0 ? "" : "//getter " + comment;
    string func_name = "get_";
    func_name += name;
    func_name = Common::camel_case(func_name.c_str());
    string result_type;
    if (repeated)
    {
        result_type = "List<";
        if (field_type == FieldType::USERDEFINE)
            result_type += user_define_type_name;
        else
            result_type += FieldJavaTypeStr[static_cast<int>(field_type)];
        result_type += ">";
    }
    else
    {
        if (field_type == FieldType::USERDEFINE)
            result_type = user_define_type_name;
        else
            result_type = FieldJavaTypeStr[static_cast<int>(field_type)];
    }
    string camel_name = Common::camel_case(name.c_str());
    static const char* const Template = "\n%s\npublic %s %s() {\n"
                                        TB"return %s;\n"
                                        "}\n";
    RETURN_CODEFORMAT(tab.c_str(), Template, getter_comment.c_str(), result_type.c_str(), func_name.c_str(), camel_name.c_str());
}

const string JavaFieldDefine::create_set_function(const string &tab) {
    string setter_comment = comment.length() == 0 ? "" : "//setter " + comment;
    string func_name = "set_";
    func_name += name;
    func_name = Common::camel_case(func_name.c_str());
    string param_type;
    if (repeated)
    {
        param_type = "List<";
        if (field_type == FieldType::USERDEFINE)
            param_type += user_define_type_name;
        else
            param_type += FieldJavaTypeStr[static_cast<int>(field_type)];
        param_type += ">";
    }
    else
    {
        if (field_type == FieldType::USERDEFINE)
            param_type = user_define_type_name;
        else
            param_type = FieldJavaTypeStr[static_cast<int>(field_type)];
    }
    string camel_name = Common::camel_case(name.c_str());
    static const char* const Template = "\n%s\npublic void %s (%s %s) {\n"
                                        TB"this.%s = %s;\n"
                                        "}\n";
    RETURN_CODEFORMAT(tab.c_str(), Template, setter_comment.c_str(), func_name.c_str(),  param_type.c_str(),
            camel_name.c_str(), camel_name.c_str(), camel_name.c_str());
}

const string JavaFieldDefine::create_serialize_function(const string &tab) {
    string camel_name = Common::camel_case(name.c_str());
    if (repeated) {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_Array_User_Define = "caps.write((Integer)%s.Length());\n"
                                                                            "for(%s v : %s) {\n"
                                                                            TB"Caps c;\n"
                                                                            TB"Integer sRst = v.serializeForCapsObj(ref c);\n"
                                                                            TB"if (sRst != CAPS_SUCCESS) return null;\n"
                                                                            TB"Integer wRst = caps.write(c);\n"
                                                                            TB"if (wRst != CAPS_SUCCESS) return null;\n"
                                                                            "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_User_Define, camel_name.c_str(), user_define_type_name.c_str(), camel_name.c_str());
        } else {
            static const char* const Template_Serialize_Array_Inner = "caps->write((Integer)%s.Length());\n"
                                                                      "for(%s v : %s) {\n"
                                                                      TB"Integer wRst = caps.write((%s)v);\n"
                                                                      TB"if (wRst != CAPS_SUCCESS) return null;\n"
                                                                      "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_Inner, camel_name.c_str(), FieldJavaTypeStr[static_cast<int>(field_type)], camel_name.c_str(), camel_name.c_str());
        }
    } else {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_User_Define = "std::shared_ptr<Caps> c_%s;\n"
                                                                      "int32_t s_rst_%s = _%s.serialize_for_caps_obj(c_%s);"
                                                                      "if (s_rst_%s != CAPS_SUCCESS) return null;\n"
                                                                      "int32_t w_rst = caps->write(c_%s);\n"
                                                                      "if (w_rst != CAPS_SUCCESS) return null;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_User_Define, name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str());
        } else if (field_type == FieldType::STRING) {
            static const char* const Template_Serialize_String = "int32_t w_rst_%s = caps->write(_%s.c_str());\n"
                                                                 "if (w_rst_%s != CAPS_SUCCESS) return null;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_String, name.c_str(), name.c_str(), name.c_str());
        } else {
            static const char* const Template_Serialize_Inner = "int32_t w_rst_%s = caps->write((%s)_%s);\n"
                                                                "if (w_rst_%s != CAPS_SUCCESS) return null;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner, name.c_str(), FieldJavaTypeStr[static_cast<int>(field_type)], name.c_str(), name.c_str());
        }
    }
}

const string JavaFieldDefine::create_deserialize_function(const string &tab) {
    if (repeated) {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Deserialize_Array_User_Define = "Integer arraySize%s = 0;\n"
                                                                              "if (caps.read(arraySize%s) != CAPS_SUCCESS) return false;\n"
                                                                              "%s.clear();\n"
                                                                              "for(Integer i = 0; i < arraySize%s; ++i) {\n"
                                                                              TB"Caps c;\n"
                                                                              TB"if (caps.read(c) == CAPS_SUCCESS && c != null) {\n"
                                                                              TBTB"%s tmpObj = new %s();\n"
                                                                              TBTB"Integer dRst = tmpObj.deserializeForCapsObj(c);\n"
                                                                              TBTB"if (dRst != CAPS_SUCCESS) return false;\n"
                                                                              TBTB"%s.add(tmpObj);\n"
                                                                              TB"}\n"
                                                                              "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_User_Define, head_up_name.c_str(), head_up_name.c_str(),
                              camel_name.c_str(), head_up_name.c_str(), user_define_type_name.c_str(), user_define_type_name.c_str(), camel_name.c_str());
        } else {
            static const char* const Template_Deserialize_Array_Inner = "Integer arraySize%s = 0;\n"
                                                                        "if (caps->read(arraySize%s) != CAPS_SUCCESS) return false;\n"
                                                                        "%s.clear();\n"
                                                                        "for(Integer i = 0; i < arraySize_%s; ++i) {\n"
                                                                        TB"%s tmpObj = new %s();\n"
                                                                        TB"if (caps->read(tmpObj) != CAPS_SUCCESS) return false;\n"
                                                                        TB"%s.add(tmpObj)"
                                                                        "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_Inner, head_up_name.c_str(), head_up_name.c_str(),
                              camel_name.c_str(), head_up_name.c_str(), FieldJavaTypeStr[static_cast<int>(field_type)],
                              FieldJavaTypeStr[static_cast<int>(field_type)], name.c_str());
        }
    } else {
        if (field_type == FieldType::USERDEFINE) {
            static const char *const Template_Deserialize_User_Define = "Caps caps%s;\n"
                                                                        "if (caps->read(caps%s) != CAPS_SUCCESS) return false;\n"
                                                                        "if (%s.desrializeForCapsObj(caps%s) != CAPS_SUCCESS) return false;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_User_Define, head_up_name.c_str(), head_up_name.c_str(),
                    camel_name.c_str(), head_up_name.c_str());
        } else {
            static const char *const Template_Deserialize_Inner = "Integer rRst_%s = caps->read(%s);\n"
                                                                  "if (rRst_%s != CAPS_SUCCESS) return false;\n";

            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Inner, name.c_str(), name.c_str(),
                              name.c_str());
        }
    }
}

const string JavaFieldDefine::create_class_member(const string &tab) {
    string camel_name = Common::camel_case(name.c_str());
    if (repeated)
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char* const Template_Member_Array_User_Define = "//%s\nprivate List<%s> %s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_User_Define, comment.c_str(), user_define_type_name.c_str(), camel_name.c_str());
        }
        else
        {
            static const char* const Template_Member_Array_String = "//%s\nprivate List<%s> %s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_String, comment.c_str(), FieldJavaTypeStr[static_cast<int>(field_type)], camel_name.c_str());
        }
    }
    else
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char* const Template_Member_User_Define = "//%s\nprivate %s %s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define, comment.c_str(), user_define_type_name.c_str(), camel_name.c_str());
        }
        else
        {
            static const char* const Template_Member_String = "//%s\nprivate %s %s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_String, comment.c_str(), FieldJavaTypeStr[static_cast<int>(field_type)], camel_name.c_str());
        }
    }
}
