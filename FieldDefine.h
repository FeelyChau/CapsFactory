//
// Created by 周飞宇 on 2018/11/17.
//

#ifndef CAPSFACTORY_FIELDDEFINE_H
#define CAPSFACTORY_FIELDDEFINE_H

#include <string>
#include "cJSON.h"


using namespace std;

enum class FieldType { STRING = 0, INT32, /*UINT32,*/ INT64, /*UINT64,*/ FLOAT, DOUBLE, USERDEFINE};
const char * const FieldTypeStr[] = {"string", "int32_t", /*"uint32_t",*/ "int64_t", /*"uint64_t",*/ "float", "double", "user_define"};

class FieldDefine {
public:
    FieldDefine() = default;

    bool Parse(cJSON* root);

    void setName(const string &name);

    void setField_type(FieldType field_type);

    void setOptional(bool optional);

    void setRepeated(bool repeated);

    const string &getName() const;

    FieldType getField_type() const;

    bool isOptional() const;

    bool isRepeated() const;

    const string createGetFunction(const string &tab);

    const string createSetFunction(const string &tab);

    const string createSerializeFunction(const string &tab);

    const string createDeserializeFunction(const string &tab);

    const string createClassMember(const string &tab);

private:
    string name;
    FieldType field_type;
    string user_define_type_name;
    bool optional = false;
    bool repeated = false;
};


#endif //CAPSFACTORY_FIELDDEFINE_H
