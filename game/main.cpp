/*
Copyright (c) 2012, Lunar Workshop, Inc.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
   This product includes software developed by Lunar Workshop, Inc.
4. Neither the name of the Lunar Workshop nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY LUNAR WORKSHOP INC ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LUNAR WORKSHOP BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "game.h"

#if defined(__APPLE_CC__)

#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <mach-o/dyld.h>

static bool SetWorkingDirectory()
{
    char exec_path[MAXPATHLEN];
    uint32_t size = sizeof(exec_path);
    
    if (_NSGetExecutablePath(exec_path, &size) != 0)
        return false;
    
    char real_path[MAXPATHLEN];
    if (realpath(exec_path, real_path) == NULL)
        return false;
    
    std::string full_path(real_path);
    size_t pos = full_path.find_last_of('/');
    std::string contents_dir = full_path.substr(0, pos) + "/content";
    
    struct stat st;
    if ((stat(contents_dir.c_str(), &st) != 0) || !S_ISDIR(st.st_mode))
        return false;
    
    return chdir(contents_dir.c_str()) == 0;
}

#endif

int main(int argc, char* argv[])
{
#if defined(__APPLE_CC__)
	if (!SetWorkingDirectory())
    {
        std::cerr << "Could not set working directory" << std::endl;
        exit(1);
    }
#endif

	// Create a game
	CGame game(argc, argv);

	// Open the game's window
	game.OpenWindow(640, 480, false, false);
	game.SetMouseCursorEnabled(false);

	game.Load();

	// Run the game loop!
	game.GameLoop();

	return 0;
}
