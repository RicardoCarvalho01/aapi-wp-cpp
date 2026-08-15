#pragma once

#include "core/state/State.hpp"


namespace aa::session
{

// Transient: brings up the Android Auto application services (video,
// audio, input, ...) over the authenticated link and immediately hands
// off to WaitingServicesState.
class StartServicesState final : public aa::core::State
{
    public:
        void onEnter(aa::core::StateMachine &machine) override;
};

}
