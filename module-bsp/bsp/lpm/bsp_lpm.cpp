#include "bsp_lpm.hpp"


#if defined(TARGET_RT1051)
#include "board/rt1051/bsp/lpm/RT1051LPM.hpp"
#elif defined(TARGET_Linux)
#include "board/linux/lpm/LinuxLPM.h"
#else
#error "Unsupported target"
#endif

namespace bsp{

    std::optional<std::unique_ptr<LowPowerMode>> LowPowerMode::Create() {

        std::unique_ptr<LowPowerMode> inst;

#if defined(TARGET_RT1051)
        inst = std::make_unique<bsp::RT1051LPM>();
#elif defined(TARGET_Linux)
        inst = std::make_unique<bsp::LinuxLPM>();
#else
#error "Unsupported target"
#endif

        return inst;
    }

}
