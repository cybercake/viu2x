#include <Path.h>

using namespace std;

namespace viu2x {

    const wstring Path::EntityDelimiter = L">";
    const wstring Path::ProtocolDelimiter = L":";
    const wstring Path::HostDelimiter = L"//";
    const wstring Path::PasswordDelimiter = L":";
    const wstring Path::UserDelimiter = L"@";
    const wstring Path::FolderDelimiter = L"/";

    const wstring ProtocolAndHost = Path::ProtocolDelimiter + Path::HostDelimiter;

    enum PathDelimiter
    {
        PathDelimiter_None,
        PathDelimiter_Folder,
        PathDelimiter_Host,
        PathDelimiter_Password,
        PathDelimiter_User,
        PathDelimiter_Protocol,
        PathDelimiter_Entity
    };

    enum PathScanningState
    {
        PathScanningState_Initial,
        PathScanningState_ProtocolEnd,
        PathScanningState_HostStart,
        PathScanningState_UserEnd,
        PathScanningState_PasswordEnd,
        PathScanningState_HostEnd,
        PathScanningState_FolderEnd,
        PathScanningState_EntityEnd
    };

    inline bool checkSubStr(const wstring & s, const size_t & pos, const wstring & delimiter)
    {
        return s.compare(pos, delimiter.length(), delimiter) == 0;
    }

    PathDelimiter scanDelimiter(const wstring & s, size_t & startPos, size_t & endPos, PathScanningState & state)
    {
        PathDelimiter result = PathDelimiter_None;

        for (size_t i = 0; i < s.length(); ++i)
        {
            if (checkSubStr(s, startPos, ProtocolAndHost))
            {
                switch(state)
                {
                    case PathScanningState_Initial:
                    case PathScanningState_EntityEnd:
                        endPos = i - 1;
                        startPos = i + ProtocolAndHost.length();
                        state = PathScanningState_HostStart;
                        return PathDelimiter_Protocol;
                    default: break;
                }
            }

            if (checkSubStr(s, startPos, Path::ProtocolDelimiter))
            {
                switch(state)
                {
                    case PathScanningState_Initial:
                    case PathScanningState_EntityEnd:
                        endPos = i - 1;
                        startPos = i + Path::ProtocolDelimiter.length();
                        state = PathScanningState_ProtocolEnd;
                        return PathDelimiter_Protocol;
                    default: break;
                }
            }

            if (checkSubStr(s, startPos, Path::HostDelimiter))
            {
                switch(state)
                {
                    case PathScanningState_Initial:
                    case PathScanningState_EntityEnd:
                    case PathScanningState_ProtocolEnd:
                        endPos = i - 1;
                        startPos = i + Path::HostDelimiter.length();
                        state = PathScanningState_HostStart;
                        return PathDelimiter_Host;
                    default: break;
                }
            }

            if (checkSubStr(s, startPos, Path::PasswordDelimiter))
            {
                switch(state)
                {
                    case PathScanningState_Initial:
                    case PathScanningState_EntityEnd:
                    case PathScanningState_HostStart:
                        endPos = i - 1;
                        startPos = i + Path::PasswordDelimiter.length();
                        state = PathScanningState_UserEnd;
                        return PathDelimiter_Password;
                    default: break;
                }
            }

            if (checkSubStr(s, startPos, Path::UserDelimiter))
            {
                switch(state)
                {
                    case PathScanningState_Initial:
                    case PathScanningState_EntityEnd:
                    case PathScanningState_HostStart:
                        endPos = i - 1;
                        startPos = i + Path::UserDelimiter.length();
                        state = PathScanningState_UserEnd;
                        return PathDelimiter_User;
                    case PathScanningState_UserEnd:
                        endPos = i - 1;
                        startPos = i + Path::UserDelimiter.length();
                        state = PathScanningState_PasswordEnd;
                        return PathDelimiter_User;
                    default: break;
                }
            }

            if (checkSubStr(s, startPos, Path::FolderDelimiter))
            {
                switch(state)
                {
                    case PathScanningState_Initial:
                    case PathScanningState_EntityEnd:
                        endPos = i - 1;
                        startPos = i + Path::FolderDelimiter.length();
                        state = PathScanningState_FolderEnd;
                        return PathDelimiter_Folder;
                    case PathScanningState_HostStart:
                    case PathScanningState_UserEnd:
                    case PathScanningState_PasswordEnd:
                        endPos = i - 1;
                        startPos = i + Path::FolderDelimiter.length();
                        state = PathScanningState_HostEnd;
                        return PathDelimiter_Folder;
                    case PathScanningState_HostEnd:
                    case PathScanningState_FolderEnd:
                        endPos = i - 1;
                        startPos = i + Path::FolderDelimiter.length();
                        state = PathScanningState_FolderEnd;
                        return PathDelimiter_Folder;
                    default: break;
                }
            }

            if (checkSubStr(s, startPos, Path::EntityDelimiter))
            {
                switch(state)
                {
                    case PathScanningState_Initial:
                    case PathScanningState_EntityEnd:
                        endPos = i - 1;
                        startPos = i + Path::EntityDelimiter.length();
                        state = PathScanningState_EntityEnd;
                        return PathDelimiter_Entity;
                    case PathScanningState_HostStart:
                        endPos = i - 1;
                        startPos = i + Path::EntityDelimiter.length();
                        state = PathScanningState_HostEnd;
                        return PathDelimiter_Entity;
                    case PathScanningState_UserEnd:
                        endPos = i - 1;
                        startPos = i + Path::EntityDelimiter.length();
                        state = PathScanningState_UserEnd;
                        return PathDelimiter_Entity;
                    case PathScanningState_PasswordEnd:
                        endPos = i - 1;
                        startPos = i + Path::EntityDelimiter.length();
                        state = PathScanningState_PasswordEnd;
                        return PathDelimiter_Entity;
                    case PathScanningState_HostEnd:
                        endPos = i - 1;
                        startPos = i + Path::EntityDelimiter.length();
                        state = PathScanningState_FolderEnd;
                        return PathDelimiter_Entity;
                    case PathScanningState_FolderEnd:
                        endPos = i - 1;
                        startPos = i + Path::EntityDelimiter.length();
                        state = PathScanningState_FolderEnd;
                        return PathDelimiter_Entity;
                    default: break;
                }
            }
        }

        startPos = s.length();
        state = PathScanningState_EntityEnd;

        return result;
    }

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

