#include <Path.h>

using namespace std;

namespace viu2x {

    const wstring Path::EntityDelimiter = L">";
    const wstring Path::ProtocolDelimiter = L":";
    const wstring Path::HostDelimiter = L"//";
    const wstring Path::FolderDelimiter = L"/";
    const wstring Path::UserDelimiter = L"@";
    const wstring Path::PasswordDelimiter = L":";

    enum PathDelimiter
    {
        FolderDelimiter,
        EntityDelimiter,
        ProtocolDelimiter,
        HostDelimiter,
        UserDelimiter,
        PasswordDelimiter
    };

    /**
     *
     */
    Path::Path() {
    }

    /**
     *
     */
    Path::Path(const Path & source) {
        Entities = source.Entities;
    }

    /**
     *
     */
    Path::Path(const wstring & source) {
        deserialize(source);
    }

    /**
     *
     */
    Path::operator wstring() const {
        return serialize();
    }

    /**
     *
     */
    wstring Path::serialize() const {
        wstring result = L"";

        for (vector<Entity>::const_iterator entity = Entities.begin(); entity != Entities.end(); ++entity)
        {
            wstring entityPath = L"";

            // Set the last entry
            if (entity->Entries.size() > 0)
                entityPath = entity->Entries[entity->Entries.size() - 1];

            // Attach other entries
            for (int i = entity->Entries.size() - 2; i >= 0; --i)
                entityPath = entity->Entries[i] + FolderDelimiter + entityPath;

            // Make host section
            wstring hostSection = L"";
            hostSection = entity->Host;
            if (entity->Username.length() > 0)
            {
                if (entity->Password.length() > 0)
                    hostSection = HostDelimiter + entity->Username + PasswordDelimiter + entity->Password + UserDelimiter + hostSection;
                else hostSection = HostDelimiter + entity->Username + UserDelimiter + hostSection;
            }
            // Attach host section without host delimiter
            if (hostSection.length() > 0)
                entityPath = hostSection + FolderDelimiter + entityPath;

            // Attach protocol and the host delimiter
            if (entity->Protocol.length() > 0)
                entityPath = entity->Protocol + ProtocolDelimiter + HostDelimiter + entityPath;
            else if (hostSection.length() > 0)
                entityPath = HostDelimiter + entityPath;

            // Attach the entity to the final result
            if (result.length() == 0)
                result = entityPath;
            else result += EntityDelimiter + entityPath;
        }

        return result;
    }

    /**
     *
     */
    void Path::deserialize(const wstring & path) {

    }

    /**
     *
     */
    Path & Path::operator = (const wstring & source) {
        deserialize(source);
        return *this;
    }

    /**
     *
     */
    Path Path::operator + (const Path & op) const {
        return Path();
    }

    /**
     *
     */
    Path Path::operator + (const wstring & op) const {
        return Path();
    }

    /**
     *
     */
    Path Path::operator - (const Path & op) const {
        return Path();
    }

    /**
     *
     */
    Path Path::operator - (const wstring & op) const {
        return Path();
    }

    /**
     *
     */
    Path & Path::operator -- () {
        return *this;
    }
}
