//
// Created by 周飞宇 on 2018/11/21.
//

#ifndef CAPSFACTORY_JSFIELDDEFINE_H
#define CAPSFACTORY_JSFIELDDEFINE_H
#include "BaseAll.h"

const char * const FieldJSWriterFunction[] = {"writeString", "writeInt32", "writeUInt32", "writeInt64", "writeUInt64", "writeFloat", "writeDouble", "writeCaps"};
const char * const FieldJSReadFunction[] = {"readString", "readInt32", "readUInt32", "readInt64", "readUInt64", "readFloat", "readDouble", "readCaps"};
const char * const FieldJSTypeDefault[] = {"\"\"", "0", "0", "0", "0", "0.0", "0.0", "{}"};


class JSFieldDefine: public BaseFieldDefine {
public:
    ~JSFieldDefine() override;

    const string create_get_function(const string &tab, CodeType ct) override;

    const string create_to_json_function(const string &tab, CodeType ct) override;

    const string create_set_function(const string &tab, CodeType ct) override;

    const string create_serialize_function(const string &tab, CodeType ct) override;

    const string create_deserialize_function(const string &tab, CodeType ct) override;

    const string create_class_member(const string &tab, CodeType ct) override;
};


#endif //CAPSFACTORY_JSFIELDDEFINE_H
