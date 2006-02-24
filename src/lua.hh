/*
 * Copyright (C) 2002,2003,2004 Daniel Heck
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
 *
 */
#ifndef LUA_HH
#define LUA_HH

#include "enigma.hh"
#include "ecl_geom.hh"

extern "C" {
    struct lua_State;
}

namespace lua
{
    using namespace enigma;

/* -------------------- Data structures -------------------- */

    struct CFunction {
        int (*func) (lua_State *); // lua_CFunction func;
        const char *name;
    };

    enum Error {
        NOERROR = 0,
        ERRRUN,
	ERRFILE,
	ERRSYNTAX,
	ERRMEM,
	ERRERR
    };

/* -------------------- Lua states for Enigma -------------------- */

    lua_State *GlobalState();
    void ShutdownGlobal();

    /*! Return the Lua state that is active while the game is running.
      This state is used to execute the Lua code that builds the level
      and to execute signal callbacks and event handlers while the
      game is running. */
    lua_State *LevelState();

    /*! Initialize the ingame Lua state. */
    lua_State *InitLevel();

    /*! Close the ingame Lua state. */
    void ShutdownLevel();


/* -------------------- Enigma/Lua bindings -------------------- */

    int FindDataFile (lua_State *L);

    void SetSoundTable (const char *name);

/* -------------------- Helper routines -------------------- */

    /*! Register the C functions in `funcs'.  The end of the array is
      denoted by an entry with func==0. */
    void RegisterFuncs (lua_State *L, CFunction funcs[]);

    /*! Set the value of entry `name' in the global table `tablename'. */
    void SetTableVar (lua_State *L, const char *tablename, const char *name, double value);

    /*! Call a Lua function with one argument.  This is mainly used
      for callbacks during the game. */
    Error CallFunc(lua_State *L, const char *funcname, const enigma::Value& arg);

    /*! Call a Lua function with a (large) byte vector as the sole
      argument.  Currently only used for loading XML levels. */
    Error CallFunc (lua_State *L, const char *funcname, const ByteVec &arg);

    /*! Find a Lua script using enigma::FindFile and run it. */
    Error Dofile (lua_State *L, const std::string & filename);

    /*! Find a system Lua script using enigma::FindFile and run it. */
    Error DoSysFile (lua_State *L, const std::string & filename);

    /*! Find a Lua script in given filesystem using enigma::FindFile and run it. */
    Error DoGeneralFile(lua_State *L, GameFS * fs, const string &filename);

    /*! Run the Lua code contained in `luacode'. */
    Error Dobuffer (lua_State *L, const ByteVec &luacode);

    /*! Try to run given file on given filesystem.  If something
      fails, provide generic error message and exit enigma.*/
    void CheckedDoFile (lua_State *L, GameFS * fs, const string &filename);

    /*! Return the text of the last error message. */
    std::string LastError (lua_State *L);


    Error DoSubfolderfile(lua_State *L, 
                        const std::string & basefolder, 
                        const std::string & filename);
}
#endif
