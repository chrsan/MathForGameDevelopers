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

#include <common_platform.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <string.h>

#include <mach-o/dyld.h>

#include <common.h>
#include <strutils.h>

#include <ApplicationServices/ApplicationServices.h>

std::string GetContentSubDirectory(const std::string& sContentSubPath)
{
    char exec_path[MAXPATHLEN];
    uint32_t size = sizeof(exec_path);

    if (_NSGetExecutablePath(exec_path, &size) != 0)
      return std::string();

    char real_path[MAXPATHLEN];
    if (realpath(exec_path, real_path) == NULL)
      return std::string();

    std::string full_path(real_path);
    size_t pos = full_path.find_last_of('/');
    std::string dir = full_path.substr(0, pos) + "/content/" + sContentSubPath;

    struct stat st;
    if ((stat(dir.c_str(), &st) != 0) || !S_ISDIR(st.st_mode))
      return std::string();

    return dir;
}

void GetScreenSize(int& iWidth, int& iHeight)
{
	CGDirectDisplayID id = CGMainDisplayID();
        size_t pw = CGDisplayPixelsWide(id);
        size_t ph = CGDisplayPixelsHigh(id);

        if (pw > INT_MAX || ph > INT_MAX) {
          fprintf(stderr, "Bogus screen size: width = %zd, height = %zd\n", pw, ph);
          exit(1);
        }

        iWidth = static_cast<int>(pw);
        iHeight = static_cast<int>(ph);
}

size_t GetNumberOfProcessors()
{
        fputs("Unimplemented", stderr);
	exit(1);
}

void SleepMS(size_t iMS)
{
	usleep(iMS);
}

void OpenBrowser(const std::string& sURL)
{
        fputs("Unimplemented", stderr);
	exit(1);
}

void OpenExplorer(const std::string& sDirectory)
{
        fputs("Unimplemented", stderr);
	exit(1);
}

void Alert(const std::string& sMessage)
{
	fputs(sMessage.c_str(), stderr);
}

void CreateMinidump(void* pInfo, char* pszDirectory)
{
        fputs("Unimplemented", stderr);
	exit(1);
}

std::string GetClipboard()
{
        fputs("Unimplemented", stderr);
	exit(1);
}

void SetClipboard(const std::string& sBuf)
{
        fputs("Unimplemented", stderr);
	exit(1);
}

std::string GetAppDataDirectory(const std::string& sDirectory, const std::string& sFile)
{
	char* pszVar = getenv("HOME");

	std::string sSuffix;
	sSuffix.append(".").append(sDirectory).append("/").append(sFile);

	std::string sReturn(pszVar);

	mkdir((std::string(sReturn).append("/").append(".").append(sDirectory)).c_str(), 0777);

	sReturn.append("/").append(sSuffix);
	return sReturn;
}

std::vector<std::string> ListDirectory(const std::string& sDirectory, bool bDirectories)
{
	std::vector<std::string> asResult;

	struct dirent *dp;

	DIR *dir = opendir((sDirectory).c_str());

	if (!dir)
		return asResult;

	while ((dp=readdir(dir)) != NULL)
	{
		if (!bDirectories && (dp->d_type == DT_DIR))
			continue;

		std::string sName = dp->d_name;
		if (sName == ".")
			continue;

		if (sName == "..")
			continue;

		asResult.push_back(sName);
	}
	closedir(dir);

	return asResult;
}

bool IsFile(const std::string& sPath)
{
	struct stat stFileInfo;
	bool blnReturn;
	int intStat;

	// Attempt to get the file attributes
	intStat = stat(sPath.c_str(), &stFileInfo);
	if(intStat == 0 && S_ISREG(stFileInfo.st_mode))
		return true;
	else
		return false;
}

bool IsDirectory(const std::string& sPath)
{
	struct stat stFileInfo;
	bool blnReturn;
	int intStat;

	// Attempt to get the file attributes
	intStat = stat(sPath.c_str(), &stFileInfo);
	if(intStat == 0 && S_ISDIR(stFileInfo.st_mode))
		return true;
	else
		return false;
}

void CreateDirectoryNonRecursive(const std::string& sPath)
{
	TUnimplemented();

	mkdir(sPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

bool CopyFileTo(const std::string& sFrom, const std::string& sTo, bool bOverride)
{
	TUnimplemented();

	int read_fd;
	int write_fd;
	struct stat stat_buf;
	off_t offset = 0;

	read_fd = open(sFrom.c_str(), O_RDONLY);

	if (!read_fd)
		return false;

	fstat(read_fd, &stat_buf);

	write_fd = open(sTo.c_str(), O_WRONLY | O_CREAT, stat_buf.st_mode);
	if (!write_fd)
	{
		close(read_fd);
		return false;
	}

	off_t size = stat_buf.st_size;

	sendfile(write_fd, read_fd, offset, &size, NULL, 0);

	close(read_fd);
	close(write_fd);

	return true;
}

std::string FindAbsolutePath(const std::string& sPath)
{
	TUnimplemented();

	char* pszFullPath = realpath(sPath.c_str(), nullptr);
	std::string sFullPath = pszFullPath;
	free(pszFullPath);

	return sFullPath;
}

time_t GetFileModificationTime(const char* pszFile)
{
	TUnimplemented();

	struct stat s;
	if (stat(pszFile, &s) != 0)
		return 0;

	return s.st_mtime;
}

void DebugPrint(const char* pszText)
{
	puts(pszText);
}

void Exec(const std::string& sLine)
{
        fputs("Unimplemented", stderr);
	exit(1);
}

// Not worried about supporting these on Linux right now.
int TranslateKeyToQwerty(int iKey)
{
	return iKey;
}

int TranslateKeyFromQwerty(int iKey)
{
	return iKey;
}
