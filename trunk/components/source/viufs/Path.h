#ifndef VIUPATH_H
#define VIUPATH_H

#include <string>
#include <vector>

namespace viu2x {

    /**
     * A path entity is a part of a path, which identifies a data entity in the current
     * parent container.
     */
    class Entity
    {
        public:
            std::wstring Protocol;
            std::wstring Username;
            std::wstring Password;
            std::wstring Host;
            std::vector<std::wstring> Entries;
    };

    /**
     * Path is a unique identifier for a data entity. It specifies the location of the
     * data entity and optionally the method to open it. Additionally, it may also contain
     * a user name and a password to achieve the privilige to access the required data.
     *
     * A Path is generally a sequence of entries. Each entry identifies a data container
     * in its hierachical level.
     *
     * A path can be absolute or relative. A relative path object can be concanated to
     * another absolution or relative path object. When using the relative path alone, it
     * will be automatically concatinated to the current path. The global current path
     * is always an absolute path.
     *
     * Path is platform-independent. All platform versions of viufs shares the same rule.
     *
     * Path object can be converted to a unicode string without losing any information and
     * vice versa.
     *
     * A path string consists of following elements:
     * - Host identifier:               The name of the host where the data is stored.
     * - Folder identifier:             A valid file name specified by the corresponding
     *                                  data storage e.g. a file system
     * - Protocol:                      A abbreviation of the way to open the data entity
     *                                  e.g. file, zip, flickr etc.
     * - Username:                      A string identifying the user on the target data
     *                                  storage.
     * - Password:                      The password to access the required data.
     * - Delimiters: The delimiter to seperate different elements.
     *   * Entity delimiter:            >
     *   * Protocol delimeter:          :
     *   * Host delimiter:              //
     *   * Folder delimiter:            /
     *   * Username/password section:   @
     *   * Username/password delimiter: :
     *
     * A full local file path would look like this:
     * file:///c/folder 1/folder 2/file.ext
     *
     * A remote file path:
     * file://username:password@host/shared folder 1/folder2/file.ext
     *
     * A file in a ZIP package on a remote host:
     * file://host/shared folder 1/folder2/package.zip>zip:///folder 1/folder 2/file.ext
     *
     * Some parts of a path can be omitted. For example:
     * - c/folder 1/folder 2/file.ext
     * - package.zip>folder 1/folder 2/file.ext
     * - flickr://flickrUser/Collection 1/Photo1.jpg
     * are valid paths.
     */
    class Path {
        protected:

            std::wstring serialize() const;
            void deserialize(const std::wstring & path);

        public:
            std::vector<Entity> Entities;

            /**
             * Entity delimiter is used to seperate data entities which should be open
             * using different methods.
             */
            static const std::wstring EntityDelimiter; // = L">";

            /**
             * Protocol delimiter is used to seperate the protocol abbreviation and host
             * or folder name. Protocol abbreviation must be followed by the protocol
             * delimiter.
             */
            static const std::wstring ProtocolDelimiter; // = L":";

            /**
             * Host delimiter is used to differentiate host section and folder names. A
             * host section always starts with the host delimiter.
             */
            static const std::wstring HostDelimiter; // = L"//";

            /**
             * Folder delimiter is used to seperate a folder name and the other parts. It
             * can be place after:
             * - The host section
             * - A folder name
             *
             * It is syntactically legal to place a folder delimiter at the end of an path
             * entity. Its meaning should be determined by specific file IO systems.
             */
            static const std::wstring FolderDelimiter; // = L"/";

            /**
             * User delimiter is used to seperate the user/password section and the host
             * name. It may appear ONLY in the host section i.e. between host delimiter
             * and the first folder delimiter.
             *
             * It is also a legal charater for folder and file names when it appears out
             * of the host section.
             */
            static const std::wstring UserDelimiter; // = L"@";

            /**
             * Password delimiter is used to seperate the username and the password in a
             * host section i.e. between host delimiter and user delimiter or folder
             * delimiter.
             */
            static const std::wstring PasswordDelimiter; // = L":";

            Path();
            Path(const Path & source);
            Path(const std::wstring & source);

            operator std::wstring() const;
            Path & operator = (const std::wstring & source);

            Path operator + (const Path & op) const;
            Path operator + (const std::wstring & op) const;

            Path operator - (const Path & op) const;
            Path operator - (const std::wstring & op) const;

            Path & operator -- ();
    };

}

#endif // VIUPATH_H
