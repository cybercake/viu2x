#ifndef VIUPATH_H
#define VIUPATH_H

using namespace std;

namespace viu2x {

    class ViuPath {
        public:
            ViuPath();
            ViuPath(const ViuPath & path);

            wstring getAsString();
            void setAsString(const wstring & path);
    };

}

#endif // VIUPATH_H
