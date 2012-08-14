#ifndef VIUPATH_H
#define VIUPATH_H

#include <string>

using namespace std;

namespace viu2x {

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
     * Path is platform-independent. All platform versions shares the same rule.
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
        public:
            const wstring PathDelimiter = L"/";
            const wstring EntityDelimiter = L">";

            Path();
            Path(const Path & path);

            wstring getAsString();
            void setAsString(const wstring & path);

            Path operator + (const Path & op);
            Path operator + (const wstring & op);

            Path operator - (const Path & op);
            Path operator - (const wstring & op);

            Path & operator -- ();
    };

}

#endif // VIUPATH_H