        // Clear current data
        Entities.clear();

        PathScanningState state = PathScanningState_Initial;
        size_t newStartPos = 0;
        Entity entity;

        while (newStartPos < path.length())
        {
            size_t startPos = newStartPos;
            size_t endPos = startPos;

            // Scan the string until the next delimiter
            PathDelimiter delimiter = scanDelimiter(path, newStartPos, endPos, state);

            switch (state)
            {
                case PathScanningState_ProtocolEnd:
                    // The previous sub string is a protocol identifier
                    entity.Protocol = path.substr(startPos, endPos - startPos + 1);
                    break;
                case PathScanningState_UserEnd:
                    // The previous sub string is a user name
                    entity.Username = path.substr(startPos, endPos - startPos + 1);
                    break;
                case PathScanningState_PasswordEnd:
                    // The previous sub string is a password
                    entity.Password = path.substr(startPos, endPos - startPos + 1);
                    break;
                case PathScanningState_HostEnd:
                    // The previous sub string is a host identifier
                    entity.Host = path.substr(startPos, endPos - startPos + 1);
                    break;
                case PathScanningState_FolderEnd:
                case PathScanningState_EntityEnd:
                    // The previous sub string is a folder name
                    entity.Entries.push_back(path.substr(startPos, endPos - startPos + 1));
                    break;
                default:
                    break;
            }

            // Check if it is an entity end.
            if (delimiter == PathDelimiter_Entity)
            {
                // Add the current entity to the list
                Entities.push_back(entity);

                // Clear the current entity
                entity.Protocol = L"";
                entity.Host = L"";
                entity.Username = L"";
                entity.Password = L"";
                entity.Entries.clear();
            }
        }
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
