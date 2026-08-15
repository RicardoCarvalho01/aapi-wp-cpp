
#include "drivers/linux/DbusBluezBackend.hpp"

#include <dbus-cxx.h>
#include <memory>
#include <string>


namespace aa::drivers::linux
{

namespace
{

constexpr const char* BLUEZ_SERVICE = "org.bluez";
constexpr const char* BLUEZ_ROOT_PATH = "/";
constexpr const char* OBJECT_MANAGER_INTERFACE = "org.freedesktop.DBus.ObjectManager";

} // namespace

class DbusBluezBackend::Impl
{
public:
    std::shared_ptr<DBus::Connection> connection;
    std::shared_ptr<DBus::ObjectProxy> objectManager;

    bool started{false};
    bool paired{false};
};

DbusBluezBackend::DbusBluezBackend() : impl_(std::make_unique<Impl>())
{}

DbusBluezBackend::~DbusBluezBackend()
{
    stop();
}

bool DbusBluezBackend::start()
{
    if (impl_->started)
    {
        return true;
    }

    try
    {
        impl_->connection = DBus::Connection::create(DBus::BusType::SYSTEM);

        if (impl_->connection == nullptr)
        {
            return false;
        }

        impl_->objectManager =
            DBus::ObjectProxy::create(impl_->connection, BLUEZ_SERVICE, BLUEZ_ROOT_PATH);

        if (impl_->objectManager == nullptr)
        {
            impl_->connection.reset();
            return false;
        }

        /*
         * This does not perform any pairing yet. It verifies that we
         * can establish the system-bus connection and create a proxy
         * for BlueZ's root object.
         */
        auto getManagedObjects = impl_->objectManager->create_method<
            std::map<std::string, std::map<std::string, std::map<std::string, DBus::Variant>>>()>(
            OBJECT_MANAGER_INTERFACE, "GetManagedObjects");

        if (getManagedObjects == nullptr)
        {
            impl_->objectManager.reset();
            impl_->connection.reset();

            return false;
        }

        (void) (*getManagedObjects)();

        impl_->started = true;
        return true;
    }
    catch (const DBus::Error&)
    {
        impl_->objectManager.reset();
        impl_->connection.reset();

        impl_->started = false;
        impl_->paired = false;

        return false;
    }
}

void DbusBluezBackend::stop()
{
    impl_->paired = false;
    impl_->started = false;

    impl_->objectManager.reset();
    impl_->connection.reset();
}

bool DbusBluezBackend::pair()
{
    if (!impl_->started)
    {
        return false;
    }

    /*
     * Pairing requires a concrete org.bluez.Device1 object path.
     *
     * The next step will discover the appropriate device from
     * ObjectManager.GetManagedObjects() and invoke Device1.Pair().
     */
    return false;
}

void DbusBluezBackend::unpair()
{
    impl_->paired = false;
}

bool DbusBluezBackend::paired() const
{
    return impl_->paired;
}

} // namespace aa::drivers::linux