//
// Created by 周飞宇 on 2018/11/21.
//

#ifndef CAPSFACTORY_JSFIELDDEFINE_H
#define CAPSFACTORY_JSFIELDDEFINE_H
#include "BaseAll.h"

const char * const FieldJSWriterFunction[] = {"writeString", "writeInt32", "writeUInt32", "WriteInt64", "xxxxx", "writeFloat", "WriteDouble", "writeCaps"};
const char * const FieldJSReadFunction[] = {"readString", "readInt32", "readUInt32", "readInt64", "xxxxx", "readFloat", "readDouble", "readCaps"};
const char * const FieldJSTypeDefault[] = {"\"\"", "0", "0", "0", "0", "0.0", "0.0", "{}"};
const char * const FieldJSTypeOf[] = {"string", "number", "number", "number", "number", "number", "number", "object"};


class JSFieldDefine: public BaseFieldDefine {
public:
    ~JSFieldDefine() override;

    const string create_get_function(const string &tab) override;

    const string create_set_function(const string &tab) override;

    const string create_serialize_function(const string &tab) override;

    const string create_deserialize_function(const string &tab) override;

    const string create_class_member(const string &tab) override;
};


#endif //CAPSFACTORY_JSFIELDDEFINE_H
