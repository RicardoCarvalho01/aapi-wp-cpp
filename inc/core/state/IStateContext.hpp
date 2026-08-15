#pragma once


namespace aa::core
{

class State;

// Gives every state access to the (unique) instance of every other state
// it might transition to, without any state needing to know who owns them
// or how they are wired together. aa::session::Session implements this by
// returning references to its own state members; tests implement it with
// lightweight stand-ins (see tests/session/MockStateContext.hpp).
class IStateContext
{
    public:
        virtual ~IStateContext() = default;

        [[nodiscard]]
        virtual State & disconnectedState() = 0;

        [[nodiscard]]
        virtual State & startWifiState() = 0;

        [[nodiscard]]
        virtual State & waitingWifiState() = 0;

        [[nodiscard]]
        virtual State & startTransportState() = 0;

        [[nodiscard]]
        virtual State & waitingTransportState() = 0;

        [[nodiscard]]
        virtual State & startProtocolState() = 0;

        [[nodiscard]]
        virtual State & waitingProtocolState() = 0;

        [[nodiscard]]
        virtual State & startAuthenticationState() = 0;

        [[nodiscard]]
        virtual State & waitingAuthenticationState() = 0;

        [[nodiscard]]
        virtual State & startServicesState() = 0;

        [[nodiscard]]
        virtual State & waitingServicesState() = 0;

        [[nodiscard]]
        virtual State & runningState() = 0;

        [[nodiscard]]
        virtual State & recoveryState() = 0;
};

}
