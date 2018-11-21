//
// Created by 周飞宇 on 2018/11/21.
//

#include "JSFieldDefine.h"
#include "Common.h"

JSFieldDefine::~JSFieldDefine() {

}

const string JSFieldDefine::create_get_function(const string &tab) {
    static const char* const Template_Getter = "get %s () {\n"
                                               TB"return this._%s\n"
                                               "}\n";
    RETURN_CODEFORMAT(tab.c_str(), Template_Getter, name.c_str(), name.c_str());
}

const string JSFieldDefine::create_set_function(const string &tab) {
    static const char* const Template_Getter = "set %s (v) {\n"
                                               TB"this._%s = v\n"
                                               "}\n";
    RETURN_CODEFORMAT(tab.c_str(), Template_Getter, name.c_str(), name.c_str());
}

const string JSFieldDefine::create_serialize_function(const string &tab) {
    if (repeated) {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_Array_User_Define =
                    "caps.writeInt32(_%s.length)\n"
                    "for(var i = 0; i < _%s.length; ++i) {\n"
                    TB"var v = _%s[i]\n"
                    TB"if (!(v instance of %s))\n"
                    TBTB"return null\n"
                    TB"var c = v.serialize_for_caps_obj()\n"
                    TB"if (c == null)\n"
                    TBTB"return null\n"
                    TB"else\n"
                    TBTB"caps.write(c)\n"
                    "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_User_Define,
                    name.c_str(), name.c_str(),
                    name.c_str(), user_define_type_name.c_str());
        } else if (field_type == FieldType::STRING) {
            static const char* const Template_Serialize_Array_String =
                    "caps.writeInt32(_%s.length)\n"
                    "for(var i = 0; i < _%s.length; ++i) {\n"
                    TB"var v = _%s[i]\n"
                    TB"if (typeof v != \"string\")\n"
                    TBTB"return null\n"
                    TB"else"
                    TBTB"caps.write(v)\n"
                    "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_String,
                    name.c_str(), name.c_str(), name.c_str());
        } else {
            static const char* const Template_Serialize_Array_Inner =
                    "caps.writeInt32(_%s.length)\n"
                    "for(var i = 0; i < _%s.length; ++i) {\n"
                    TB"var v = _%s[i]\n"
                    TB"if (typeof v != \"number\")\n"
                    TBTB"return null\n"
                    TB"else\n"
                    TBTB"caps.%s(c)\n"
                    "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_Inner,
                    name.c_str(), name.c_str(),
                    name.c_str(), FieldJSWriterFunction[static_cast<int>(field_type)]);
        }
    } else {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_User_Define =
                    "if (!(_%s instancof %s))\n"
                    TB"return null\n"
                    "var c_%s = _%s.serialize_for_caps_obj()\n"
                    "if (c_%s == null)\n"
                    TB"return null\n"
                    "else\n"
                    TB"caps->write(c_%s)\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_User_Define,
                              name.c_str(), user_define_type_name.c_str(),
                              name.c_str(), name.c_str(), name.c_str(), name.c_str());
        } else {
            static const char* const Template_Serialize_Inner = "caps->%s(_%s)\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner,
                    FieldJSWriterFunction[static_cast<int>(field_type)], name.c_str());
        }
    }
}

const string JSFieldDefine::create_deserialize_function(const string &tab) {
    if (repeated) {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_Array_User_Define =
                    "count_%s = caps.get(index++)\n"
                    "if (typeof count_%s != \"number\")\n"
                    TB"return false\n"
                    "for(var i = 0; i < count_%s; ++i) {\n"
                    TB"var c = caps.get(index++)\n"
                    TB"if (!(c instance of Caps))\n"
                    TBTB"return false\n"
                    TB"if (!_%s.deserialize_for_caps_obj(c))\n"
                    TBTB"return false\n"
                    "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_User_Define,
                              name.c_str(), name.c_str(),
                              name.c_str(), user_define_type_name.c_str());
        }
        else {
            static const char *const Template_Serialize_Array_String =
                    "count_%s = caps.get(index++)\n"
                    "if (typeof count_%s != \"number\")\n"
                    TB"return false\n"
                    "for(var i = 0; i < count_%s; ++i) {\n"
                    TB"var v = caps.get(index++)\n"
                    TB"if (typeof v != \"%s\")\n"
                    TBTB"return false\n"
                    TB"_%s.push(v)\n"
                    "}\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Array_String,
                              name.c_str(), name.c_str(), name.c_str(),
                              FieldJSTypeOf[static_cast<int>(field_type)], name.c_str());
        }
    } else {
        if (field_type == FieldType::USERDEFINE) {
            static const char* const Template_Serialize_User_Define =
                    "c_%s = caps.get(index++)\n"
                    "if (!(c_%s instanceof Caps)\n"
                    TB"return false\n"
                    "if (!_%s.deserialize_for_caps_obj(c_%s))\n"
                    TB"return false\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_User_Define,
                              name.c_str(), name.c_str(), name.c_str(), name.c_str());
        }
        else if (field_type == FieldType::STRING) {
            static const char* const Template_Serialize_Inner = "var tmp_%s = caps->get(index++)\n"
                                                                "if (typeof tmp_%s != \"string\")\n"
                                                                TB"return false\n"
                                                                "else\n"
                                                                TB"_%s = tmp_%s\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner, name.c_str(), name.c_str(),
                              name.c_str(), name.c_str());
        }
        else{
            static const char* const Template_Serialize_Inner = "var tmp_%s = caps->get(index++)\n"
                                                                "if (typeof tmp_%s != \"number\")\n"
                                                                TB"return false\n"
                                                                "else\n"
                                                                TB"_%s = tmp_%s\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Serialize_Inner, name.c_str(), name.c_str(),
                              name.c_str(), name.c_str());
        }
    }
}

const string JSFieldDefine::create_class_member(const string &tab) {
    if (repeated)
    {
        static const char* const Template_Member_Array_User_Define = "this._%s = []\n";
        RETURN_CODEFORMAT(tab.c_str(), Template_Member_Array_User_Define, name.c_str());
    }
    else
    {
        if (field_type == FieldType::USERDEFINE)
        {
            static const char* const Template_Member_User_Define = "this._%s = new %s()\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define,
                    name.c_str(), user_define_type_name.c_str());
        }
        else
        {
            static const char* const Template_Member_User_Define = "this._%s = %s\n";
            RETURN_CODEFORMAT(tab.c_str(), Template_Member_User_Define,
                    name.c_str(),FieldJSTypeDefault[static_cast<int>(field_type)]);
        }

    }
}
