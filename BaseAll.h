//
// Created by 周飞宇 on 2018/11/20.
//

#ifndef CAPSFACTORY_BASEALL_H
#define CAPSFACTORY_BASEALL_H

#include "cJSON.h"
#include <string>
#include <vector>

using namespace std;

enum class FieldType { STRING = 0, INT32, UINT32, INT64, UINT64, FLOAT, DOUBLE, USERDEFINE};
enum class CodeType { SOURCE = 0, HEAD};

typedef struct _sDefaultValue {
    int64_t i;
    double d;
    std::string s;
    void *p;
} DefaultValue;

typedef struct _sDefaultDef {
    bool is_set;
    FieldType ft;
    DefaultValue value;
} DefaultDef;

class BaseMsgDefine;
class BaseMsgGroup;
class BaseFieldDefine {
public:
    BaseFieldDefine() = default;
    virtual ~BaseFieldDefine() = default;

    bool parse(cJSON* root);

    void set_name(const string &name);

    void set_field_type(FieldType field_type);

    void set_optional(bool optional);

    void set_repeated(bool repeated);

    const string &get_name() const;

    FieldType get_field_type() const;

    bool is_optional() const;

    bool is_repeated() const;

    void set_msg_define(BaseMsgDefine *msgDefine);

    bool is_userdefine_data();

    const string & get_user_define_type_name();

    virtual const string create_get_function(const string &tab, CodeType ct) = 0;

    virtual const string create_set_function(const string &tab, CodeType ct) = 0;

    virtual const string create_serialize_function(const string &tab, CodeType ct) = 0;

    virtual const string create_deserialize_function(const string &tab, CodeType ct) = 0;

    virtual const string create_class_member(const string &tab, CodeType ct) = 0;

    virtual const string create_to_json_function(const string &tab, CodeType ct) = 0;

protected:
    string name;
    FieldType field_type;
    DefaultDef defualt_value;
    string user_define_type_name;
    bool optional = false;
    bool repeated = false;
    string camel_name;
    string head_up_name;
    string comment;
    BaseMsgDefine *msgDefine;
};


class BaseMsgDefine {
protected:
    string msg_name = "";
    string msg_name_upper = "";
    string comment = "";
    BaseMsgGroup *msg_group = nullptr;
public:
    const string &get_msg_name_upper() const;

    const string &get_msg_name() const;

    const vector<shared_ptr<BaseFieldDefine>> &get_fields() const;

    void set_msg_group(BaseMsgGroup *msg_group);
protected:
    vector<std::shared_ptr<BaseFieldDefine>> fields;
    virtual string create_serialize(const string &tab, CodeType ct) = 0;
    virtual string create_deserialize(const string &tab, CodeType ct) = 0;
    virtual string create_serialize_for_caps_obj(const string &tab, CodeType ct) = 0;
    virtual string create_deserialize_for_caps_obj(const string &tab, CodeType ct) = 0;
    virtual std::shared_ptr<BaseFieldDefine> new_field_define() = 0;
public:
    BaseMsgDefine() = default;
    virtual ~BaseMsgDefine() = default;
    bool parse(cJSON *root);
    virtual string create_code_string(const string &tab, CodeType ct) = 0;
};

class BaseMsgGroup {
protected:
    string ns;
    vector<std::shared_ptr<BaseMsgDefine>> msg_define;
    virtual std::shared_ptr<BaseMsgDefine> new_msg_define() = 0;
    string comment;
public:
    BaseMsgGroup() = default;
    virtual ~BaseMsgGroup() = default;

    bool parse(cJSON *root);
    virtual void create_code_file(const string &file_path) = 0;
    const string& get_ns();
};


#endif //CAPSFACTORY_BASEALL_H
