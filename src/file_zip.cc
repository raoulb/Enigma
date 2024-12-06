/*
 * Copyright (C) 2006 Ronald Lamprecht
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

// Whoever knows how to integrate this code into file.cc should do it!
// The problem to solve is the clashing macro definitions in "zipios-config.h"
// and "config.h"
// The static var "zip" should be a private ivar of GameFS

#include "file.hh"
#include "errors.hh"
#include "zipios++/zipfile.h"
#include "zipios++/zipoutputstream.h"
#include "zipios++/zipoutputstreambuf.h"
#include "zipios++/zipinputstreambuf.h"
#include <istream>
#include <ostream>
#include <ctime>

using namespace enigma;
using namespace std;
using namespace zipios;

static std::unique_ptr<zipios::ZipFile> zip;
static std::string lastZipPath;
static std::string cachedZipString;

/*bool enigma::findInZip(std::string zipPath, std::string zippedFilename1,
                       std::string zippedFilename2, std::string &dest,
                       std::unique_ptr<std::istream> &isresult) {

    // reuse last opened zip if possible
    if (lastZipPath != zipPath) {
         zip.reset (new zipios::ZipFile (zipPath));
         lastZipPath = zipPath;
    }
    if (auto isptr = zip->getInputStream(zippedFilename2)) {
        isresult.reset(isptr);
        dest = zippedFilename2;
        return true;
    }
    if (auto isptr = zip->getInputStream(zippedFilename1)) {
        isresult.reset(isptr);
        dest = zippedFilename1;
        return true;
    }
    return false;
}*/

bool enigma::findInZip(std::string zipPath, std::string zippedFilename1,
                       std::string zippedFilename2, std::string &dest,
                       std::string &inflatedString) {
    // reuse last opened zip if possible
    if (lastZipPath != zipPath) {
        std::basic_ifstream<char> ifs(zipPath.c_str(), ios::binary | ios::in);
        if(not ifs) // error opening zip file
            return false;
        ByteVec intermediary;
        Readfile(ifs, intermediary);
        cachedZipString = std::string(intermediary.begin(), intermediary.end());
        lastZipPath = zipPath;
    }
    try {
        inflatedString = extractFromZipString(cachedZipString, zippedFilename2);
        dest = zippedFilename2;
    } catch(...) {
        try {
            inflatedString = extractFromZipString(cachedZipString, zippedFilename1);
            dest = zippedFilename1;
        } catch(...) {
            // Not found in zip, or not valid, or read error.
            return false;
        }
    }
    return true;
}

std::string enigma::extractFromZipString(std::string zipString, std::string fileName) {
    size_t compressedSize = 0;
    size_t uncompressedSize = 0;
    size_t posLocalHeader = 0;
    Bytef *ptrCompressed = NULL;
    Bytef *ptrUncompressed = NULL;
    std::string inflatedContents = "";
    try {
        // Search for the first occurence of fileName, from back to front.
        size_t pos = zipString.rfind(fileName);
        while(pos != std::string::npos) {
            // get compressed size at pos - 26
            compressedSize = ecl::string_to_uint32(zipString.substr(pos - 26, 4));
            // get uncompressed size at pos - 22
            uncompressedSize = ecl::string_to_uint32(zipString.substr(pos - 22, 4));
            // get position of beginning of local header at pos - 4
            posLocalHeader = ecl::string_to_uint32(zipString.substr(pos - 4, 4));
            // check the signature
            if(zipString.substr(posLocalHeader, 4) == "\x50\x4B\x03\x04")
                // we found it
                break;
            pos = zipString.rfind(fileName, pos - 1);
        }
        ASSERT(pos != std::string::npos, XFrontend, "No file of this name found in zip file.");
        uint32_t fileNameLength = ecl::string_to_uint32(zipString.substr(posLocalHeader + 26, 2));
        ASSERT(fileNameLength == fileName.length(), XFrontend, "Error in zip file.");
        uint32_t extraFieldLength = ecl::string_to_uint32(zipString.substr(posLocalHeader + 28, 2));
        std::string compressedData = zipString.substr(
            posLocalHeader + 30 + fileNameLength + extraFieldLength, uncompressedSize);

        // We need to allocate enough memory to save the inflated (decompressed) xml-file.
        ptrCompressed = (Bytef*)(compressedData.data());
        ptrUncompressed = (Bytef*)(calloc(uncompressedSize, 1));
        ASSERT(ptrUncompressed != NULL, XFrontend, "Could not allocate memory for zip inflation.");

        // Prepare a zlib-stream for inflation.
        z_stream inflateStream;
        inflateStream.zalloc = Z_NULL;
        inflateStream.zfree = Z_NULL;
        inflateStream.opaque = Z_NULL;
        inflateStream.avail_in = compressedSize;
        inflateStream.next_in = ptrCompressed;
        inflateStream.avail_out = uncompressedSize;
        inflateStream.next_out = ptrUncompressed;

        // Inflate using zlib.
        int err = inflateInit2(&inflateStream, -15);
        ASSERT(err != Z_MEM_ERROR, XFrontend, "Not enough memory allocated for inflation.");
        ASSERT(err == Z_OK, XFrontend, "Error during zip inflation init.");
        err = inflate(&inflateStream, Z_FINISH);
        inflatedContents = std::string((char*)ptrUncompressed, uncompressedSize);
        //Log << inflatedContents << "\n";
        ASSERT(err != Z_NEED_DICT, XFrontend, "Z_NEED_DICT");
        ASSERT(err != Z_DATA_ERROR, XFrontend, "Z_DATA_ERROR");
        ASSERT(err != Z_STREAM_ERROR, XFrontend, "Z_STREAM_ERROR");
        ASSERT(err != Z_MEM_ERROR, XFrontend, "Z_MEM_ERROR");
        ASSERT(err != Z_BUF_ERROR, XFrontend, "Z_BUF_ERROR");
        ASSERT(err != Z_OK, XFrontend, "Z_OK");
        ASSERT(err == Z_OK || err == Z_STREAM_END, XFrontend, "Error during zip inflation.");
        inflateEnd(&inflateStream);
        ASSERT(err != Z_OK, XFrontend, "Z_OK during inflateEnd");
        ASSERT(err == Z_OK || err == Z_STREAM_END, XFrontend, "Error during zip inflation cleanup.");
        ASSERT(uncompressedSize == (int)(inflateStream.next_out - ptrUncompressed), XFrontend, "Uncompressed size incorrect. Maybe a broken zip file?");
        inflatedContents = std::string((char*)ptrUncompressed, uncompressedSize);
        // TODO: Error messages are not thrown correctly, but get ignored somewhere.
    }
    catch (...) {
        if(ptrUncompressed)
            free(ptrUncompressed);
        throw;
    }
    if(ptrUncompressed)
        free(ptrUncompressed);
    return inflatedContents;
}
