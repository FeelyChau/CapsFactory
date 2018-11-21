//
// Created by 周飞宇 on 2018/11/20.
//

#include "JavaFieldDefine.h"
#include "Common.h"

JavaFieldDefine::~JavaFieldDefine() {

}

const string JavaFieldDefine::create_get_function(const string &tab) {
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
    static const char* const Template = "public %s %s() {\n"
                                        TB"return %s;\n"
                                        "}\n";
    RETURN_CODEFORMAT(tab.c_str(), Template, result_type.c_str(), func_name.c_str(), camel_name.c_str());
}

const string JavaFieldDefine::create_set_function(const string &tab) {
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
    static const char* const Template = "public void %s (%s %s) {\n"
                                        TB"this.%s = %s;\n"
                                        "}\n";
    RETURN_CODEFORMAT(tab.c_str(), Template, func_name.c_str(),  param_type.c_str(),
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
                                                                            TB"if (sRst != CAPS_SUCCESS) return sRst;\n"
                                                                            TB"Integer wRst = caps.write(c);\n"
                                                                            TB"if (wRst != CAPS_SUCCESS) return wRst;\n"
                                                                            "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_User_Define, camel_name.c_str(), user_define_type_name.c_str(), camel_name.c_str());
        } else {
            static const char* const Template_Serialize_Array_Inner = "caps->write((Integer)%s.Length());\n"
                                                                      "for(%s v : %s) {\n"
                                                                      TB"Integer wRst = caps.write((%s)v);\n"
                                                                      TB"if (wRst != CAPS_SUCCESS) return wRst;\n"
                                                                      "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_Inner, camel_name.c_str(), FieldJavaTypeStr[static_cast<int>(field_type)], camel_name.c_str(), camel_name.c_str());
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
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner, name.c_str(), FieldJavaTypeStr[static_cast<int>(field_type)], name.c_str(), name.c_str(), name.c_str());
        }
    }
}

const string JavaFieldDefine::create_deserialize_function(const string &tab) {
    string camel_name = Common::camel_case(name.c_str());
    if (repeated) {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Deserialize_Array_User_Define = "Integer arraySize_%s = 0;\n"
                                                                              "Integer rRst_%s = caps.read(arraySize_%s);\n"
                                                                              "if (rRst_%s != CAPS_SUCCESS) return rRst_%s;\n"
                                                                              "%s.clear();\n"
                                                                              "for(Integer i = 0; i < arraySize_%s; ++i) {\n"
                                                                              TB"Caps c;\n"
                                                                              TB"if (caps.read(c) == CAPS_SUCCESS && c != null) {\n"
                                                                              TB TB"%s tmpObj = new %s();\n"
                                                                              TB TB"Integer dRst = tmpObj.deserializeForCapsObj(c);\n"
                                                                              TB TB"if (dRst != CAPS_SUCCESS) return dRst;\n"
                                                                              TB TB"%s.add(tmpObj);\n"
                                                                              TB"}\n"
                                                                              "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_User_Define, camel_name.c_str(), camel_name.c_str(),
                              camel_name.c_str(), camel_name.c_str(), camel_name.c_str(), camel_name.c_str(),
                              camel_name.c_str(), user_define_type_name.c_str(), user_define_type_name.c_str(), camel_name.c_str());
        } else {
            static const char* const Template_Deserialize_Array_Inner = "Integer arraySize_%s = 0;\n"
                                                                        "Integer rRst_%s = caps->read(arraySize_%s);\n"
                                                                        "if (rRst_%s != CAPS_SUCCESS) return rRst_%s;\n"
                                                                        "%s.clear();\n"
                                                                        "for(Integer i = 0; i < arraySize_%s; ++i) {\n"
                                                                        TB"%s tmpObj = new %s();\n"
                                                                        TB"Integer rRst = caps->read(ref tmpObj);\n"
                                                                        TB"if (rRst != CAPS_SUCCESS) return rRst;\n"
                                                                        "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Array_Inner, name.c_str(), name.c_str(),
                              name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str(), name.c_str());
        }
    } else {
        if (field_type == FieldType::USERDEFINE) {
            static const char *const Template_Deserialize_User_Define = "Caps c_%s;\n"
                                                                        "Integer rRst_%s = caps->read(ref c_%s);\n"
                                                                        "if (rRst_%s != CAPS_SUCCESS) return rRst_%s;\n"
                                                                        "rRst_%s = %s.desrializeForCapsObj(c_%s);\n"
                                                                        "if (rRst_%s != CAPS_SUCCESS) return rRst_%s;\n";

            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_User_Define, camel_name.c_str(), camel_name.c_str(), camel_name.c_str(), camel_name.c_str(),
                              camel_name.c_str(), camel_name.c_str(), camel_name.c_str(), camel_name.c_str(), camel_name.c_str(), camel_name.c_str());
        } else {
            static const char *const Template_Deserialize_Inner = "Integer rRst_%s = caps->read(%s);\n"
                                                                  "if (rRst_%s != CAPS_SUCCESS) return rRst_%s;\n";

            RETURN_CODEFORMAT(tab.c_str(), Template_Deserialize_Inner, name.c_str(), name.c_str(),
                              name.c_str(), name.c_str());
        }
    }
}

const string JavaFieldDefine::create_class_member(const string &tab) {
    string camel_name = Common::camel_case(name.c_str());
    if (repeated)
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char* const Template_Member_Array_User_Define = "private List<%s> %s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_User_Define, user_define_type_name.c_str(), camel_name.c_str());
        }
        else
        {
            static const char* const Template_Member_Array_String = "private List<%s> %s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_String, FieldJavaTypeStr[static_cast<int>(field_type)], camel_name.c_str());
        }
    }
    else
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char* const Template_Member_User_Define = "private %s %s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define, user_define_type_name.c_str(), camel_name.c_str());
        }
        else
        {
            static const char* const Template_Member_String = "private %s %s;\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_String, FieldJavaTypeStr[static_cast<int>(field_type)], camel_name.c_str());
        }
    }
}
