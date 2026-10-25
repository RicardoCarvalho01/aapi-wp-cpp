#pragma once

#include "core/state/StateTable.hpp"
#include "services/IServices.hpp"


namespace aa::session
{

// What every state action gets to work with. Holds only a reference to the
// services, so actions are testable with fake services and the session never
// needs to know which concrete drivers (Linux, null, fake) are used.
struct SessionContext
{
    aa::services::IServices &services;
};

using SessionTable = aa::core::StateTable<SessionContext>;

// The Android Auto handshake as a constexpr lookup table of function
// pointers (see src/session/SessionTable.cpp for the graph).
[[nodiscard]]
SessionTable const & sessionTable() noexcept;

}
