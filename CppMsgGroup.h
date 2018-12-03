//
// Created by 周飞宇 on 2018/11/17.
//

#ifndef CAPSFACTORY_MSGGROUP_H
#define CAPSFACTORY_MSGGROUP_H
#include "BaseAll.h"
#include "CppMsgDefine.h"

using namespace std;


class CppMsgGroup: public BaseMsgGroup {
protected:
    virtual std::shared_ptr<BaseMsgDefine> new_msg_define() override;
public:
    CppMsgGroup() = default;
    virtual ~CppMsgGroup() override {};
    virtual void create_code_file(const string &file_path) override;
};

#endif //CAPSFACTORY_MSGGROUP_H
