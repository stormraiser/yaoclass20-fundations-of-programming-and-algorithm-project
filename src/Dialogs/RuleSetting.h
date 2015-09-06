/**
  * RuleSetting.h
  * Defines struct RuleSetting for wrapping rule settings and passing them around dialogs.
  */

#pragma once

namespace CACore{

class RuleBase;

}

namespace CAGUI{

class ShaderBase;

struct RuleSetting{
    int typeID;
    CACore::RuleBase *rule;
    ShaderBase *shader;
};

}
