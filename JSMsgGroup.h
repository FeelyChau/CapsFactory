//
// Created by 周飞宇 on 2018/11/21.
//

#ifndef CAPSFACTORY_JSMSGGROUP_H
#define CAPSFACTORY_JSMSGGROUP_H
#include "BaseAll.h"


class JSMsgGroup: public BaseMsgGroup {
public:
    JSMsgGroup() = default;

    ~JSMsgGroup() override;

    void create_code_file(const string &file_path) override;

protected:
    shared_ptr<BaseMsgDefine> new_msg_define() override;
};


#endif //CAPSFACTORY_JSMSGGROUP_H
