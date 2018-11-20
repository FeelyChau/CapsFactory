//
// Created by 周飞宇 on 2018/11/20.
//

#ifndef CAPSFACTORY_JAVAMSGGROUP_H
#define CAPSFACTORY_JAVAMSGGROUP_H
#include "BaseAll.h"


class JavaMsgGroup: public BaseMsgGroup {
protected:
    virtual std::shared_ptr<BaseMsgDefine> new_msg_define() override;
public:
    JavaMsgGroup() = default;
    ~JavaMsgGroup() override {};
    virtual string create_code_string() override;
};


#endif //CAPSFACTORY_JAVAMSGGROUP_H
