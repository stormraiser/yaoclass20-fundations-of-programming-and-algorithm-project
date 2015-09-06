#pragma once

#include "Types.h"
#include "Shader.h"

using CACore::DoublePair;

namespace CAGUI{

class ReactionDiffusionShader: public Shader<DoublePair>{
private:
    QColor shadeColor[201][201];

public:
    ReactionDiffusionShader();
    QColor shade(DoublePair state);
    void update();
};

}
